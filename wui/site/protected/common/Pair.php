<?php

class Pair
{
  public function __construct($f,$s)
  {
    $this->first=$f;
    $this->second=$s;
  }

  public static function compare_rev($a, $b)
  {
    if ($a->second > $b->second) return -1;
    else if($a->second == $b->second) return 0;
    else return 1;
  }

  public $first;
  public $second;
};

?>