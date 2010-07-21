--
-- create_logs.sql
--

-- create users
CREATE SEQUENCE wui_users_id_seq;
CREATE TABLE    wui_users
(
  id int           PRIMARY KEY
                   DEFAULT nextval('wui_users_id_seq'),
  login   char(32) NOT NULL,
  pwdhash char(32) NOT NULL
);

-- logger_logs
CREATE TABLE    logs
(
  id_wui_user int          NULL
                           REFERENCES wui_users(id),
  ctime       timestamp    NOT NULL
                           DEFAULT now(),
  msg         varchar(128) NOT NULL
);
