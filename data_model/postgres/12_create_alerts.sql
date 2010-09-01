--
-- create_alerts.sql
--

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
  id          int          PRIMARY KEY DEFAULT nextval('alerts_id_seq'),
  name        varchar(256) NOT NULL,
  detect_time timestamp    NULL,
  create_time timestamp    NOT NULL DEFAULT now(),
  id_severity int          NOT NULL REFERENCES severities(id),
  -- TODO: typo in name
  certanity   real         NOT NULL DEFAULT 1.0,
  description text         NOT NULL,

  -- TODO: typo in name
  CONSTRAINT certanity_check      CHECK ( 0<=certanity AND certanity<=1 ),
  CONSTRAINT dates_relation_check CHECK ( detect_time IS NULL OR
                                          detect_time<=create_time )
);


-- reference urls
CREATE SEQUENCE reference_urls_id_seq;
CREATE TABLE    reference_urls
(
  id   int          PRIMARY KEY DEFAULT nextval('reference_urls_id_seq'),
  name varchar(128) NOT NULL,
  url  varchar(256) NOT NULL
);


-- hosts
CREATE SEQUENCE hosts_id_seq;
CREATE TABLE    hosts
(
  id       int      PRIMARY KEY DEFAULT nextval('hosts_id_seq'),
  id_alert int      REFERENCES alerts(id),
  id_ref   int      NULL REFERENCES reference_urls(id),
  role     char(3)  NOT NULL,
  ip       inet     NOT NULL,
  mask     inet     NULL,
  os       char(32) NULL,
  name     char(64) NULL        -- DNS host's name

  CONSTRAINT role_check CHECK (role IN ('src', 'dst'))
);


-- processes
CREATE SEQUENCE procs_id_seq;
CREATE TABLE    procs
(
  id        int          PRIMARY KEY DEFAULT nextval('procs_id_seq'),
  id_host   int          NOT NULL REFERENCES hosts(id),
  id_ref    int          NULL REFERENCES reference_urls(id),
  path      varchar(256) NULL,
  name      varchar(256) NOT NULL,
  md5       char(32)     NULL,
  pid       int          NULL,
  uid       int          NULL,
  username  varchar(32)  NULL,
  arguments text         NULL
);
CREATE INDEX procs_id_host_index ON procs(id_host);


-- services
CREATE SEQUENCE services_id_seq;
CREATE TABLE    services
(
  id       int      PRIMARY KEY DEFAULT nextval('services_id_seq'),
  id_host  int      NOT NULL REFERENCES hosts(id),
  id_ref   int      NULL REFERENCES reference_urls(id),
  name     char(32) NOT NULL,
  port     int      NOT NULL,
  protocol char(32) NULL,

  CONSTRAINT port_check CHECK ( 0<port AND port<65536 )
);


-- analyzers
CREATE SEQUENCE analyzers_id_seq;
CREATE TABLE    analyzers
(
  id      int          PRIMARY KEY DEFAULT nextval('analyzers_id_seq'),
  sys_id  int          NOT NULL UNIQUE,
  name    varchar(128) NOT NULL,
  version char(16)     NULL,
  os      varchar(128) NULL,
  ip      inet         NULL,

  UNIQUE(sys_id),
  UNIQUE(sys_id, name, version, os, ip)  -- this will make analyzers' scanning faster
);


-- alerts<->analyzers
CREATE TABLE alert_analyzers
(
  id_alert    int NOT NULL REFERENCES alerts(id),
  id_analyzer int NOT NULL REFERENCES analyzers(id),

  UNIQUE(id_alert, id_analyzer)
);
