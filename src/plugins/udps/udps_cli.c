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
  vlib_cli_output(vm, "Got below params: action name=%s, offset=%u, oper=%u, len=%d, out_port=%d",
                       action_name, offset_val, oper, len, out_port);

  vlib_cli_output(vm, "\n Hex rewrite_str :");
  for (int i =0; i<vec_len(rewrite_str);i++) {
       vlib_cli_output(vm, "%x", rewrite_str[i]);
  } 
  udps_db_rule_action_add(action_name, oper, offset_val, rewrite_str, len, out_port);
  vlib_cli_output(vm, "udps_policy_action_set, WORK in PROGRESS, retry later\n");
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


static clib_error_t *
udps_policy_action_show (vlib_main_t * vm,
		         unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  vlib_cli_output(vm, "udps_policy_action_show, WORK in PROGRESS, retry later\n");
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
  .short_help = "show udps policy-action <action-name>",
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

  vlib_cli_output(vm, "Got below params: action name=%s, rule=%u, offset=%u, action=%s",
                       policy_name, id, offset_val, action_name); 

  vlib_cli_output(vm, "\n Hex val_str :");
  for (int i =0; i<vec_len(val_str);i++) {
    vlib_cli_output(vm, "%x", val_str[i]);
  }  

  if (offset_set) {
    oper = UDPS_MATCH_PKT;
  } else {
    oper = UDPS_MATCH_UNUSED;  
  }

  udps_db_rule_entry_add(policy_name, id, oper, offset_val, val_str, action_name);
  vlib_cli_output(vm, "udps_policy_set, WORK in PROGRESS, retry later\n");
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


static clib_error_t *
udps_policy_show (vlib_main_t * vm,
		  unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  vlib_cli_output(vm, "udps_policy_show, WORK in PROGRESS, retry later\n");
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
  .short_help = "show udps policy <policy-name>",
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

  vlib_cli_output(vm, "Got below params: if name=%d, rx_set=%d, tx_set=%d, policy=%s",
                       if_name, rx_set, tx_set, policy_name); 
  udps_db_policy_apply(if_name, rx_set, policy_name);
  vlib_cli_output(vm, "udps_policy_interface_set, WORK in PROGRESS, retry later\n");
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


static clib_error_t *
udps_policy_interface_show (vlib_main_t * vm,
		  unformat_input_t * input, vlib_cli_command_t * cmd)
{
  clib_error_t *error = 0;
  vlib_cli_output(vm, "udps_policy_show, WORK in PROGRESS, retry later\n");
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
  .short_help = "show udps interface <interface-name>",
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


