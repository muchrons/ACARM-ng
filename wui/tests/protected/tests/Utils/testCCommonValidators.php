<?php

class testCCommonValidators extends UnitTestCase
{
  //
  // TEST IP
  //

  public function testValidIP1()
  {
    $this->assertTrue( CCommonValidators::isIP("12.13.14.15") );
  }

  public function testZeroPaddedIP()
  {
    $this->assertTrue( CCommonValidators::isIP("012.013.014.015") );
  }

  public function testNullIP()
  {
    $this->assertFalse( CCommonValidators::isIP(null) );
  }

  public function testTooShortIP()
  {
    $this->assertFalse( CCommonValidators::isIP("1.2.3") );
  }

  public function testDotEndIP()
  {
    $this->assertFalse( CCommonValidators::isIP("1.2.3.4.") );
  }

  public function testLetterInIP()
  {
    $this->assertFalse( CCommonValidators::isIP("1.2.3a.4") );
  }

  public function testTooBigNumberInIP()
  {
    $this->assertFalse( CCommonValidators::isIP("1.256.3.4") );
  }

  //
  // TEST HOST
  //

  public function testLocalhostName()
  {
    $this->assertTrue( CCommonValidators::isHost("localhost") );
  }

  public function testSomeInternetHost()
  {
    $this->assertTrue( CCommonValidators::isHost("www.google.pl") );
  }

  public function testNumbersInHostName()
  {
    $this->assertTrue( CCommonValidators::isHost("www.d9.pl") );
  }

  public function testHypenInHost()
  {
    $this->assertTrue( CCommonValidators::isHost("www.some-service.pl") );
  }

  public function testMultiDotInARowHost()
  {
    $this->assertFalse( CCommonValidators::isHost("invalid..domain") );
  }

  public function testStartingDotHost()
  {
    $this->assertFalse( CCommonValidators::isHost(".invalid.domain") );
  }

  public function testEndingDotHost()
  {
    $this->assertFalse( CCommonValidators::isHost("invalid.domain.") );
  }

  public function testUpperCaseHost()
  {
    $this->assertFalse( CCommonValidators::isHost("inValid.domain") );
  }

  public function testLeadingNumber()
  {
    $this->assertFalse( CCommonValidators::isHost("3nvalid.domain") );
  }

  //
  // TEST E-MAIL
  //

  public function testLocalEmail()
  {
    $this->assertTrue( CCommonValidators::isEmail("user@mypc") );
  }

  public function testInternetEmail()
  {
    $this->assertTrue( CCommonValidators::isEmail("alice@gmail.com") );
  }

  public function testDotEmail()
  {
    $this->assertTrue( CCommonValidators::isEmail("alice.cat@gmail.com") );
  }

  public function testHypEmail()
  {
    $this->assertTrue( CCommonValidators::isEmail("alice-cat@gmail.com") );
  }

  public function testHypEndingInUsernameOfEmail()
  {
    $this->assertFalse( CCommonValidators::isEmail("alice-@gmail.com") );
  }

  public function testDotEndingInUsernameOfEmail()
  {
    $this->assertFalse( CCommonValidators::isEmail("alice.@gmail.com") );
  }

  public function testUpperCaseInUsernameOfEmail()
  {
    $this->assertFalse( CCommonValidators::isEmail("aliCe@gmail.com") );
  }

  public function testNoAtSimbolInEmail()
  {
    $this->assertFalse( CCommonValidators::isEmail("invalid.email") );
  }

  public function testMultipleAtSimbolsInEmail()
  {
    $this->assertFalse( CCommonValidators::isEmail("alice@has@cat.pl") );
  }
}

?>
