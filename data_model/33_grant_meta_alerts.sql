--
-- grant_meta_alerts.sql
--

-- ui/daemon may read everything
GRANT SELECT
  ON TABLE meta_alerts,
           meta_alerts_tree,
           alert_to_meta_alert_map
  TO "acarm-ng-ui",
     "acarm-ng-daemon";

-- daemon may add new data to DB
GRANT INSERT
  ON TABLE meta_alerts,
           meta_alerts_tree,
           alert_to_meta_alert_map
  TO "acarm-ng-daemon";

-- daemon may update selected columns
GRANT UPDATE(last_update_time)
  ON TABLE meta_alerts
  TO "acarm-ng-daemon";

-- daemon may cleanup old data from DB
GRANT DELETE
  ON TABLE meta_alerts,
           meta_alerts_tree,
           alert_to_meta_alert_map
  TO "acarm-ng-daemon";
