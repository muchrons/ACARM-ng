<?php

class ComputeLinkForAlerts
{
  function __construct($service) 
  {
    $this->service_=$service;
  }
  
  public function computeLink($data_row)
  {
    $url    =$this->service_->constructUrl( 'Alert', array('id' => $data_row->id) );
    return "<a href=\"$url\">details</a>";
  }

  private $service_;
}
  


class InputAlerts extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $this->Alerts->computation_=new ComputeLinkForAlerts($this->Service);
  }
}


?>
