#include "udps_includes.h"

void
udps_db_rule_action_add(u8 *name, u8 oper, u16 offset, u8 *value, u8 len, u32 out_port)
{
}

bool
udps_db_rule_action_del(u8 *name)
{
    return true;
}

bool
udps_db_rule_action_get(u8 *name, udps_rule_action_t **ra)
{
    return true;
}

bool
udps_db_rule_match(u32 sw_if_index, u8 is_rx, u8 *bytes, udps_rule_action_t **ra)
{
    return true;
}

bool
udps_db_rule_entry_add(u8 *name, u8 id, u8 oper, u16 offset, u8 *value, u8 *aname)
{
    return true;
}

u8
udps_db_rule_entry_cnt(u8 *name)
{
    return 0;
}

bool
udps_db_rule_entry_get(u8 *name, u8 id, udps_rule_entry_t **re)
{
    return true;
}

bool
udps_db_rule_policy_del(u8 *name)
{
    return true;
}

void
udps_db_policy_apply(u32 sw_if_index, u8 is_rx, u8 *name)
{
}

void
udps_db_policy_remove(u32 sw_if_index, u8 is_rx, u8 *name)
{
}

