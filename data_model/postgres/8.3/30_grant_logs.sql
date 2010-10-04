--
-- grant_logs.sql
--

-- ui/daemon may add new logs and cleanup old ones
GRANT INSERT,
      DELETE
  ON TABLE logs
  TO "acarm-ng-ui",
     "acarm-ng-daemon";

-- allow changing sequence IDs
GRANT UPDATE,
      SELECT
  ON SEQUENCE wui_users_id_seq
  TO "acarm-ng-ui";

-- ui may show logs
GRANT SELECT
  ON TABLE logs
  TO "acarm-ng-ui";
