--
-- create_config.sql
--

-- config_rdonly
CREATE TABLE config_rdonly
(
  owner varchar(64)  NOT NULL,  -- name of one who owns the entry
  key   varchar(64)  NOT NULL,
  value varchar(256) NOT NULL,

  UNIQUE(owner, key)
);


-- config
CREATE TABLE config
(
  owner varchar(64)  NOT NULL,  -- name of one who owns the entry
  key   varchar(64)  NOT NULL,
  value varchar(256) NOT NULL,

  UNIQUE(owner, key)
);
