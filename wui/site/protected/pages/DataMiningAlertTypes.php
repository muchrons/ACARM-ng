<?php

class DataMiningAlertTypes extends TPage
{
  public function onLoad($param) {
    parent::onLoad($param); 
    
    $this->generateGraph();
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

    $this->AlertTypes->ImageUrl = $this->getRequest()->constructUrl('graph', "AlertTypes", array( 'xdata' => $xdata, 'ydata' => $ydata, 'ytitle' => 'title'), false);
  }

}


?>
