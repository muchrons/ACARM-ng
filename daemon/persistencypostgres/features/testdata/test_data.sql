--
-- test_data.sql
--

--
-- clean up any old entries
--
DELETE FROM meta_alerts_in_use;
DELETE FROM alert_to_meta_alert_map;
DELETE FROM meta_alerts_tree;
DELETE FROM meta_alerts;
DELETE FROM reported_procs;
DELETE FROM reported_services;
DELETE FROM reported_hosts;
DELETE FROM alert_analyzers;
DELETE FROM alerts;
DELETE FROM analyzers;
DELETE FROM hosts;
DELETE FROM reference_urls;
DELETE FROM services;
DELETE FROM procs;
DELETE FROM config;
DELETE FROM config_rdonly;
DELETE FROM logger_logs;
DELETE FROM logger_nodes;


