CREATE USER "acarm-ng-tests" PASSWORD 'test.password';

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
    alerts,
    alert_analyzers,
    meta_alerts,
    alert_to_meta_alert_map,
    meta_alerts_tree,
    meta_alerts_roots,
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
    alerts_id_seq,
    meta_alerts_id_seq
    TO "acarm-ng-tests";

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
    alerts,
    alert_analyzers,
    meta_alerts,
    alert_to_meta_alert_map,
    meta_alerts_tree,
    meta_alerts_roots,
    meta_alerts_in_use,
    meta_alerts_already_triggered
    TO _USER_;

GRANT ALL ON SEQUENCE
    wui_users_id_seq,
    procs_id_seq,
    services_id_seq,
    reference_urls_id_seq,
    hosts_id_seq,
    analyzers_id_seq,
    alerts_id_seq,
    meta_alerts_id_seq
    TO _USER_;
