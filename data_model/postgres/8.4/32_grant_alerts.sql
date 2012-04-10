--
-- grant_alerts.sql
--

-- ui/daemon may read everything
GRANT SELECT
  ON TABLE alerts,
           hosts,
           alert_sum,
           procs,
           analyzers,
           alert_analyzers,
           reference_urls,
           services
  TO "acarm-ng-ui",
     "acarm-ng-daemon";
GRANT SELECT
  ON SEQUENCE
           alerts_id_seq,
           hosts_id_seq,
           procs_id_seq,
           analyzers_id_seq,
           reference_urls_id_seq,
           services_id_seq
  TO "acarm-ng-ui",
     "acarm-ng-daemon";


-- daemon may add new data to DB
GRANT INSERT
  ON TABLE alerts,
           alert_sum,
           hosts,
           procs,
           analyzers,
           alert_analyzers,
           reference_urls,
           services
  TO "acarm-ng-daemon";
GRANT UPDATE
  ON SEQUENCE
           alerts_id_seq,
           hosts_id_seq,
           procs_id_seq,
           analyzers_id_seq,
           reference_urls_id_seq,
           services_id_seq
  TO "acarm-ng-daemon";


-- daemon may update pieces of information gathered by sensors
GRANT UPDATE(name)
  ON TABLE hosts
  TO "acarm-ng-daemon";


-- daemon may cleanup old data from DB
GRANT DELETE
  ON TABLE alerts,
           hosts,
           procs,
           alert_sum,
           analyzers,
           alert_analyzers,
           reference_urls,
           services
  TO "acarm-ng-daemon";
