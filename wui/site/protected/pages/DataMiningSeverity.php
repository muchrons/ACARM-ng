<?php

class DataMiningSeverity extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $this->SeveritiesImg->ImageUrl = $this->getRequest()->constructUrl('graph', "SeverityPie");
  }
}

?>
