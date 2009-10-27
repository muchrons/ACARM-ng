--
-- create_correlated_alerts.sql
--

-- meta alerts
CREATE SEQUENCE meta_alerts_id_seq;
CREATE TABLE    meta_alerts
(
  id               int          PRIMARY KEY
                                DEFAULT nextval('meta_alerts_id_seq'),
  name             varchar(128) NOT NULL,
  severity_delta   real         NOT NULL
                                DEFAULT 0,
  certanity_delta  real         NOT NULL
                                DEFAULT 0,
  id_ref           int          NULL
                                REFERENCES reference_urls(id)
                                DEFAULT NULL,
  creat_time       timestamp with time zone
                                NOT NULL,
  last_update_time timestamp with time zone
                                NOT NULL
                                DEFAULT now()
);

-- TODO: add table with list of still referenced meta-alerts


-- alert to meta-alert mapping table
CREATE TABLE alert_to_meta_alert_map
(
  id_alert      int NOT NULL
                REFERENCES alerts(id),
  id_meta_alert int NOT NULL
                REFERENCES meta_alerts(id),

  UNIQUE(id_alert, id_meta_alert)
);


-- meta alerts tree
CREATE TABLE meta_alerts_tree
(
  id_node  int NOT NULL
           REFERENCES meta_alerts(id),
  id_child int NULL
           REFERENCES meta_alerts(id),

  UNIQUE(id_node, id_child)
);



--
-- triggers and procedures to make entries in tables
-- alerts/meta_alerts/alert_to_meta_alert_map/meta_alert_tree properly alligned
--

-- get_parents_impl()
CREATE FUNCTION get_parents_impl(id int) RETURNS SETOF int AS
$$
DECLARE
  id_node_parent int;
BEGIN
  FOR id_node_parent IN SELECT id_node FROM meta_alerts_tree WHERE id_child=id
  LOOP
    RETURN NEXT id_node_parent;
    RETURN QUERY SELECT get_parents_impl(id_node_parent);
  END LOOP;
END;
$$ LANGUAGE plpgsql;

-- get_parents()
CREATE FUNCTION get_parents(id int) RETURNS SETOF int AS
$$
BEGIN
  -- empty set for NULLs
  IF id IS NULL THEN
    RETURN;
  END IF;
  RETURN QUERY SELECT DISTINCT( get_parents_impl(id) );
END;
$$ LANGUAGE plpgsql;


-- get_children_impl()
CREATE FUNCTION get_children_impl(id int) RETURNS SETOF int AS
$$
DECLARE
  id_node_child int;
BEGIN
  FOR id_node_child IN SELECT id_child FROM meta_alerts_tree WHERE id_node=id AND id_child IS NOT NULL
  LOOP
    RETURN NEXT id_node_child;
    RETURN QUERY SELECT get_children_impl(id_node_child);
  END LOOP;
END;
$$ LANGUAGE plpgsql;

-- get_children()
CREATE FUNCTION get_children(id int) RETURNS SETOF int AS
$$
BEGIN
  -- empty set for NULLs
  IF id IS NULL THEN
    RETURN;
  END IF;
  RETURN QUERY SELECT DISTINCT( get_children_impl(id) );
END;
$$ LANGUAGE plpgsql;


-- link_meta_alerts()
CREATE FUNCTION link_meta_alerts(id_from int, id_to int) RETURNS void AS
$$
BEGIN
  -- NULLs are not allowed
  IF id_from IS NULL OR id_to IS NULL THEN
    RAISE EXCEPTION 'parameters cannot be null';
  END IF;

  IF id_to IN ( SELECT get_parents(id_from) ) THEN
    RAISE EXCEPTION 'trying to link child (%) to parent (%)', id_from, id_to;
  END IF;

  INSERT INTO meta_alerts_tree VALUES (id_from, id_to);
END;
$$ LANGUAGE plpgsql;


-- ensure_leafs_are_unique()
CREATE FUNCTION ensure_leafs_are_unique() RETURNS TRIGGER AS
$$
BEGIN
  -- ensure record does not exist already and is not inserted as non-leaf
  PERFORM id_node FROM meta_alerts_tree WHERE
    id_node=NEW.id_node AND ( id_child IS NULL OR NEW.id_child IS NULL )
    LIMIT 1;
  IF FOUND THEN
    RAISE EXCEPTION 'entry node % already exists in meta_alets_tree (as a leaf or non-leaf)', NEW.id_node;
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;
-- register it as a trigger
CREATE TRIGGER ensure_leafs_are_unique_trigger
BEFORE INSERT ON meta_alerts_tree
  FOR EACH ROW EXECUTE PROCEDURE ensure_leafs_are_unique();


-- setup_environemnt_for_new_alert()
CREATE FUNCTION setup_environemnt_for_new_alert() RETURNS TRIGGER AS
$$
DECLARE
  tmp int;
BEGIN
  -- creates new meta-alert for any newly inserted alert
  INSERT INTO meta_alerts VALUES (DEFAULT, NEW.name, DEFAULT, DEFAULT, DEFAULT, NEW.create_time, DEFAULT) RETURNING id INTO tmp;
  -- adds proper mapping between alerts and meta-alerts
  INSERT INTO alert_to_meta_alert_map VALUES (NEW.id, tmp);
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;
-- register it as a trigger
CREATE TRIGGER setup_environemnt_for_new_alert_trigger
AFTER INSERT ON alerts
  FOR EACH ROW EXECUTE PROCEDURE setup_environemnt_for_new_alert();
