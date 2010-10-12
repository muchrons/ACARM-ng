<?php

class Layout extends TTemplateControl
{
  public function onLoad($param)
  {
    parent::onLoad($param);

    if ($this->content->HasControls)
      echo "<h1>true</h1>";
    else
      echo "<h1>false</h1>";

    ///$this->mainmenu->MultiView->ActiveViewIndex=$this->content->roll();
  }
}

?>
