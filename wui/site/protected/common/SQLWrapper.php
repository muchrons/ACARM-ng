<?php

function microtime_float()
{
  list($usec, $sec) = explode(" ", microtime());
  return ((float)$usec + (float)$sec);
}

class SQLWrapper
{
  public function queryForObject($param,$value=null)
  {
    $start=microtime_float();
    $ret=CSQLMap::get()->queryForObject($param,$value);
    $time=microtime_float()-$start;
    $timec=number_format($time,2,'.','');
    Prado::log("SQL query executed: $param in $timec seconds.", TLogger::INFO, 'WUI.DB');
    return $ret;
  }

  public function queryForList($param,$value=null)
  {
    $start=microtime_float();
    $ret=CSQLMap::get()->queryForList($param,$value);
    $time=microtime_float()-$start;
    $timec=number_format($time,2,'.','');
    $records=count($ret);
    Prado::log("SQL query executed: $param. Returned $records records in $timec seconds.", TLogger::INFO, 'WUI.DB');
    return $ret;
  }
};

?>