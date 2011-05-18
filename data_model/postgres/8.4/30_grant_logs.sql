--
-- grant_logs.sql
--

-- ui/daemon may add new logs and cleanup old ones
GRANT INSERT,
      SELECT,
      DELETE
  ON TABLE pradolog
  TO "acarm-ng-ui",
     "acarm-ng-daemon";

-- allow changing sequence IDs
GRANT UPDATE,
      SELECT
  ON SEQUENCE pradolog_id_seq
  TO "acarm-ng-ui";

