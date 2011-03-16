--
-- fill_config.sql
--

INSERT INTO config_rdonly VALUES
  ('acarm-ng', 'version',     'v0.0.0'),
  ('acarm-ng', 'db-version',  '3'),
  ('acarm-ng', 'create_time', now() )
;
