#include "udps_includes.h"

void
udps_node_policy_apply(u32 sw_if_index, u8 is_rx)
{
    if (is_rx) {
        vnet_feature_enable_disable("device-input",
		                    "udps-rx-node",
				     sw_if_index,
				     1, 0, 0);
    } else {
        vnet_feature_enable_disable("interface-output",
		                    "udps-tx-node",
				     sw_if_index,
				     1, 0, 0);
    }
}

void
udps_node_policy_remove(u32 sw_if_index, u8 is_rx)
{
    if (is_rx) {
        vnet_feature_enable_disable("device-input",
		                    "udps-rx-node",
				     sw_if_index,
				     0, 0, 0);
    } else {
        vnet_feature_enable_disable("interface-output",
		                    "udps-tx-node",
				     sw_if_index,
				     0, 0, 0);
    }
}

VLIB_NODE_FN (udps_rx_node) (vlib_main_t *vm,
                             vlib_node_runtime_t *node,
                             vlib_frame_t *frame)
{
  return 0;
}

VLIB_NODE_FN (udps_tx_node) (vlib_main_t *vm,
                             vlib_node_runtime_t *node,
                             vlib_frame_t *frame)
{
  return 0;
}

/* *INDENT-OFF* */
VLIB_REGISTER_NODE (udps_rx_node) =
{
    .name          = "udps-rx-node",
    .vector_size   = sizeof (u32),
    .format_trace  = NULL,
    .type          = VLIB_NODE_TYPE_INTERNAL,
    .n_next_nodes  = VNET_DEVICE_INPUT_N_NEXT_NODES,
    .next_nodes    = VNET_DEVICE_INPUT_NEXT_NODES,
    .n_errors      = 0,
    .error_strings = NULL,
};

VNET_FEATURE_INIT (udps_rx_node, static) =
{
    .arc_name    = "device-input",
    .node_name   = "udps-rx-node",
    .runs_before = VNET_FEATURES ("ethernet-input"),
};

VLIB_REGISTER_NODE (udps_tx_node) =
{
    .name          = "udps-tx-node",
    .vector_size   = sizeof (u32),
    .format_trace  = NULL,
    .type          = VLIB_NODE_TYPE_INTERNAL,
    .n_next_nodes  = 0,
    .n_errors = 0,
    .error_strings = NULL,
};

VNET_FEATURE_INIT (udps_tx_node, static) =
{
    .arc_name    = "interface-output",
    .node_name   = "udps-tx-node",
    .runs_before = VNET_FEATURES ("interface-tx"),
};

VLIB_PLUGIN_REGISTER () =
{
    .version = VPP_BUILD_VER,
    .description = "user defined packet steer plugin registration",
};
/* *INDENT-ON* */
