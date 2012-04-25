--
-- create_alerts.sql
--

-- alerts
CREATE SEQUENCE alerts_id_seq;
CREATE TABLE    alerts
(
  id          bigint       PRIMARY KEY DEFAULT nextval('alerts_id_seq'),
  name        varchar(256) NOT NULL,
  detect_time timestamp    NULL,
  create_time timestamp    NOT NULL DEFAULT now(),
  severity    int          NOT NULL,
  certainty   real         NOT NULL DEFAULT 1.0,
  description text         NOT NULL,

  CONSTRAINT certainty_check      CHECK ( 0<=certainty AND certainty<=1 ),
  CONSTRAINT severity_check       CHECK ( 0<=severity AND severity<=4 ),
  CONSTRAINT dates_relation_check CHECK ( detect_time IS NULL OR detect_time<=create_time )
);
CREATE INDEX alerts_create_time_index ON alerts(create_time);

-- alerts preprocessed
CREATE TABLE    alert_sum
(
  create_time timestamp PRIMARY KEY,
  high    bigint,
  medium  bigint,
  low     bigint,
  info    bigint,
  debug   bigint
);
CREATE INDEX alert_sum_create_time_index ON alert_sum(create_time);

-- reference urls
CREATE SEQUENCE reference_urls_id_seq;
CREATE TABLE    reference_urls
(
  id   bigint       PRIMARY KEY DEFAULT nextval('reference_urls_id_seq'),
  name varchar(128) NOT NULL,
  url  varchar(256) NOT NULL
);


-- hosts
CREATE SEQUENCE hosts_id_seq;
CREATE TABLE    hosts
(
  id       bigint   PRIMARY KEY DEFAULT nextval('hosts_id_seq'),
  id_alert bigint   NOT NULL REFERENCES alerts(id),
  id_ref   bigint   NULL REFERENCES reference_urls(id),
  role     char(3)  NOT NULL,
  ip       inet     NOT NULL,
  mask     inet     NULL,
  os       char(32) NULL,
  name     char(64) NULL        -- DNS host's name

  CONSTRAINT role_check CHECK (role IN ('src', 'dst'))
);
CREATE INDEX hosts_id_alert_index ON hosts(id_alert);


-- processes
CREATE SEQUENCE procs_id_seq;
CREATE TABLE    procs
(
  id        bigint       PRIMARY KEY DEFAULT nextval('procs_id_seq'),
  id_host   bigint       NOT NULL REFERENCES hosts(id),
  id_ref    bigint       NULL REFERENCES reference_urls(id),
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
  id       bigint   PRIMARY KEY DEFAULT nextval('services_id_seq'),
  id_host  bigint   NOT NULL REFERENCES hosts(id),
  id_ref   bigint   NULL REFERENCES reference_urls(id),
  name     char(32) NOT NULL,
  port     int      NOT NULL,
  protocol char(32) NULL,

  CONSTRAINT port_check CHECK ( 0<port AND port<65536 )
);
CREATE INDEX services_id_host_index ON services(id_host);


-- analyzers
CREATE SEQUENCE analyzers_id_seq;
CREATE TABLE    analyzers
(
  id      bigint       PRIMARY KEY DEFAULT nextval('analyzers_id_seq'),
  sys_id  bigint       NOT NULL UNIQUE,
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
  id_alert    bigint NOT NULL REFERENCES alerts(id),
  id_analyzer bigint NOT NULL REFERENCES analyzers(id),

  UNIQUE(id_alert, id_analyzer)
);
CREATE INDEX alert_analyzers_id_alert_index ON alert_analyzers(id_alert);
