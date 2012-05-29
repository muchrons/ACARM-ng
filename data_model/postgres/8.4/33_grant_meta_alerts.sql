--
-- grant_meta_alerts.sql
--

-- ui/daemon may read everything
GRANT SELECT
  ON TABLE meta_alerts,
           meta_alert_sum,
           meta_alerts_tree,
           meta_alerts_roots,
           alert_to_meta_alert_map
  TO "acarm-ng-ui",
     "acarm-ng-daemon";
GRANT SELECT
  ON SEQUENCE meta_alerts_id_seq
  TO "acarm-ng-ui",
     "acarm-ng-daemon";


-- daemon may add new data to DB
GRANT INSERT
  ON TABLE meta_alerts,
           meta_alert_sum,
           meta_alerts_tree,
           meta_alerts_roots,
           alert_to_meta_alert_map
  TO "acarm-ng-daemon";
GRANT UPDATE
  ON TABLE meta_alerts_id_seq
  TO "acarm-ng-daemon";


-- daemon may update selected columns
GRANT UPDATE(last_update_time,
             severity_delta,
             certainty_delta)
  ON TABLE meta_alerts
  TO "acarm-ng-daemon";


-- daemon may cleanup old data from DB
GRANT DELETE
  ON TABLE meta_alerts,
           meta_alert_sum,
           meta_alerts_tree,
           meta_alerts_roots,
           alert_to_meta_alert_map
  TO "acarm-ng-daemon";
