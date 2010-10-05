--
-- create_config.sql
--

-- TODO: consider making owner non-NULL as well...

-- config_rdonly
CREATE TABLE config_rdonly
(
  owner varchar(64)  NULL,      -- name of one who owns the entry
  key   varchar(64)  NOT NULL,
  value varchar(256) NOT NULL,

  UNIQUE(owner, key)
);


-- config
CREATE TABLE config
(
  owner varchar(64)  NULL,      -- name of one who owns the entry
  key   varchar(64)  NOT NULL,
  value varchar(256) NOT NULL,

  UNIQUE(owner, key)
);
