<?php

class DataMiningAlertTypes extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    if ($this->AlertTypes->width == 0)
      $this->AlertTypes->width=700;
    if ($this->AlertTypes->height == 0)
      $this->AlertTypes->height=800;
    $this->generateGraph();
  }

  private function constructUrl($xdata, $ydata)
  {
    $linkdata=array( 'xdata' => $xdata,
                     'ydata' => $ydata,
                     'ytitle' => 'Alert Types',
                     'width' => $this->AlertTypes->width,
                     'height' => $this->AlertTypes->height);

    return $this->getRequest()->constructUrl('graph', "AlertTypes", $linkdata, false);
  }

  private function generateGraph()
  {
    $pairs=CSQLMap::get()->queryForList('DMAlertTypes');

    $ydata = array();
    $xdata = array();

    foreach( $pairs as $e )
    {
      $xdata[] = $e->key;
      $ydata[] = $e->value;
    }

    $ydata = implode(',', $ydata);
    $xdata = implode(',', $xdata);

    $this->AlertTypes->ImageUrl = $this->constructUrl($xdata, $ydata);
  }

}


?>
