--
-- create_heartbeats.sql
--

-- meta alerts is use by corelation system
CREATE TABLE heartbeats
(
  owner     varchar(64)  NOT NULL,
  module    varchar(128) NOT NULL,
  timestamp timestamp    NOT NULL,
  timeout   int          NOT NULL,

  CONSTRAINT positive_timeout CHECK (timeout>0)
);
