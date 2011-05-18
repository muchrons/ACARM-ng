--
-- create_logs.sql
--

CREATE SEQUENCE pradolog_id_seq;
-- Prado logger
CREATE TABLE    pradolog
(
  log_id        int PRIMARY KEY DEFAULT nextval('pradolog_id_seq'),
  level         int,
  category      varchar(128),
  logtime       varchar(20),
  message       varchar(255)
);
