<?php

class DataMiningAlertTimeSeries extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    if ($this->AlertTS->width == 0)
      $this->AlertTS->width=700;
    if ($this->AlertTS->height == 0)
      $this->AlertTS->height=800;
    $this->generateGraph();
  }

  private function constructUrl($query)
  {
    $linkdata=array( 'title' => 'Alerts time series',
                     'query' => $query,
                     'width' => $this->AlertTS->width,
                     'height' => $this->AlertTS->height);

    return $this->getRequest()->constructUrl('graph', "AlertTimeSeries", $linkdata, false);
  }

  private function generateGraph()
  {
    $this->AlertTS->ImageUrl = $this->constructUrl('DMAlertCountTimeSeries');
  }

}



?>
