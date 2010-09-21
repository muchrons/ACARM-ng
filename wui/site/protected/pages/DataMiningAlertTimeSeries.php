<?php

class DataMiningAlertTimeSeries extends TPage
{
  public function onLoad($param) {
    parent::onLoad($param); 
    
    $this->generateGraph();
  }

 
  private function generateGraph() 
  {
    $pairs=CSQLMap::get()->queryForList('DMAlertCountTimeSeries');

    $ydata = array();
    $xdata = array();
 
    foreach( $pairs as $e ) 
      {
        $xdata[] = $e->key;
        $ydata[] = $e->value;
      }

    $ydata = implode(',', $ydata);
    $xdata = implode(',', $xdata);

    $this->AlertTimeSeries->ImageUrl = $this->getRequest()->constructUrl('graph', "AlertTimeSeries", array( 'xdata' => $xdata, 'ydata' => $ydata, 'ytitle' => 'title'), false);
  }

}


?>
