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
  clib_error_t *error = 0;
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
  .short_help = "set udps policy-action <action-name> "
	        "[offset <offset-val> [ [insert|replace] rewrite_string <rewrite-val>] | remove <byte-len>]"
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
  .short_help = "set udps policy <policy-name> rule <id>"
	        " [ offset <offset-val> val <match-val> ] [ action <action-name> ]",
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
  .short_help = "set udps interface <interface-name> direction <rx|tx> policy <policy-name>",
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


