<?php
ini_set("memory_limit","256M"); //override default memory settings

class AlertRecord
{
  public $id;
  public $Link;
  public $Name;
  public $Severity;
  public $Created;
};

class ComputeLinkForAlerts
{
  function __construct($service)
  {
    $this->service_=$service;
  }

  private function computeLink($id)
  {
    $url=$this->service_->constructUrl( 'Alert', array('id' => $id) );
    return "<a href=\"$url\">details</a>";
  }

  public function computeStructure($data_row)
  {
    $ret=new AlertRecord();
    $ret->id=$data_row->id;
    $ret->Link=$this->computeLink($data_row->id);
    $ret->Name=$data_row->name;
    $ret->Severity=$data_row->severity;
    $ret->Created=$data_row->create_time;
    return $ret;
  }

  private $service_;
};


class Alerts extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $this->Alerts->computation_=new ComputeLinkForAlerts($this->Service);
  }
};


?>
