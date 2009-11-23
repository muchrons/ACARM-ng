--
-- drop_correlated_alerts.sql
--

-- TODO: drop meta-alerts

DROP TABLE    alert_to_meta_alert_map;

DROP TRIGGER  ensure_leafs_are_unique_trigger ON meta_alerts_tree;
DROP TABLE    meta_alerts_tree;

DROP FUNCTION ensure_leafs_are_unique();

DROP FUNCTION link_meta_alerts(int, int);

DROP FUNCTION get_parents(int);
DROP FUNCTION get_parents_impl(int);

DROP FUNCTION get_children(int);
DROP FUNCTION get_children_impl(int);

DROP TABLE    meta_alerts;
DROP SEQUENCE meta_alerts_id_seq;

