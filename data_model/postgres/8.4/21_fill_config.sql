--
-- fill_config.sql
--

INSERT INTO config_rdonly VALUES
  ('acarm-ng', 'version',     'v0.2.1'),
-- acarm version to be removed
  ('acarm-ng', 'db-version',  '5'),
  ('acarm-ng', 'create_time', now() )
;
