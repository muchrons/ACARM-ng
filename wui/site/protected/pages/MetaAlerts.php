<?php
ini_set("memory_limit","256M"); //override default memory settings

class MetaAlertRecord
{
  public $id;
  public $Link;
  public $Name;
  public $Created;
};

class ComputeLinkForMetaAlerts
{
  function __construct($service)
  {
    $this->service_=$service;
  }

  private function computeLink($id)
  {
    $url=$this->service_->constructUrl( 'MetaAlert', array('id' => $id) );
    return "<a href=\"$url\">details</a>";
  }

  public function computeStructure($data_row)
  {
    $ret=new MetaAlertRecord();
    $ret->id=$data_row->id;
    $ret->Link=$this->computeLink($data_row->id);
    $ret->Name=$data_row->name;
    $ret->Created=$data_row->create_time;
    return $ret;
  }

  private $service_;
};


class MetaAlerts extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $this->MetaAlerts->computation_=new ComputeLinkForMetaAlerts($this->Service);
  }
};


?>
