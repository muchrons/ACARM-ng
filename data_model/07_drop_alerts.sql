--
-- drop_alerts.sql
--

DROP TABLE    reported_procs;
DROP SEQUENCE reported_procs_id_seq;

DROP TABLE    reported_services;
--DROP SEQUENCE reported_services_id_seq;

DROP TABLE    reported_hosts;
DROP SEQUENCE reported_hosts_id_seq;

DROP TABLE    alerts;
DROP SEQUENCE alerts_id_seq;

DROP TABLE    severities;
-- DROP SEQUENCE severities_id_seq;

DROP TABLE    analyzers;
DROP SEQUENCE analyzers_id_seq;

DROP TABLE    hosts;
DROP SEQUENCE hosts_id_seq;

DROP TABLE    reference_urls;
DROP SEQUENCE reference_urls_id_seq;

DROP TABLE    services;
DROP SEQUENCE services_id_seq;

DROP TABLE    procs;
DROP SEQUENCE procs_id_seq;
