<?php

Prado::using('System.Data.SqlMap.TSqlMapManager');

//
// helper class for handling SQL maps.
//
class CSQLMap
{
  // returns instance of properly configures SQL map.
  public static function get()
  {
    $conn=CSQLMap::getConnection();

    $manager=new TSqlMapManager($conn);
    if($manager===null)
      throw new TInvalidDataValueException("unable to create manager instance");
    $manager->configureXml('protected/mp-sqlmap.xml');

    $sqlmap=$manager->getSqlMapGateway();
    if($sqlmap===null)
      throw new TInvalidDataValueException("uanlbe to create sqlMap object");
    return $sqlmap;
  }

  // starts new transaction and returns handle to it.
  public static function beginTransaction()
  {
    $conn=CSQLMap::getConnection();
    return $conn->beginTransaction();
  }

  // gets connection, with proper setup
  private static function getConnection()
  {
    // TODO: i don't like this implementation - it gives config file name explicitly.
    //       it should not be done this way. to be fixed later on.
    // TODO: I've found a different syntax for obtaining SQLMap:
    // $this->sqlmap = $this->Application->Modules['sqlmap']->Client;
    // what's the difference?
    $conn=TActiveRecordManager::getInstance()->getDbConnection();
    if($conn===null)
      throw new TInvalidDataValueException("unable to create connection object");

    $conn->Active=true;     // ensure connection is established

    $tzSetup=$conn->createCommand("SET TIMEZONE TO UTC;");
    if($tzSetup===null)
      throw new TInvalidDataValueException("cannot create SQL command to change timezone");
    $tzSetup->execute();

    // return properly configured connection
    return $conn;
  }
}

?>
