<?php
ini_set("memory_limit","256M"); //override default memory settings

class AnalyzerRecord
{
  public $id;
  public $Link;
  public $Name;
  public $Version;
  public $OS;
  public $IP;
}

class ComputeLinkForAnalyzers
{
  function __construct($service)
  {
    $this->service_=$service;
  }

  private function computeLink($id)
  {
    $url    =$this->service_->constructUrl( 'Analyzer', array('id' => $id) );
    return "<a href=\"$url\">details</a>";
  }

  public function computeStructure($data_row)
  {
    $ret=new AnalyzerRecord();
    $ret->id=$data_row->sys_id;
    $ret->Link=$this->computeLink($data_row->id);
    $ret->Name=$data_row->name;
    $ret->Version=$data_row->version;
    $ret->OS=$data_row->os;
    $ret->IP=$data_row->ip;
    return $ret;
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
