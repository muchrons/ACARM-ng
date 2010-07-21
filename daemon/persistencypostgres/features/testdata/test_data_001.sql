--
-- test_data.sql
--

--
-- clean up any old entries
--
DELETE FROM meta_alerts_already_triggered;
DELETE FROM meta_alerts_in_use;
DELETE FROM alert_to_meta_alert_map;
DELETE FROM meta_alerts_tree;
DELETE FROM meta_alerts;
DELETE FROM reported_procs;
DELETE FROM reported_services;
DELETE FROM reported_hosts;
DELETE FROM alert_analyzers;
DELETE FROM alerts;
DELETE FROM severities;
DELETE FROM analyzers;
DELETE FROM hosts;
DELETE FROM reference_urls;
DELETE FROM services;
DELETE FROM procs;
DELETE FROM config;
DELETE FROM config_rdonly;
DELETE FROM logs;
DELETE FROM wui_users;


-- config
INSERT INTO config_rdonly VALUES
  ('owner1', 'some key',  'some value'),
  ('owner1', 'other key', 'xyz'),
  ('owner2', 'other key', 'secret value')
;
INSERT INTO config VALUES
  ('owner1', 'Xsome key',  'Qsome value'),
  ('owner1', 'Xother key', 'Qxyz'),
  ('owner2', 'Xother key', 'Qsecret value')
;

-- alerts and meta-alerts
INSERT INTO procs VALUES
  (1, '/some/path',  'binaryname', '01234567890123456789012345678912'),
  (2, '/other/path', 'appname',    '012345678901234567890123456789ab'),
  (3, NULL,          'doom.exe',   NULL)
;
INSERT INTO services VALUES
  (1, 'sendmail',     25, 'smtp' ),
  (2, 'apache',       80, 'www'  ),
  (3, 'apache',      443, 'https'),
  (4, 'doom server', 443, NULL)
;
INSERT INTO reference_urls VALUES
  (1, 'some reference',  'http://abc.def.org'),
  (2, 'other reference', 'https://xyz.org'),
  (3, 'misc link',       'http://alice.has.a.cat')
;
INSERT INTO hosts VALUES
  (1, '127.0.0.1', '255.255.255.0', 'Linux', 'localhost'),
  (2, '127.0.0.2', '255.255.255.0', 'Linux', NULL),
  (3, '127.0.0.3', NULL,            NULL,    NULL)
;
INSERT INTO analyzers VALUES
  (1, 11, 'analyzer 1', '1.2.3', 'Linux', '127.0.0.1'),
  (2, 12, 'analyzer 2', '2.2.3', 'Linux', '127.0.0.3'),
  (3, 10, 'analyzer 3', '1.3.3', 'Linux', '127.0.0.2'),
  (4, 44, 'analyzer 4', '1.2.3', 'Linux', '127.0.0.1')
;
INSERT INTO severities VALUES
  (7, 0, 'level 1'),
  (8, 1, 'level 2'),
  (9, 2, 'level 3'),
  (2, 3, 'level 4'),
  (6, 4, 'level 5'),
  (1, 5, 'level 6'),
  (5, 6, 'level 7')
;
INSERT INTO alerts VALUES
  (1, 'some alert 1', '1990-01-02 12:13:14', '1990-01-02 13:00:00', 5, 0.42, 'some description a'),
  (2, 'some alert 2', NULL,                  '2010-01-06 13:01:00', 6, 0.74, 'some description b'),
  (3, 'some alert 3', '2010-01-04 11:16:10', '2010-01-04 13:02:00', 2, 1.00, 'some description c'),
  (4, 'some alert 4', '2010-01-05 11:16:10', '2010-01-05 13:03:00', 9, 0.01, 'some description d')
;
INSERT INTO alert_analyzers VALUES
  (1, 3),
  (2, 3),
  (3, 2),
  (3, 1),
  (4, 1)
;
INSERT INTO reported_hosts VALUES
  (1, 1, 1, 'dst', 3),
  (2, 2, 1, 'dst', 1),
  (3, 2, 2, 'src', NULL),
  (4, 3, 3, 'dst', NULL),
  (5, 3, 1, 'dst', NULL),
  (6, 4, 2, 'src', NULL)
;
INSERT INTO reported_services VALUES
  (4, 1, 1),
  (5, 2, NULL)
;
INSERT INTO reported_procs VALUES
  (1, 6, 3, 42,   666, 'kain',   '-h -e -l', 2),
  (2, 6, 3, 43,   666, 'kain',   '-h -e -l', 2),
  (3, 2, 1, 13,   997, 'norman', NULL,       NULL),
  (4, 1, 1, NULL, NULL, NULL,    NULL,       NULL)
;
INSERT INTO meta_alerts VALUES
  (1, 'meta/alert number 1', 0.5,  0.5,    3, '1990-01-03 11:22:33', '1990-02-01 10:00:00'),
  (2, 'meta/alert number 2', 1.5, -0.5, NULL, '2010-01-03 12:22:33', '2010-02-01 11:10:00'),
  (3, 'meta number 1',       0.1, -0.1, NULL, '2010-02-03 12:22:33', '2010-02-03 13:10:00'),
  (4, 'meta/alert number 3', 0.1, -0.1, NULL, '2010-02-03 12:22:33', '2010-02-03 13:10:00'),
  (5, 'meta/alert number 4', 0.1, -0.1, NULL, '2010-02-03 12:22:33', '2010-02-03 13:10:00'),
  (6, 'meta number 2',       0.1, -0.1, NULL, '2010-02-03 12:22:33', '2010-02-03 13:10:00'),
  (7, 'meta number 3',       0.1, -0.1, NULL, '2010-02-03 12:22:33', '2010-02-03 13:10:00')
;
INSERT INTO alert_to_meta_alert_map VALUES
  (1, 2),
  (2, 4),
  (3, 1),
  (4, 5)
;
INSERT INTO meta_alerts_tree VALUES
  (3, 1),
  (3, 2),
  (6, 4),
  (6, 2),
  (6, 5),
  (7, 3),
  (7, 6)
;
INSERT INTO meta_alerts_in_use VALUES
  (1),
  (2),
  (3)
;
INSERT INTO meta_alerts_already_triggered VALUES
  (2, 'trigger1'),
  (3, 'trigger2')
;
