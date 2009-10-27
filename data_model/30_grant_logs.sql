--
-- grant_logs.sql
--

-- ui/daemon may add new logs and cleanup old ones
GRANT INSERT,
      DELETE
  ON TABLE logger_logs
  TO "acarm-ng-ui",
     "acarm-ng-daemon";


-- ui may show logs
GRANT SELECT
  ON TABLE logger_logs
  TO "acarm-ng-ui";
