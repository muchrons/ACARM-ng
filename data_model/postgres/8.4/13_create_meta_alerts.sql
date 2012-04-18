--
-- create_correlated_alerts.sql
--

-- meta alerts
CREATE SEQUENCE meta_alerts_id_seq;
CREATE TABLE    meta_alerts
(
  id               bigint       PRIMARY KEY DEFAULT nextval('meta_alerts_id_seq'),
  sys_id           bigint       NOT NULL UNIQUE,
  id_ref           bigint       NULL REFERENCES reference_urls(id) DEFAULT NULL,
  name             varchar(256) NOT NULL,
  severity_delta   real         NOT NULL DEFAULT 0,
  certainty_delta  real         NOT NULL DEFAULT 0,
  create_time      timestamp    NOT NULL,
  last_update_time timestamp    NOT NULL DEFAULT now()
);
CREATE INDEX meta_alerts_create_time_index ON meta_alerts(create_time);

-- alerts preprocessed
CREATE TABLE    meta_alert_sum
(
  create_time timestamp PRIMARY KEY,
  meta_alerts           bigint,
  meta_alerts_root      bigint
);
CREATE INDEX meta_alert_sum_create_time_index ON meta_alert_sum(create_time);


-- alert to meta-alert mapping table
CREATE TABLE alert_to_meta_alert_map
(
  id_alert      bigint NOT NULL REFERENCES alerts(id),
  id_meta_alert bigint NOT NULL REFERENCES meta_alerts(id),

  UNIQUE(id_alert, id_meta_alert)
);
CREATE INDEX alert_to_meta_alert_map_id_alert_index      ON alert_to_meta_alert_map(id_alert);
CREATE INDEX alert_to_meta_alert_map_id_meta_alert_index ON alert_to_meta_alert_map(id_meta_alert);


--
-- meta-alerts tree is in fact acircular graph. 'tree' name is used, since
-- most of the time this graph is just a set of trees. the exceptions are:
--   1) in general few trees may have common subtree
--   2) multiple parent nodes may have the same child
--   3) it does not have one, single root (using graph terminology, it is
--      "not consistent")
--
-- structure representation from logical point of view looks like this:
--   <parent1; {children set 1}>
--   <parent2; {children set 2}>
--   ...
-- in SQL it is implemented as a relation set, i.e. if parent is in relation with
-- a given child (aka: has a child), it is reflected as a single entry in 'tree'
-- table, like this:
--   <parent1; child1_of_parent1>
--   <parent1; child2_of_parent1>
--   <parent1; child3_of_parent1>
--   ...
--   <parent2; child1_of_parent2>
--   <parent2; child2_of_parent2>
--   ...
-- though such a structure perfectly fits this system needs in general it
-- permits entering data inconsistent with the model, like pairs:
--   <1; 2>
--   <2; 1>
-- that would produce a cycle. in order to prevent such a situations and
-- to make adding new connections between nodes as simple as possible set of
-- PL/pgSQL functions and triggers has been provided.
-- in general data base checks itself if given connection is allowed or not,
-- before commiting data. user can use following functions to make her life
-- easier:
--   - get_parents(id) - returns set of IDs representing all parents above
--                       given node ('id' param).
--   - get_children(id) - returns set of IDs representing all children below
--                        given node ('id' param).
--   - link_meta_alerts(id_from, id_to) - creates link from 'id_from' node to
--                                        to 'id_to' node or raises exception
--                                        if such a link would create circular
--                                        dependency in graph.
--

-- meta alerts tree
CREATE TABLE meta_alerts_tree
(
  id_node  bigint NOT NULL REFERENCES meta_alerts(id),
  id_child bigint NOT NULL REFERENCES meta_alerts(id),

  UNIQUE(id_node, id_child)
);
CREATE INDEX meta_alerts_tree_id_node_index  ON meta_alerts_tree(id_node);
CREATE INDEX meta_alerts_tree_id_child_index ON meta_alerts_tree(id_child);

-- meta alerts roots
CREATE TABLE meta_alerts_roots
(
  id_root bigint NOT NULL UNIQUE REFERENCES meta_alerts(id)
);

--
-- triggers and procedures to make entries in tables
-- alerts/meta_alerts/alert_to_meta_alert_map/meta_alert_tree properly alligned
--

CREATE LANGUAGE plpgsql;

-- get_parents_impl()
CREATE FUNCTION get_parents_impl(id bigint) RETURNS SETOF bigint AS
$$
DECLARE
  id_node_parent bigint;
BEGIN
  FOR id_node_parent IN SELECT id_node FROM meta_alerts_tree WHERE id_child=id
  LOOP
    RETURN NEXT id_node_parent;
    RETURN QUERY SELECT get_parents_impl(id_node_parent);
  END LOOP;
END;
$$ LANGUAGE plpgsql;

-- get_parents()
CREATE FUNCTION get_parents(id bigint) RETURNS SETOF bigint AS
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
CREATE FUNCTION get_children_impl(id bigint) RETURNS SETOF bigint AS
$$
DECLARE
  id_node_child bigint;
BEGIN
  FOR id_node_child IN SELECT id_child FROM meta_alerts_tree WHERE id_node=id AND id_child IS NOT NULL
  LOOP
    RETURN NEXT id_node_child;
    RETURN QUERY SELECT get_children_impl(id_node_child);
  END LOOP;
END;
$$ LANGUAGE plpgsql;

-- get_children()
CREATE FUNCTION get_children(id bigint) RETURNS SETOF bigint AS
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
CREATE FUNCTION link_meta_alerts(id_from bigint, id_to bigint) RETURNS void AS
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
