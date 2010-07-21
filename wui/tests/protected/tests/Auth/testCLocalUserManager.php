<?php

class testCLocalUserManager extends UnitTestCase
{
  private function cleanTable()
  {
    CTestSQLHelper::execSQL('DELETE FROM wui_users');
    $cnt=CTestSQLHelper::getEntryCount('wui_users');
    $this->assertTrue($cnt===0, "users not remvoed - still $cnt users in data base");
  }

  // returns user record. it is automaticaly added to DB whren parameter is true.
  private function createUser($name, $password)
  {
    $ur=new CUserRecord();
    $ur->login  =$name;
    $ur->pwdhash=md5($password);
    $ur->save();
    return $ur;
  }

  private function validate($username, $password)
  {
    $pwdHash=md5($password);
    $manager=new CLocalUserManager();
    $ret    =$manager->validateUser($username, $pwdHash);
    return $ret;
  }

  function testGettingGuestName()
  {
    $manager=new CLocalUserManager();
    $guest  =$manager->getGuestName();
    $this->assertTrue($guest==="guest", "guest user name ('$guest') does not match expected ('guest')");
  }

  function testGettingGuestUser()
  {
    $manager=new CLocalUserManager();

    $guest=$manager->getUser(null);
    $this->assertTrue($guest->isGuest, "guest is not marked as guest");

    $user=$manager->getUser('a.b');
    $this->assertFalse($user->isGuest, "non-guest is marked as guest");
  }

  function testValidatingNonExistingUser()
  {
    $this->cleanTable();
    $ret=$this->validate('IdoNotExist', 'nothingness');
    $this->assertTrue($ret===false, "validation of non-existing user succedded");
  }

  function testValidatingExistingUser()
  {
    $this->cleanTable();
    $this->createUser('killer', 'secret');
    $ret=$this->validate('killer', 'secret');
    $this->assertTrue($ret===true, "validation of existing user failed");
  }

  function testValidatingExistingBlockedUser()
  {
    $this->cleanTable();
    $this->createUser('Jan', 'Kszak', true);
    $ret=$this->validate('jan.kszak', 'secret');
    $this->assertTrue($ret===false, "validation of existing, blocked user succedded");
  }

  function testValidatingExistingUserWithWrongPassword()
  {
    $this->cleanTable();
    $this->createUser('killer', 'secret');
    $ret=$this->validate('killler', 'wrong-secret');
    $this->assertTrue($ret===false, "validation of existing user with wrong password succedded");
  }

  function testValidatingWrongUserWithExistingPassword()
  {
    $this->cleanTable();
    $this->createUser('killer', 'secret');
    $ret=$this->validate('non.existing', 'secret');
    $this->assertTrue($ret===false, "validation of non-existing user with existing password succedded");
  }
}

?>
