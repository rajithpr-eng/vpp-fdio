/*
 * Copyright (c) 2020 Cisco and/or its affiliates.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "udps_includes.h"

static clib_error_t *
udps_policy_action_set (vlib_main_t * vm,
		        unformat_input_t * input, vlib_cli_command_t * cmd)
{
  vnet_main_t *vnm = vnet_get_main ();	
  u8* action_name = 0;
  u8* rewrite_str = 0;
  u32 out_port = UDPS_NO_PORT;
  clib_error_t *error = 0;
  u32 offset_val = 0, len = 0;
  enum udps_rewrite_oper_e oper = UDPS_REWRITE_UNUSED;
  u8 offset_set = 0, name_set = 0, insert_set = 0, replace_set = 0, remove_set = 0, out_port_set = 0, rewrite_str_set = 0;
  while (unformat_check_input(input) != UNFORMAT_END_OF_INPUT) {
    if (unformat(input, "name %s", &action_name)) {
      name_set = 1;
    } else if (unformat(input, "offset %u", &offset_val)) {
      offset_set = 1;
    } else if (unformat(input, "insert")) {
      insert_set = 1;
    } else if (unformat(input, "replace")) {
      replace_set = 1;
    } else if (unformat(input, "rewrite_string %U", unformat_hex_string, &rewrite_str)) {
      rewrite_str_set = 1;
    } else if (unformat(input, "remove %u", &len)) {
      remove_set = 1;
    } else if (unformat(input, "out-port %U", unformat_vnet_sw_interface, vnm, &out_port)) {
      out_port_set = 1;
    } else {
      vlib_cli_output(vm, "ERROR: Specify policy-action name <action-name>"
                          " [offset <val> [ [insert|replace] rewrite_string <val>]"
                          " | remove <byte-len>] [out-port <interface-name>]\n");
      return 0;
    }
  }

  if (!name_set || !offset_set) {
    vlib_cli_output(vm, "ERROR: Invalid Command");
    return error;
  } 

  if (replace_set) {
    oper = UDPS_REWRITE_REPLACE; 
  } else if (remove_set) {
    oper = UDPS_REWRITE_REMOVE;
  } else if (insert_set) {
    oper = UDPS_REWRITE_INSERT;
  } else {
    oper = UDPS_REWRITE_UNUSED;
  }
  udps_db_rule_action_add(action_name, oper, offset_val, rewrite_str, len, out_port);
  vlib_cli_output(vm, "Policy action added successfuly.\n");
  return error;
}

/*
 * This Commands configure policy-action for UDPS feature
 * set udps policy-action <action-name> [offset <val> [ [insert|replace] <rewrite_string <val>] | remove <byte-len>] [out-port <interface-name>]
 * Notes: above command can be usede to add multiple rewrites, single port redirect to same policy action (identified by action-name). 
*/

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (set_udps_policy_action_command, static) = {
  .path = "set udps policy-action",
  .function = udps_policy_action_set,
  .short_help = "set udps policy-action name <action-name> "
	        "[offset <offset-val> [ [insert|replace] rewrite_string <rewrite-val>] | [remove <byte-len>]]"
		" [out-port <interface-name>]",
};
/* *INDENT-ON* */

char* udps_oper_id_to_name(u8 oper) 
{
  switch(oper) {
    case UDPS_REWRITE_UNUSED:
      return "UDPS_REWRITE_UNUSED";
    case UDPS_REWRITE_INSERT:
      return "UDPS_REWRITE_INSERT";
    case UDPS_REWRITE_REPLACE:
      return "UDPS_REWRITE_REPLACE";
    case UDPS_REWRITE_REMOVE:
      return "UDPS_REWRITE_REMOVE";
    default:
      return "";
  }
  return "";
}

void 
udps_dump_policy_action (vlib_main_t * vm, u8* action_name) 
{
  udps_rule_action_t *ra;
  char delimiter[65] = ""; 
  bool ret = udps_db_rule_action_get(action_name, &ra);
  if (!ret) {
     vlib_cli_output(vm, "No Policy Action found for given name = %s", action_name);
     return;
  }
  vlib_cli_output(vm, "Policy Action");
  for (int i = 0; i < 65; i++) {
     strcat(delimiter, "-");
  }
  vlib_cli_output(vm, "%s\n", delimiter);
  vlib_cli_output(vm,"%-40s : %s", "Policy Action name", ra->name);
  vlib_cli_output(vm,"%-40s : %d", "Out-port", ra->out_port);
  vlib_cli_output(vm,"%-40s : %s", "Oper", udps_oper_id_to_name(ra->rewrite->oper));
  vlib_cli_output(vm,"%-40s : %d", "Packet Offset", ra->rewrite->offset);
  if (ra->rewrite->oper == UDPS_REWRITE_REMOVE) {
     vlib_cli_output(vm,"%-40s : %d", "Rewrite Length", ra->rewrite->len);
  }
  vlib_cli_output(vm,"%-40s : %U", "Value", format_hex_bytes, ra->rewrite->value, vec_len(ra->rewrite->value));
}

