#include "udps_includes.h"

udps_main_t udps_main;


bool udps_db_rule_policy_get(u8 *name, udps_policy_entry_t **pe);

void
udps_db_rule_action_add(u8 *name, u8 oper, u16 offset, u8 *value, u8 len, u32 out_port)
{
    udps_rewrite_t rw;
    udps_rule_action_t *ra;
    u32 act_id;
    bool ret;

    ret = udps_db_rule_action_get(name, &ra);
    if (false == ret) {
        pool_get(udps_main.action_db, ra);
        act_id = ra - udps_main.action_db;
        ra->name  = format(0, "%s%c", name, 0);
        ra->rewrite = NULL;
        ra->out_port = UDPS_NO_PORT;
    }
    rw.oper = oper;
    switch(oper) {
        case UDPS_REWRITE_INSERT:
        case UDPS_REWRITE_REPLACE:
            rw.offset = offset;
            rw.value = vec_dup(value);
            vec_add1(ra->rewrite, rw);
            break;
        case UDPS_REWRITE_REMOVE:
            rw.offset = offset;
            rw.len = len;
            vec_add1(ra->rewrite, rw);
            break;
    }
    if (UDPS_NO_PORT != out_port) {
        ra->out_port = out_port;
    }
    if (false == ret){
        hash_set_mem(udps_main.action_by_name, name, act_id);
    }
}

bool
udps_db_rule_action_del(u8 *name)
{
    return true;
}

bool
udps_db_rule_action_get(u8 *name, udps_rule_action_t **ra)
{
    uword *p;
    u32 act_id;
    
    p = hash_get_mem(udps_main.action_by_name, name);
    if (!p) {
        return false;
    }
    act_id = p[0];
    *ra = pool_elt_at_index(udps_main.action_db, act_id);
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
    udps_rule_entry_t ir = {UDPS_INVALID_RULE};
    udps_match_pkt_t mp;
    udps_rule_action_t *ra;
    udps_policy_entry_t *pe;
    u32 policy_id;
    u32 act_id;
    bool ret;

    if (aname) {
        ret = udps_db_rule_action_get(aname, &ra); 
        if (false == ret) {
            return false;
        }
        act_id = ra - udps_main.action_db;
    }

    ret = udps_db_rule_policy_get(name, &pe); 
    if (false == ret) {
        pool_get(udps_main.policy_db, pe);
        policy_id = pe - udps_main.policy_db;
        pe->name  = format(0, "%s%c", name, 0);
        pe->rules = NULL;
    }
    vec_validate_init_empty(pe->rules, id, ir);
    pe->rules[id].rule_id = id;
    switch(oper) {
        case UDPS_MATCH_PKT:
            mp.offset = offset;
            mp.value = vec_dup(value);
	    vec_add1(pe->rules[id].match_pkt, mp);
            break;
    }
    if (aname) {
        pe->rules[id].act_id = act_id;
    }
    if (false == ret){
        hash_set_mem(udps_main.policy_by_name, name, policy_id);
    }
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

bool
udps_db_rule_policy_get(u8 *name, udps_policy_entry_t **pe)
{
    uword *p;
    u32 policy_id;
    
    p = hash_get_mem(udps_main.policy_by_name, name);
    if (!p) {
        return false;
    }
    policy_id = p[0];
    *pe = pool_elt_at_index(udps_main.policy_db, policy_id);
    return true;
}


void
udps_db_policy_apply(u32 sw_if_index, u8 is_rx, u8 *name)
{
    uword *p;
    u32 policy_id;
    
    p = hash_get_mem(udps_main.policy_by_name, name);
    if (!p) {
        return;
    }
    policy_id = p[0];

    if (is_rx) {
        vec_validate_init_empty(udps_main.ing_policy_by_sw_if_index,
                          sw_if_index, UDPS_INVALID_POLICY_ID);
        udps_main.ing_policy_by_sw_if_index[sw_if_index] = policy_id;
    } else {
        vec_validate_init_empty(udps_main.egr_policy_by_sw_if_index,
                          sw_if_index, UDPS_INVALID_POLICY_ID);
        udps_main.egr_policy_by_sw_if_index[sw_if_index] = policy_id;
    }
}

void
udps_db_policy_remove(u32 sw_if_index, u8 is_rx)
{
    if (is_rx) {
        vec_validate_init_empty(udps_main.ing_policy_by_sw_if_index,
                          sw_if_index, UDPS_INVALID_POLICY_ID);
        udps_main.ing_policy_by_sw_if_index[sw_if_index] = UDPS_INVALID_POLICY_ID;
    } else {
        vec_validate_init_empty(udps_main.egr_policy_by_sw_if_index,
                          sw_if_index, UDPS_INVALID_POLICY_ID);
        udps_main.egr_policy_by_sw_if_index[sw_if_index] = UDPS_INVALID_POLICY_ID;
    }
}

static clib_error_t*
udps_db_init (vlib_main_t *vm)
{
    udps_main.action_by_name  = hash_create_string(0, sizeof(uword));
    udps_main.policy_by_name  = hash_create_string(0, sizeof(uword));
    return 0;
}

VLIB_INIT_FUNCTION (udps_db_init);
