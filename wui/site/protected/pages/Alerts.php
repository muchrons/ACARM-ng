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
    $url=$this->service_->constructUrl( 'MetaAlert', array('aid' => $id) );
    return "<a href=\"$url\">details</a>";
  }

  public function computeStructure($data_row)
  {
    $ret=new AlertRecord();
    $ret->id=$data_row->id;
    $ret->Link=$this->computeLink($data_row->id);
    $ret->Name=$data_row->name;

    $ret->Severity="<font";
    $value=trim($data_row->severity);

    if ($value=="4")
      $ret->Severity.=" color=\"red\"";
    elseif ($value=="3")
      $ret->Severity.=" color=\"orange\"";
    elseif ($value=="2")
      $ret->Severity.=" color=\"yellow\"";
    elseif ($value=="1")
      $ret->Severity.=" color=\"green\"";
    elseif ($value=="0")
      $ret->Severity.=" color=\"blue\"";

    $ret->Severity.="><b>".$this->severityToName($value)."</b></font>";

    $data_row->severity;
    $ret->Created=$data_row->create_time;
    return $ret;
  }

  private function severityToName($n)
  {
    if ($n=="0")
      return "debug";
    if ($n=="1")
      return "info";
    if ($n=="2")
      return "low";
    if ($n=="3")
      return "medium";
    if ($n=="4")
      return "high";
    return "unknown";
  }


  private $service_;
};


class Alerts extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    if (!$this->isPostBack)
      {
        $srcip=$this->Request->itemAt('srcip');
        $dstip=$this->Request->itemAt('dstip');
        $this->Range->setSrcDst($srcip,$dstip);

        $this->Range->setSev("Alert");

        $date_from=$this->Request->itemAt('from');
        $date_to=$this->Request->itemAt('to');
        $this->Range->setDates($date_from,$date_to);

        $type=$this->Request->itemAt('type');
        if( $type!==null )
          $this->Range->setType("(^".$this->clearType($type)."$)");

        $severities=$this->Request->itemAt('severities');
        if( $severities!==null )
          $this->Range->CB->setSelectedValues(explode('.',$severities));
      }
    $this->Alerts->computation_=new ComputeLinkForAlerts($this->Service);
    $this->Alerts->params_=$this->Range->getRangeData();
  }

  private function clearType($param)
  {
    $escape_from=array("\\","(",")","^","$",".","*","?");
    $escape_to  =array("\\\\","\(","\)","\^","\\$","\.","\\*","\\?");
    return str_replace($escape_from,$escape_to,$param);
  }
};

?>