static clib_error_t *
udps_policy_action_show (vlib_main_t * vm,
		         unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  u8* action_name = 0;
  u8 name_set = 0;
  while (unformat_check_input(input) != UNFORMAT_END_OF_INPUT) {
    if (unformat(input, "name %s", &action_name)) {
      name_set = 1;
    } else {
      vlib_cli_output(vm, "ERROR: Cli not in correct form\n");
      return 0;
    }
  }

  if (!name_set) {
    vlib_cli_output(vm, "ERROR: Policy action name needed!");
    return error;
  } 
  udps_dump_policy_action(vm, action_name);
  return error;
}

/*
 * This Commands show policy-action for UDPS feature
 * show udps policy-action <action-name>
*/

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (show_udps_policy_action_command, static) = {
  .path = "show udps policy-action",
  .function = udps_policy_action_show,
  .short_help = "show udps policy-action name <action-name>",
  .is_mp_safe = 1,
};
/* *INDENT-ON* */

static clib_error_t *
udps_policy_action_delete (vlib_main_t * vm,
		           unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  vlib_cli_output(vm, "udps_policy_action_delete, WORK in PROGRESS, retry later\n");
  return error;
}

/*
 * This Commands delete policy-action for UDPS feature
 * delete udps policy-action <action-name>
*/

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (delete_udps_policy_action_command, static) = {
  .path = "delete udps policy-action",
  .function = udps_policy_action_delete,
  .short_help = "delete udps policy-action <action-name>",
  .is_mp_safe = 1,
};
/* *INDENT-ON* */

static clib_error_t *
udps_policy_set (vlib_main_t * vm,
		 unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  u8* policy_name = 0, *action_name = 0;
  u8* val_str = 0;
  u32 offset_val = 0, id = 0;
  u8  offset_set = 0, name_set = 0, action_set = 0, rule_id_set = 0, val_str_set = 0;
  
  enum udps_match_oper_e oper;

  while (unformat_check_input(input) != UNFORMAT_END_OF_INPUT) {
    if (unformat(input, "name %s", &policy_name)) {
      name_set = 1;
    } else if(unformat(input, "rule %u", &id)) {
      rule_id_set = 1;
    } else if (unformat(input, "offset %u", &offset_val)) {
      offset_set = 1;
    } else if (unformat(input, "val %U", unformat_hex_string, &val_str)) {
      val_str_set = 1;
    } else if (unformat(input, "action %s", &action_name)) {
      action_set = 1;
    } else {
      vlib_cli_output(vm, "ERROR: CLI not in proper form\n");
      return 0;
    }
  }
  
  if (!name_set || !rule_id_set) {
    vlib_cli_output(vm, "ERROR: Invalid Command");
    return error;
  } 

  if (offset_set) {
    oper = UDPS_MATCH_PKT;
  } else {
    oper = UDPS_MATCH_UNUSED;  
  }

  udps_db_rule_entry_add(policy_name, id, oper, offset_val, val_str, action_name);
  vlib_cli_output(vm, "Policy Rule addedd successfully.\n");
  return error;
}

/*
 * This Commands configure policy for UDPS feature
 * set udps policy <policy-name> rule <id> [ offset <offset-val> val <match-val> ] [ action <action-name> ]
 * Notes: above command can be usede to add multiples rules to a policy. Mulitiple matches in each rule are allowed. For now, single action in a rule. 
*/

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (set_udps_policy_command, static) = {
  .path = "set udps policy",
  .function = udps_policy_set,
  .short_help = "set udps policy name <policy-name> rule <id> [ offset <offset> val <val>] [ action <action-name> ]",
};
/* *INDENT-ON* */

