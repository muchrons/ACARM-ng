CREATE USER "acarm-ng-tests" PASSWORD 'test.password';
GRANT ALL ON DATABASE acarm_ng_test TO "acarm-ng-tests";
GRANT ALL ON TABLE
    config_rdonly,
    config,
    wui_users,
    logs,
    procs,
    services,
    reference_urls,
    hosts,
    analyzers,
    severities,
    alerts,
    alert_analyzers,
    reported_hosts,
    reported_services,
    reported_procs,
    meta_alerts,
    alert_to_meta_alert_map,
    meta_alerts_tree,
    meta_alerts_in_use,
    meta_alerts_already_triggered
    TO "acarm-ng-tests";

GRANT ALL ON SEQUENCE
    wui_users_id_seq,
    procs_id_seq,
    services_id_seq,
    reference_urls_id_seq,
    hosts_id_seq,
    analyzers_id_seq,
--    severities_id_seq,
    alerts_id_seq,
    reported_hosts_id_seq,
--    reported_services_id_seq,
    reported_procs_id_seq,
    meta_alerts_id_seq
    TO "acarm-ng-tests";
