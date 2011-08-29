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

    $ret->Severity="<font";
    $value=trim($data_row->severity);

    if ($value=="4")
      $ret->Severity.=" color=\"red\"";
    else
      if ($value=="3")
        $ret->Severity.=" color=\"orange\"";
      else
        if ($value=="2")
          $ret->Severity.=" color=\"yellow\"";
        else
          if ($value=="1")
            $ret->Severity.=" color=\"green\"";
          else
            if ($value=="0")
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
        if( $srcip!==null )
          $this->Range->srcip->Text=$srcip;

        $dstip=$this->Request->itemAt('dstip');
        if( $dstip!==null )
          $this->Range->dstip->Text=$dstip;

        $date_from=$this->Request->itemAt('from');
        $date_to=$this->Request->itemAt('to');
        $this->Range->setDates($date_from,$date_to);

        $type=$this->Request->itemAt('type');
        if( $type!==null )
          $this->Range->Type->Text=$type;

        $severities=$this->Request->itemAt('severities');
        if( $severities!==null )
          $this->Range->CB->setSelectedValues(explode('.',$severities));
      }
    $this->Alerts->computation_=new ComputeLinkForAlerts($this->Service);
    $this->Alerts->params_=$this->Range->getRangeData();
  }
};


?>
