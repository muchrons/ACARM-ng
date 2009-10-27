--
-- filter_states.sql
--

-- meta alerts is use by corelation system
CREATE TABLE meta_alerts_in_use
(
  id_meta_alert int NOT NULL
                    REFERENCES meta_alerts(id)
);
