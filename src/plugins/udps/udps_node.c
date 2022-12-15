#include "udps_includes.h"

#define foreach_udps_rx_error          \
    _(UDPS_RX,    "UDPS_RX outgoing packets")       \
    _(DROP,       "UDPS_RX drops")

typedef enum
{
#define _(sym,str) UDPS_RX_ERROR_##sym,
      foreach_udps_rx_error
#undef _
          UDPS_RX_N_ERROR,
} udps_rx_error_t;

static char *udps_rx_error_strings[] = {
#define _(sym,string) string,
      foreach_udps_rx_error
#undef _
};

#define foreach_udps_tx_error          \
    _(UDPS_TX,    "UDPS_TX outgoing packets")       \
    _(DROP,       "UDPS_TX drops")

typedef enum
{
#define _(sym,str) UDPS_TX_ERROR_##sym,
      foreach_udps_tx_error
#undef _
          UDPS_TX_N_ERROR,
} udps_tx_error_t;

static char *udps_tx_error_strings[] = {
#define _(sym,string) string,
      foreach_udps_tx_error
#undef _
};

typedef enum
{
    UDPS_TX_NEXT_DROP,
    UDPS_TX_NEXT_INTERFACE_TX,
    UDPS_TX_N_NEXT,
} udps_tx_next_t;

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

static uword
udps_node_common_internal  (vlib_main_t *vm,
                            vlib_node_runtime_t *node,
                            vlib_frame_t *frame,
                            bool is_rx)
{
    return 0;
}

VLIB_NODE_FN (udps_rx_node) (vlib_main_t *vm,
                             vlib_node_runtime_t *node,
                             vlib_frame_t *frame)
{
    u32 counter[UDPS_RX_N_ERROR] = {0};
    udps_node_common_internal(vm, node, frame, true);
    counter[UDPS_RX_ERROR_UDPS_RX]++;
#define _(n, s)                                                            \
{                                                                          \
    if (counter[UDPS_RX_ERROR_##n]) {                                      \
        vlib_node_increment_counter(vm, node->node_index,                  \
                                    UDPS_RX_ERROR_##n,                     \
                                    counter[UDPS_RX_ERROR_##n]);           \
    }                                                                      \
}
        foreach_udps_rx_error
#undef _
    return 0;
}

VLIB_NODE_FN (udps_tx_node) (vlib_main_t *vm,
                             vlib_node_runtime_t *node,
                             vlib_frame_t *frame)
{
    u32 counter[UDPS_TX_N_ERROR] = {0};
    udps_node_common_internal(vm, node, frame, false);
    counter[UDPS_TX_ERROR_UDPS_TX]++;
#define _(n, s)                                                            \
{                                                                          \
    if (counter[UDPS_TX_ERROR_##n]) {                                      \
        vlib_node_increment_counter(vm, node->node_index,                  \
                                    UDPS_TX_ERROR_##n,                     \
                                    counter[UDPS_TX_ERROR_##n]);           \
    }                                                                      \
}
        foreach_udps_tx_error
#undef _

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
    .n_errors      = ARRAY_LEN(udps_rx_error_strings),
    .error_strings = udps_rx_error_strings,
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
    .n_next_nodes  = UDPS_TX_N_NEXT,
    .n_errors      = ARRAY_LEN(udps_tx_error_strings),
    .error_strings = udps_tx_error_strings,
    .next_nodes    = {
        [UDPS_TX_NEXT_DROP] = "error-drop",
        [UDPS_TX_NEXT_INTERFACE_TX] = "interface-tx",
    }

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
