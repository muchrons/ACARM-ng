<?php

class Credits extends TPage
{
  public function onLoad($param)
  {
    $names=array("Bartosz Szurgot","Bartłomiej Balcerek","Mariusz Uchroński","Wojciech Waga");
    shuffle($names);
    $this->nr1->Text=$names[0];
    $this->nr2->Text=$names[1];
    $this->nr3->Text=$names[2];
    $this->nr4->Text=$names[3];
  }
};

?>
