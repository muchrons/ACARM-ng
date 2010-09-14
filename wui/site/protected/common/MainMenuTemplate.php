<?php

class MainMenuTemplate extends TTemplateControl
{
  public function buttonClicked($sender,$param)
  {
    $this->Result->Text="Command name: $param->CommandName, Command parameter: $param->CommandParameter";
  }
}

?>