void 
udps_dump_policy_rule_by_rule_entry (vlib_main_t * vm, udps_rule_entry_t *re, char *delimiter) 
{
  vlib_cli_output(vm,"%-40s : %u", "Rule Id", re->rule_id);
  vlib_cli_output(vm,"%-40s : %u", "Action Id", re->act_id);
  vlib_cli_output(vm, "%s\n", delimiter);
  vlib_cli_output(vm, "%-40s | %s", "Packet Offset", "Value");
  for (int i = 0; i<vec_len(re->match_pkt); i++) {
    vlib_cli_output(vm,"%-40d | %U", re->match_pkt[i].offset, format_hex_bytes, re->match_pkt[i].value, vec_len(re->match_pkt[i].value));
  }
  vlib_cli_output(vm, "%s\n", delimiter);
}

void 
udps_dump_policy_rule_by_policy_entry (vlib_main_t * vm, udps_policy_entry_t *pe, char *delimiter)
{
  vlib_cli_output(vm,"%-40s : %s", "Policy Name", pe->name);
  vlib_cli_output(vm, "%s\n", delimiter);

  for (int i=0;i< vec_len(pe->rules); i++) {
     if (pe->rules[i].rule_id == UDPS_INVALID_RULE){
        continue;
     }
     vlib_cli_output(vm, "\n");
     udps_dump_policy_rule_by_rule_entry(vm, &pe->rules[i], delimiter);
  }  
}

void 
udps_dump_policy_rule (vlib_main_t * vm, u8* policy_name) 
{
  udps_policy_entry_t *pe;
  char delimiter[65] = "";
  bool ret = udps_db_rule_policy_get(policy_name, &pe);
  if (!ret) {
     vlib_cli_output(vm, "No policy rule found for given name:%s", policy_name);
     return;
  }
  for (int i = 0; i < 65; i++) {
     strcat(delimiter, "-");
  }
  udps_dump_policy_rule_by_policy_entry(vm, pe, delimiter);  
}

void 
udps_dump_policy_rule_by_id (vlib_main_t * vm, u8* policy_name, u32 id) 
{
  udps_rule_entry_t *re;
  char delimiter[65] = "";
  bool ret = udps_db_rule_entry_get(policy_name, id, &re);
  if (!ret) {
     vlib_cli_output(vm, "No policy rule found for given name:%s and id:%d", policy_name, id);
     return;
  }
  vlib_cli_output(vm,"%-40s : %s", "Policy name", policy_name);
  for (int i = 0; i < 65; i++) {
     strcat(delimiter, "-");
  }
  vlib_cli_output(vm, "%s\n", delimiter);
  udps_dump_policy_rule_by_rule_entry(vm, re, delimiter);  
}

static clib_error_t *
udps_policy_show (vlib_main_t * vm,
		  unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  u8* policy_name = 0;
  u32 id = 0;
  u8 name_set = 0, rule_id_set = 0;
  while (unformat_check_input(input) != UNFORMAT_END_OF_INPUT) {
    if (unformat(input, "name %s", &policy_name)) {
      name_set = 1;
    } else if(unformat(input, "rule %u", &id)) {
      rule_id_set = 1;
    } else {
      vlib_cli_output(vm, "ERROR: CLI not in proper form\n");
      return 0;
    }
  }
  
  if (!name_set) {
    vlib_cli_output(vm, "ERROR: Policy name");
    return error;
  } 

  if (rule_id_set) {
    udps_dump_policy_rule_by_id(vm, policy_name, id);
  } else {
    udps_dump_policy_rule(vm, policy_name);	  
  }

  return error;
}

/*
 * This Commands show policy for UDPS feature
 * show udps policy <action-name>
*/

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (show_udps_policy_command, static) = {
  .path = "show udps policy",
  .function = udps_policy_show,
  .short_help = "show udps policy name <policy-name> [rule <id>]",
  .is_mp_safe = 1,
};
/* *INDENT-ON* */

static clib_error_t *
udps_policy_delete (vlib_main_t * vm,
		           unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  vlib_cli_output(vm, "udps_policy_delete, WORK in PROGRESS, retry later\n");
  return error;
}

/*
 * This Commands delete policy for UDPS feature
 * delete udps policy <action-name>
*/

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (delete_udps_policy_command, static) = {
  .path = "delete udps policy",
  .function = udps_policy_delete,
  .short_help = "delete udps policy <policy-name>",
  .is_mp_safe = 1,
};
/* *INDENT-ON* */

