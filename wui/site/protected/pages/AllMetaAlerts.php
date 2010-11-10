<?php
ini_set("memory_limit","256M"); //override default memory limit


class ComputeLinkForMetaAlerts
{
  function __construct($service)
  {
    $this->service_=$service;
  }

  public function computeLink($data_row)
  {
    $url    =$this->service_->constructUrl( 'MetaAlert', array('id' => $data_row->id) );
    return "<a href=\"$url\">details</a>";
  }

  private $service_;
}


class AllMetaAlerts extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $this->MetaAlerts->computation_=new ComputeLinkForMetaAlerts($this->Service);
  }
}


?>
