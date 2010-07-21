<?php

//Prado::using('System.Data.SqlMap.TSqlMapManager');

// set of helper methods to be used from tests
//
// deriving from UnitTestCase - this is just stub so that framework will not
// complain about not test class in this file - simply ignore it.
class CTestSQLHelper extends UnitTestCase
{
  // executes simple SQL query
  public static function execSQL($sql)
  {
    if($sql==null)
      throw new TInvalidDataValueException('null query string is not acceptable');

    $conn=TActiveRecordManager::getInstance()->getDbConnection();
    $conn->Active=true;  // connection is established
    $command=$conn->createCommand($sql);
    $affectedRowCount=$command->execute();
  }

  // builds and returns SQLMap object
  public static function getSQLMap()
  {
    return CSQLMap::get();
  }

  // returns number of entries in table
  public static function getEntryCount($tableName)
  {
    if($tableName==null)
      throw new TInvalidDataValueException("null table name not allowed");
    $conn=TActiveRecordManager::getInstance()->getDbConnection();
    $conn->Active=true;  // connection is established
    // yes, following can lead to SQL-injection but it will stay this way sinc these are only tests...
    $command=$conn->createCommand("SELECT * FROM $tableName");
    $reader=$command->query();
    if($reader==null)
      throw new TInvalidDataValueException("null entries returned");

    // count number of read entries
    $count=0;
    foreach($reader as $row)
    {
      ++$count;
    }
    return $count;
  }
}

?>
