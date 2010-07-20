<?php

class testCUTFCleaner extends UnitTestCase
{
  public function testPolishUppercaseReplaces()
  {
    $in ="#Ą#Ć#Ę#Ł#Ń#Ó#Ś#Ź#Ż#";
    $out=CUTFCleaner::removeUTFs($in);
    $this->assertNotNull($out);
    $this->assertEqual($out, "#A#C#E#L#N#O#S#Z#Z#");
  }

  public function testPolishLowercaseReplaces()
  {
    $in ="#ą#ć#ę#ł#ń#ó#ś#ź#ż#";
    $out=CUTFCleaner::removeUTFs($in);
    $this->assertNotNull($out);
    $this->assertEqual($out, "#a#c#e#l#n#o#s#z#z#");
  }

  public function testNullUTF()
  {
    $in =null;
    $out=CUTFCleaner::removeUTFs($in);
    $this->assertNull($out);
  }
}

?>
