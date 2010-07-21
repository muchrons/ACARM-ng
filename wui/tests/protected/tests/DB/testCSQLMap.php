<?php

class testCSQLMap extends UnitTestCase
{
  function testGetingInstance()
  {
    $sqlmap=CSQLMap::get();
    $this->assertNotNull($sqlmap, "null has been returned instead of sqlmap");
  }

  function testUsingInstance()
  {
    $res=CSQLMap::get()->queryForList('SelectUsers');
    $this->assertNotNull($res, "unexpetec null result recieved");
  }

  function testCreatingTransaction()
  {
    $transaction=CSQLMap::beginTransaction();
    $transaction->rollback();
  }
}

?>
