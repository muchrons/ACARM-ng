--
-- grant_logs.sql
--

-- ui/daemon may add new logs and cleanup old ones
GRANT INSERT,
      DELETE
  ON TABLE logger_logs
  TO "acarm-ng-ui",
     "acarm-ng-daemon";

-- allow changing sequence IDs
--GRANT UPDATE,
--      SELECT
--  ON SEQUENCE logger_logs_id_seq
--  TO "acarm-ng-ui",
--     "acarm-ng-daemon";


-- ui may show logs
GRANT SELECT
  ON TABLE logger_logs
  TO "acarm-ng-ui";
