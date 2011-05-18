<?php

class SQLWrapper
{
  public function queryForObject($param,$value=null)
  {
    Prado::log("SQL query executed: $param", TLogger::ALERT, 'WUI.DB');
    return CSQLMap::get()->queryForObject($param,$value);
  }

  public function queryForList($param,$value=null)
  {
    $ret=CSQLMap::get()->queryForList($param,$value);
    $records=count($ret);
    Prado::log("SQL query executed: $param. Returned $records records.", TLogger::ALERT, 'WUI.DB');
    return $ret;
  }
};

?>