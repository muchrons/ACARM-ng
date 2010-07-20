<?php

class CUserRecord extends TActiveRecord
{
  const TABLE='users';

  public $id;
  public $username;
  public $password;

  public static function finder($className=__CLASS__)
  {
    return parent::finder($className);
  }
}

?>
