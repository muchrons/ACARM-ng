<?php

class MainMenuTemplate extends TTemplateControl
{
  public function buttonClicked($sender,$param)
  {
    $this->MultiView->setActiveViewIndex(2);
    $sender->Text="I'm clicked";
    echo "azsdasdasdasdAS";
    //    die("O my god");
  }

  public function callback($param,$param2)
  {
    //die("O my god");
  }
}

?>
