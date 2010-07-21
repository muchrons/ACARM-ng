<?php

class CUserRecord extends TActiveRecord
{
  const TABLE='wui_users';

  public $id;
  public $login;
  public $pwdhash;

  public static function finder($className=__CLASS__)
  {
    return parent::finder($className);
  }
}

?>
