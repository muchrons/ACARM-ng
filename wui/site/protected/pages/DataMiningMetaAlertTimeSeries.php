<?php
class DataMiningMetaAlertTimeSeries extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    if ($this->MetaAlertTS->width == 0)
      $this->MetaAlertTS->width=700;
    if ($this->MetaAlertTS->height == 0)
      $this->MetaAlertTS->height=800;
    $this->generateGraph();
  }

  private function constructUrl($query)
  {
    $linkdata=array( 'title' => 'Metaalerts time series',
                     'query' => $query,
                     'width' => $this->MetaAlertTS->width,
                     'height' => $this->MetaAlertTS->height);

    return $this->getRequest()->constructUrl('graph', "AlertTimeSeriesXXX", $linkdata, false);
  }

  private function generateGraph()
  {
    $this->MetaAlertTS->ImageUrl = $this->constructUrl('DMMetaAlertCountTimeSeries');
  }

}
?>