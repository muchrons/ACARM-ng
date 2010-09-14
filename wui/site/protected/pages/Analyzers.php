<?php

class ComputeLinkForAnalyzers
{
  function __construct($service) 
  {
    $this->service_=$service;
  }
  
  public function computeLink($data_row)
  {
    $url    =$this->service_->constructUrl( 'Analyzer', array('id' => $data_row->id) );
    return "<a href=\"$url\">details</a>";
  }

  private $service_;
}  


class Analyzers extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $this->Analyzers->computation_=new ComputeLinkForAnalyzers($this->Service);
  }
}


?>
