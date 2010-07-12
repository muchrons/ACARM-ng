--
-- grant_filter_states.sql
--

-- ui/daemon may read everything
GRANT SELECT
  ON TABLE meta_alerts_in_use,
           meta_alerts_already_triggered
  TO "acarm-ng-ui",
     "acarm-ng-daemon";


-- daemon may add and delete data to set
GRANT INSERT,
      DELETE
  ON TABLE meta_alerts_in_use,
           meta_alerts_already_triggered
  TO "acarm-ng-daemon";
