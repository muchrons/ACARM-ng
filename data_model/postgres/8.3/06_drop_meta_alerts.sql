--
-- drop_correlated_alerts.sql
--

DROP TABLE    alert_to_meta_alert_map;

DROP TRIGGER  ensure_leafs_are_unique_trigger ON meta_alerts_tree;
DROP TABLE    meta_alerts_tree;

DROP FUNCTION ensure_leafs_are_unique();

DROP FUNCTION link_meta_alerts(bigint, bigint);

DROP FUNCTION get_parents(bigint);
DROP FUNCTION get_parents_impl(bigint);

DROP FUNCTION get_children(bigint);
DROP FUNCTION get_children_impl(bigint);

DROP TABLE    meta_alerts;
DROP SEQUENCE meta_alerts_id_seq;

