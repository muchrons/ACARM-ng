<?php

function microtime_float()
{
  list($usec, $sec) = explode(" ", microtime());
  return ((float)$usec + (float)$sec);
}

class SQLWrapper
{
  public static function queryForObject($param,$value=null)
  {
    $start=microtime_float();
    $ret=CSQLMap::get()->queryForObject($param,$value);
    $time=microtime_float()-$start;
    $timec=number_format($time,2,'.','');
    Prado::log(substr("SQL query executed: $param in $timec seconds.",0,255), TLogger::INFO, 'WUI.DB');
    return $ret;
  }

  public static function queryForList($param,$value=null)
  {
    $start=microtime_float();
    $ret=CSQLMap::get()->queryForList($param,$value);
    $time=microtime_float()-$start;
    $timec=number_format($time,2,'.','');
    $records=count($ret);
    Prado::log(substr("SQL query executed: $param. Returned $records records in $timec seconds.",0,255), TLogger::INFO, 'WUI.DB');
    return $ret;
  }

  public static function delete($param,$value)
  {
    $start=microtime_float();
    $ret=CSQLMap::get()->delete($param,$value);
    $time=microtime_float()-$start;
    $timec=number_format($time,2,'.','');
    $records=count($ret);
    Prado::log(substr("SQL delete executed: $param. $records records deleted in $timec seconds.",0,255), TLogger::INFO, 'WUI.DB');
    return $ret;
  }

  public static function check_dbversion()
  {
    $min_db_version=8;
    $max_db_version=8;

    $ret=CSQLMap::get()->queryForObject("DBversion");
    if($ret< $min_db_version || $ret> $max_db_version)
      {
        echo "Your ACARM-ng database is in version $ret but this WUI works with database version";
        if ($min_db_version==$max_db_version)
          echo " $min_db_version.";
        else
          echo "s from $min_db_version to $max_db_version.";
        die();
      }
  }
};

?>
