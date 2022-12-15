#ifndef __UDPS_H__
#define __UDPS_H__

#define UDPS_NO_PORT   (~0)
#define UDPS_NO_ACTION (NULL)

enum udps_rewrite_oper_e {
  UDPS_REWRITE_UNUSED = 0,
  UDPS_REWRITE_INSERT,
  UDPS_REWRITE_REPLACE,
  UDPS_REWRITE_REMOVE
};

enum udps_match_oper_e {
  UDPS_MATCH_UNUSED = 0,
  UDPS_MATCH_PKT,
};

typedef struct udps_rewrite_s {
    u8  oper; 
    u16 offset;
    u8 *value;
    u8 len;
} udps_rewrite_t;

typedef struct udps_rule_action_s {
    u8 *name;
    udps_rewrite_t *rewrite;
    u32 out_port;
} udps_rule_action_t;

typedef struct udps_match_pkt_s {
    u16 offset;
    u8 *value;
} udps_match_pkt_t;

typedef struct udps_rule_entry_s {
    u8 rule_id;
    udps_match_pkt_t *match_pkt;
    u32 act_id;
} udps_rule_entry_t;

typedef struct udps_policy_entry_s {
    u8 *name;
    udps_rule_entry_t *rules;
} udps_policy_entry_t;

typedef struct udps_main_s {
    u32 *ing_policy_by_sw_if_index;
    u32 *egr_policy_by_sw_if_index; 
    udps_policy_entry_t *policy_db;
    udps_rule_action_t *action_db;
    uword *action_by_name;
    uword *policy_by_name;
} udps_main_t;

/*  Add a rule action.
    Creates action if not present.
    If already present updates the existing one.
    name, oper are mandatory.
    oper determines which of offset, value, len are set.
*/
void udps_db_rule_action_add(u8 *name, u8 oper, u16 offset, u8 *value, u8 len, u32 out_port);

/*  Delete a rule action.
    Deletes action if not used by a policy.
    If used, returns false.
*/
bool udps_db_rule_action_del(u8 *name);

/*  Get a rule action.
    If present, rule action is returned in ra.
    Otherwise, returns false.
*/
bool udps_db_rule_action_get(u8 *name, udps_rule_action_t **ra);

/*  Get a matching rule action for the packet.
    If present, rule action is returned in ra.
    Otherwise, returns false.
*/
bool udps_db_rule_match(u32 sw_if_index, u8 is_rx, u8 *bytes, udps_rule_action_t **ra);

/*  Add a rule entry.
    Creates rule entry if not present.
    If already present updates the existing one.
    name, id, oper are mandatory.
    oper determines if offset, value is valid.
    If aname is not created yet, returns false.
*/
bool udps_db_rule_entry_add(u8 *name, u8 id, u8 oper, u16 offset, u8 *value, u8 *aname);

/*  Get the rule entry count.
*/
u8 udps_db_rule_entry_cnt(u8 *name);

/*  Get a rule entry.
    If present, rule entry is returned in re.
    Otherwise, returns false.
*/
bool udps_db_rule_entry_get(u8 *name, u8 id, udps_rule_entry_t **re);

/*  Delete a policy.
    Deletes policy if not used by an interface.
    If used, returns false.
*/
bool udps_db_rule_policy_del(u8 *name);

/*  Applies policy on interface in the given direction.
*/
void udps_db_policy_apply(u32 sw_if_index, u8 is_rx, u8 *name);

/*  Removes policy on interface in the given direction.
*/
void udps_db_policy_remove(u32 sw_if_index, u8 is_rx, u8 *name);

/*  Applies policy on interface in the given direction.
*/
void udps_node_policy_apply(u32 sw_if_index, u8 is_rx);

/*  Removes policy on interface in the given direction.
*/
void udps_node_policy_remove(u32 sw_if_index, u8 is_rx);

#endif /* __UDPS_H__ */
