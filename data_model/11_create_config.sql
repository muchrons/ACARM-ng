--
-- 11_create_config.sql
--

-- config_rdonly
CREATE TABLE config_rdonly
(
  key   varchar(64)  NOT NULL UNIQUE,
  value varchar(256) NOT NULL
);

-- config
CREATE TABLE config
(
  key   varchar(64)  NOT NULL UNIQUE,
  value varchar(256) NOT NULL
);

-- TODO
