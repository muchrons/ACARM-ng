--
-- processor_states.sql
--

-- meta alerts is use by corelation system
CREATE TABLE meta_alerts_in_use
(
  id_meta_alert bigint NOT NULL UNIQUE REFERENCES meta_alerts(id)
);

-- meta alerts already reported by triggers
CREATE TABLE meta_alerts_already_triggered
(
  id_meta_alert bigint NOT NULL REFERENCES meta_alerts(id),
  trigger_name  varchar(128) NOT NULL,

  UNIQUE(id_meta_alert, trigger_name)
);
