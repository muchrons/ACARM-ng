<?php

class DataMiningMetaAlertTimeSeries extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $this->AlertTimeSeries->ImageUrl = $this->getRequest()->constructUrl('graph', "AlertTimeSeries", array("query" => "DMMetaAlertCountTimeSeries"));
  }
}


?>
