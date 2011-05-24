BEGIN TRANSACTION;

--
-- cleanup of data base content
--
TRUNCATE config CASCADE;
TRUNCATE config_rdonly CASCADE;
TRUNCATE severities CASCADE;
TRUNCATE reference_urls CASCADE;
TRUNCATE meta_alerts CASCADE;
TRUNCATE meta_alerts_tree CASCADE;
TRUNCATE meta_alerts_already_triggered CASCADE;
TRUNCATE meta_alerts_in_use CASCADE;
TRUNCATE alerts CASCADE;
TRUNCATE alert_to_meta_alert_map CASCADE;
TRUNCATE analyzers CASCADE;
TRUNCATE alert_analyzers CASCADE;
TRUNCATE hosts CASCADE;
TRUNCATE services CASCADE;
TRUNCATE procs CASCADE;
TRUNCATE wui_users CASCADE;
TRUNCATE logs CASCADE;
TRUNCATE heartbeats CASCADE;

COMMIT;
