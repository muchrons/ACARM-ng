<?php

class RangeSelector extends TTemplateControl
{
  function __construct()
  {
    parent::__construct();
    $this->From->Timestamp=strtotime("-1 month");
    $this->To->Timestamp=strtotime("today");
  }

  public function onInit($param)
  {
    parent::onInit($param);
  }

  public function getFrom()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('From');
  }

  public function getTo()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('To');
  }

}

?>
