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
-- helper id selection
CREATE TEMP TABLE tmp_ma
  ON COMMIT DROP
  AS
  ( SELECT id_meta_alert FROM alert_to_meta_alert_map WHERE id_alert IN (SELECT id FROM tmp) );

--
-- drop
--

DELETE FROM reported_services WHERE id_reported_host     IN (SELECT id FROM tmp_rh);
DELETE FROM services          WHERE id               NOT IN (SELECT id_service FROM reported_services);

DELETE FROM reported_procs    WHERE id_reported_host     IN (SELECT id FROM tmp_rh);
DELETE FROM procs             WHERE id               NOT IN (SELECT id_proc FROM reported_procs);

DELETE FROM reported_hosts    WHERE id                   IN (SELECT id FROM tmp_rh);
DELETE FROM hosts             WHERE id               NOT IN (SELECT id_host FROM reported_hosts);

DELETE FROM alert_analyzers   WHERE id_alert             IN (SELECT id FROM tmp);
DELETE FROM analyzers         WHERE id               NOT IN (SELECT id_analyzer FROM alert_analyzers);

DELETE FROM alert_to_meta_alert_map WHERE id_alert       IN (SELECT id FROM tmp);
DELETE FROM alerts                  WHERE id             IN (SELECT id FROM tmp);

-- this is a bit tricky - removing all entries stright away would require do-while iteration
-- and so to make things simple removing is done in one or more steps - when next pruning
-- will be called, it will remove next level in tree, until all unused entries are removed.
-- this causes some dead entries to be present for some time, though they will not
-- interfear with existing entries, since they are already marked as unused anyway.
DELETE FROM meta_alerts_tree        WHERE id_child       IN (SELECT id_meta_alert FROM tmp_ma);
DELETE FROM meta_alerts             WHERE id             IN (SELECT id_meta_alert FROM tmp_ma);
DELETE FROM meta_alerts             WHERE id         NOT IN (SELECT id_node  FROM meta_alerts_tree) AND
                                          id         NOT IN (SELECT id_child FROM meta_alerts_tree);

-- ending
COMMIT;
