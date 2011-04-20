--
-- grant_filter_states.sql
--

-- ui/daemon may do everything
GRANT SELECT,
      INSERT,
      DELETE
  ON TABLE heartbeats
  TO "acarm-ng-ui",
     "acarm-ng-daemon";
