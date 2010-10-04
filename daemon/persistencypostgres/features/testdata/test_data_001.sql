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
DELETE FROM alert_analyzers;
DELETE FROM analyzers;
DELETE FROM services;
DELETE FROM procs;
DELETE FROM hosts;
DELETE FROM reference_urls;
DELETE FROM alerts;
DELETE FROM severities;
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
SELECT setval('analyzers_id_seq', 5);
INSERT INTO analyzers VALUES
  (1, 11, 'analyzer 1', '1.2.3', 'Linux', '127.0.0.1'),
  (2, 12, 'analyzer 2', '2.2.3', 'Linux', '127.0.0.3'),
  (3, 10, 'analyzer 3', '1.3.3', 'Linux', '127.0.0.2'),
  (4, 44, 'analyzer 4', '1.2.3', 'Linux', '127.0.0.1')
;
--SELECT setval('severities_id_seq', 10);
INSERT INTO severities VALUES
  (7, 0, 'level 1'),
  (8, 1, 'level 2'),
  (9, 2, 'level 3'),
  (2, 3, 'level 4'),
  (6, 4, 'level 5'),
  (1, 5, 'level 6'),
  (5, 6, 'level 7')
;
SELECT setval('alerts_id_seq', 5);
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
SELECT setval('reference_urls_id_seq', 4);
INSERT INTO reference_urls VALUES
  (1, 'some reference',  'http://abc.def.org'),
  (2, 'other reference', 'https://xyz.org'),
  (3, 'misc link',       'http://alice.has.a.cat')
;
SELECT setval('hosts_id_seq', 7);
INSERT INTO hosts VALUES
-- id, id_alert, id_ref, role, ip, mask, os, name
  (1, 1, 3,    'dst', '127.0.0.1', '255.255.255.0', 'Linux', 'localhost'),
  (2, 2, NULL, 'dst', '127.0.0.1', '255.255.255.0', 'Linux', 'localhost'),
  (3, 2, NULL, 'src', '127.0.0.2', '255.255.255.0', 'Linux', NULL),
  (4, 3, NULL, 'dst', '127.0.0.3', NULL,            NULL,    NULL),
  (5, 3, NULL, 'dst', '127.0.0.1', '255.255.255.0', 'Linux', 'localhost'),
  (6, 4, NULL, 'src', '127.0.0.2', '255.255.255.0', 'Linux', NULL)
;

SELECT setval('services_id_seq', 3);
INSERT INTO services VALUES
-- id, id_host, id_ref, name, port, protocol
  (1, 4, 1,    'sendmail', 25, 'smtp'),
  (2, 5, NULL, 'apache',   80, 'www')
;
SELECT setval('procs_id_seq', 5);
INSERT INTO procs VALUES
-- id, id_host, id_ref, path, name, md5, pid, uid, username, args
  (1, 6, 2,    NULL,          'doom.exe',   NULL,                               42,   666, 'kain',   '-h -e -l'),
  (2, 6, 2,    NULL,          'doom.exe',   NULL,                               43,   666, 'kain',   '-h -e -l'),
  (3, 2, NULL, '/some/path',  'binaryname', '01234567890123456789012345678912', 13,   997, 'norman', NULL),
  (4, 1, NULL, '/some/path',  'binaryname', '01234567890123456789012345678912', NULL, NULL, NULL,    NULL)
;
SELECT setval('meta_alerts_id_seq', 8);
INSERT INTO meta_alerts VALUES
  (1, 27, 3,    'meta/alert number 1', 0.5,  0.5, '1990-01-03 11:22:33', '1990-02-01 10:00:00'),
  (2, 14, NULL, 'meta/alert number 2', 1.5, -0.5, '2010-01-03 12:22:33', '2010-02-01 11:10:00'),
  (3, 51, NULL, 'meta number 1',       0.1, -0.1, '2010-02-03 12:22:33', '2010-02-03 13:10:00'),
  (4, 62, NULL, 'meta/alert number 3', 0.1, -0.1, '2010-02-03 12:22:33', '2010-02-03 13:10:00'),
  (5, 48, NULL, 'meta/alert number 4', 0.1, -0.1, '2010-02-03 12:22:33', '2010-02-03 13:10:00'),
  (6, 93, NULL, 'meta number 2',       0.1, -0.1, '2010-02-03 12:22:33', '2010-02-03 13:10:00'),
  (7, 18, NULL, 'meta number 3',       0.1, -0.1, '2010-02-03 12:22:33', '2010-02-03 13:10:00')
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
