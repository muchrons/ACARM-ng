--
-- create_alerts.sql
--

-- processes
CREATE SEQUENCE procs_id_seq;
CREATE TABLE    procs
(
  id   int          PRIMARY KEY
                    DEFAULT nextval('procs_id_seq'),
  path varchar(256) NULL,
  name varchar(256) NOT NULL,
  md5  char(32)     NULL
);


-- services
CREATE SEQUENCE services_id_seq;
CREATE TABLE    services
(
  id       int      PRIMARY KEY
                    DEFAULT nextval('services_id_seq'),
  name     char(32) NOT NULL,
  port     int      NOT NULL,
  protocol char(32) NULL,

  CONSTRAINT port_check CHECK ( 0<port AND port<65536 )
);


-- reference urls
CREATE SEQUENCE reference_urls_id_seq;
CREATE TABLE    reference_urls
(
  id   int          PRIMARY KEY
                    DEFAULT nextval('reference_urls_id_seq'),
  name varchar(128) NOT NULL,
  url  varchar(256) NOT NULL
);


-- hosts
CREATE SEQUENCE hosts_id_seq;
CREATE TABLE    hosts
(
  id   int      PRIMARY KEY
                DEFAULT nextval('hosts_id_seq'),
  ip   inet     NOT NULL,
  mask inet     NULL,
  os   char(32) NULL,
  name char(64) NULL        -- DNS host's name
);


-- analyzers
CREATE SEQUENCE analyzers_id_seq;
CREATE TABLE    analyzers
(
  id      int          PRIMARY KEY
                       DEFAULT nextval('analyzers_id_seq'),
  sys_id  bigint       NOT NULL,
  name    varchar(128) NOT NULL,
  version char(16)     NULL,
  os      varchar(128) NULL,
  ip      inet         NULL
);


-- severities
-- CREATE SEQUENCE severities_id_seq;
CREATE TABLE    severities
(
  id    int      PRIMARY KEY,
  level int      NOT NULL UNIQUE,
  name  char(16) NOT NULL UNIQUE,

  CONSTRAINT level_check CHECK ( 0<=level AND level<=6 )
);


-- alerts
CREATE SEQUENCE alerts_id_seq;
CREATE TABLE    alerts
(
  id          int          PRIMARY KEY
                           DEFAULT nextval('alerts_id_seq'),
  name        varchar(256) NOT NULL,
  detect_time timestamp    NULL,
  create_time timestamp    NOT NULL
                           DEFAULT now(),
  id_severity int          NOT NULL
                           REFERENCES severities(id),
  -- TODO: typo in name
  certanity   real         NOT NULL
                           DEFAULT 1.0,
  description text         NOT NULL,

  -- TODO: typo in name
  CONSTRAINT certanity_check      CHECK ( 0<=certanity AND certanity<=1 ),
  CONSTRAINT dates_relation_check CHECK ( detect_time IS NULL OR
                                          detect_time<=create_time )
);


-- alerts<->analyzers
CREATE TABLE alert_analyzers
(
  id_alert    int NOT NULL
                  REFERENCES alerts(id),
  id_analyzer int NOT NULL
                  REFERENCES analyzers(id),

  UNIQUE(id_alert, id_analyzer)
);


-- reported hosts
CREATE SEQUENCE reported_hosts_id_seq;
CREATE TABLE    reported_hosts
(
  id       int     PRIMARY KEY
                   DEFAULT nextval('reported_hosts_id_seq'),
  id_alert int     NOT NULL
                   REFERENCES alerts(id),
  id_host  int     NOT NULL
                   REFERENCES hosts(id),
  role     char(3) NOT NULL,
  id_ref   int     NULL
                   REFERENCES reference_urls(id),

  CONSTRAINT role_check CHECK (role IN ('src', 'dst'))
);


-- reported services
--CREATE SEQUENCE reported_services_id_seq;
CREATE TABLE    reported_services
(
  id_reported_host int NOT NULL
                       REFERENCES reported_hosts(id),
  id_service       int NOT NULL
                       REFERENCES services(id),
  id_ref           int NULL
                       REFERENCES reference_urls(id),

  UNIQUE(id_reported_host, id_service)
);


-- reported processes
CREATE SEQUENCE reported_procs_id_seq;
CREATE TABLE    reported_procs
(
  id               int         PRIMARY KEY
                               DEFAULT nextval('reported_procs_id_seq'),
  id_reported_host int         NOT NULL
                               REFERENCES reported_hosts(id),
  id_proc          int         NOT NULL
                               REFERENCES procs(id),
  pid              int         NULL,
  uid              int         NULL,
  username         varchar(32) NULL,
  arguments        text        NULL,
  id_ref           int         NULL
                               REFERENCES reference_urls(id)
);
