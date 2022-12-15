#ifndef __UDPS_H__
#define __UDPS_H__

void udps_node_policy_apply(uint32_t sw_if_index, uint8_t is_rx);
void udps_node_policy_remove(uint32_t sw_if_index, uint8_t is_rx);

#endif /* __UDPS_H__ */
