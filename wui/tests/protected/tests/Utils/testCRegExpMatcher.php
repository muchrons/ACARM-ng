<?php

class testCRegExpMatcher extends UnitTestCase
{
  public function testPregMatchOnPolishStr1()
  {
    $this->assertTrue( CRegExpMatcher::pregMatch('/^[A-Z][a-z]+$/', "Łączkówkiśniż") );
  }

  public function testPregMatchOnPolishStr2()
  {
    $this->assertTrue( CRegExpMatcher::pregMatch('/^([a-z][A-Z])+$/', "aĄbŁ") );
  }

  public function testPregMatchNoMatchOnPolishStr1()
  {
    $this->assertFalse( CRegExpMatcher::pregMatch('/^[A-Z][a-z]+$/', "ła") );
  }
}

?>
