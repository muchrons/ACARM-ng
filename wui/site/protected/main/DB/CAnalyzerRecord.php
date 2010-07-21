<?php

class CAnalyzerRecord extends TActiveRecord
{
  const TABLE='analyzers';

  public $id;
  public $sys_id;
  public $name;
  public $version;
  public $os;
  public $ip;

  public static function finder($className=__CLASS__)
  {
    return parent::finder($className);
  }
}

?>
