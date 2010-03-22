--
-- clean up procedure draft
--

BEGIN TRANSACTION;

-- deadline
CREATE TEMP TABLE tmp
  ON COMMIT DROP
  AS
  ( SELECT id FROM alerts WHERE create_time<'2010-03-01' AND id NOT IN
    ( SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert IN
      ( SELECT id_meta_alert FROM meta_alerts_in_use )
    )
  );
-- helper id selection
CREATE TEMP TABLE tmp_rh
  ON COMMIT DROP
  AS
  ( SELECT id FROM reported_hosts WHERE id_alert IN (SELECT id FROM tmp) );

--
-- drop
--

DELETE FROM alert_analyzers   WHERE id_alert             IN (SELECT id FROM tmp);
DELETE FROM analyzers         WHERE id               NOT IN (SELECT id_analyzer FROM alert_analyzers);

DELETE FROM reported_services WHERE id_reported_host     IN (SELECT id FROM tmp_rh);
DELETE FROM services          WHERE id               NOT IN (SELECT id_service FROM reported_services);

DELETE FROM reported_procs    WHERE id_reported_host     IN (SELECT id FROM tmp_rh);
DELETE FROM procs             WHERE id               NOT IN (SELECT id_proc FROM reported_procs);

DELETE FROM reported_hosts    WHERE id                   IN (SELECT id FROM tmp_rh);

-- ending
select * from tmp;
select * from tmp_rh;
COMMIT;
