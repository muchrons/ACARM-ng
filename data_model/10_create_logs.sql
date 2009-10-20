--
-- create_logs.sql
--

-- logger_nodes
CREATE SEQUENCE logger_nodes_id_seq;
CREATE TABLE    logger_nodes
(
  id   int          PRIMARY KEY
                    DEFAULT nextval('logger_nodes_id_seq'),
  name varchar(128) NOT NULL UNIQUE
);

-- logger_logs
--CREATE SEQUENCE logger_logs_id_seq;
CREATE TABLE    logger_logs
(
--  id    int            PRIMARY KEY
--                       DEFAULT nextval('logger_logs_id_seq'),
  id_node int          REFERENCES logger_nodes(id),
  prio    char(5)      NOT NULL,
  ctime   timestamp    NOT NULL
                       DEFAULT now(),
  msg     varchar(256) NOT NULL

  CONSTRAINT prio_check CHECK (prio IN ('DEBUG',
                                        'INFO',
                                        'WARN',
                                        'ERROR',
                                        'FATAL'))
);