static clib_error_t *
udps_policy_interface_set (vlib_main_t * vm,
		           unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  u32 if_name;
  vnet_main_t *vnm = vnet_get_main ();
  u8* policy_name = 0;
  u8 rx_set = 0, tx_set = 0, policy_set = 0, name_set = 0;
  
  while (unformat_check_input(input) != UNFORMAT_END_OF_INPUT) {
    if (unformat(input, "name %U", unformat_vnet_sw_interface, vnm, &if_name)) {
      name_set = 1;
    } else if(unformat(input, "direction")) {
       if (unformat(input, "rx")) {
         rx_set = 1;
       } else if (unformat(input, "tx")) {
         tx_set = 1;
       } else {
         vlib_cli_output(vm, "ERROR: Invalid direction\n");
	 return 0;
       }
    } else if (unformat(input, "policy %s", &policy_name)) {
      policy_set = 1;
    } else {
      vlib_cli_output(vm, "ERROR: CLI not in proper form\n");
      return 0;
    }
  }
  
  if (!name_set || !(rx_set || tx_set) || !policy_set ) {
    vlib_cli_output(vm, "ERROR: Invalid Command");
    return error;
  } 

  udps_db_policy_apply(if_name, rx_set, policy_name);
  udps_node_policy_apply(if_name, rx_set);
  vlib_cli_output(vm, "Policy applied on interface successfully\n");
  return error;
}

/*
 * This Commands apply UDPS feature policy on interface
 * set udps interface <interface-name> direction <rx|tx> policy <policy-name>
*/

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (set_udps_policy_interface_command, static) = {
  .path = "set udps interface",
  .function = udps_policy_interface_set,
  .short_help = "set udps interface name <interface-name> direction <rx|tx> policy <policy-name>",
};
/* *INDENT-ON* */

void 
udps_dump_interface_policy (vlib_main_t * vm, u32 if_index, u8 is_rx) 
{
  udps_policy_entry_t *pe;
  char delimiter[65] = "";
  bool ret = udps_db_policy_get_by_sw_if_index(if_index, is_rx, &pe);
  if (!ret) {
     vlib_cli_output(vm, "No policy entry found for given if_index:%d and direction :%s",
		     if_index, is_rx?"RX":"TX");
     return;
  }
  vlib_cli_output(vm, "Interface Policy sw_if_index=%d Direction=%s", if_index, is_rx?"RX":"TX");
  for (int i = 0; i < 65; i++) {
     strcat(delimiter, "-");
  }
  vlib_cli_output(vm, "%s\n\n", delimiter);
  udps_dump_policy_rule_by_policy_entry(vm, pe, delimiter);
}

static clib_error_t *
udps_policy_interface_show (vlib_main_t * vm,
		  unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  vnet_main_t *vnm = vnet_get_main ();
  u32 if_name;
  u8 name_set = 0;
  u8 rx_set = 0, tx_set = 0;
  while (unformat_check_input(input) != UNFORMAT_END_OF_INPUT) {
    if (unformat(input, "name %U", unformat_vnet_sw_interface, vnm, &if_name)) {
      name_set = 1;
    } else if(unformat(input, "direction")) {
       if (unformat(input, "rx")) {
         rx_set = 1;
       } else if (unformat(input, "tx")) {
         tx_set = 1;
       } else {
         vlib_cli_output(vm, "ERROR: Invalid direction\n");
	 return 0;
       }
    } else {
      vlib_cli_output(vm, "ERROR: CLI not in proper form\n");
      return 0;
    }
  }

  if (!name_set  || (!rx_set && !tx_set)) {
     vlib_cli_output(vm, "Error: Interface name and direction required\n");
     return 0;
  }
  
  udps_dump_interface_policy (vm, if_name, rx_set);
  return error;
}

/*
 * This Commands show policy for UDPS feature on interface
 * show udps interface <interface-name>
*/

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (show_udps_policy_interface_command, static) = {
  .path = "show udps interface",
  .function = udps_policy_interface_show,
  .short_help = "show udps interface name <interface-name> direction <rx|tx>",
  .is_mp_safe = 1,
};
/* *INDENT-ON* */

static clib_error_t *
udps_policy_interface_delete (vlib_main_t * vm,
		              unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  vlib_cli_output(vm, "udps_policy_interface_delete, WORK in PROGRESS, retry later\n");
  return error;
}

/*
 * This Commands delete policy for UDPS feature on an interface
 * delete udps interface <interface-name> direction <rx|tx>
*/

/* *INDENT-OFF* */
VLIB_CLI_COMMAND (delete_udps_policy_interface_command, static) = {
  .path = "delete udps interface",
  .function = udps_policy_interface_delete,
  .short_help = "delete udps interface <interface-name> direction <rx|tx>",
  .is_mp_safe = 1,
};
/* *INDENT-ON* */


