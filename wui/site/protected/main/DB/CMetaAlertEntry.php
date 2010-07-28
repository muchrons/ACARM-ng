<?php

class CMetaAlertEntry
{
  public $id;
  public $name;
  public $create_time;
  public $last_update_time;
  public $severity_delta;
  public $certainty_delta;
  public $id_alert;             // id of associated alert (leaf) or null (node)
}

?>
