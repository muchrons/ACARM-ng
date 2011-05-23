<?php

class NagiosPlugin extends TPage
{
  function __construct()
  {
    // heat data from data base
    $heartbeats=CSQLMap::get()->queryForList('SelectHeartbeats');

    // print status of each enrty
    foreach($heartbeats as $h)
    {
      echo "{$h->owner} # {$h->module} # ";
      if($h->alive)
        echo "alive";
      else
        echo "DEAD";
      echo "\n";
    }

    // we do not want whole page to be rendered - just the heartbeats raw data.
    die("");
  }
}

?>
