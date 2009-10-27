--
-- grant_config.sql
--

-- ui/daemon may change config at will
GRANT INSERT,
      DELETE,
      UPDATE,
      SELECT
  ON TABLE config
  TO "acarm-ng-ui",
     "acarm-ng-daemon";


-- ui/daemon may read constant part of config
GRANT SELECT
  ON TABLE config_rdonly
  TO "acarm-ng-ui",
     "acarm-ng-daemon";
