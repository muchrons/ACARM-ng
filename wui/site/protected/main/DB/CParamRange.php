<?php

class CParamRange
{
  function __construct()
  {
    $this->orderdir="DESC";
    $this->ordercol="create_time";
  }
  public $date_from;
  public $date_to;
  public $srct;
  public $src;
  public $ignoresrc;
  public $dstt;
  public $dst;
  public $ignoredst;
  public $severities;
  public $type;
  public $ignoretype;
  public $limit;
  public $offset;
  public $orderdir;
  public $ordercol;
  public $extra;
}

?>
