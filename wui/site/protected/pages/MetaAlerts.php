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
    $url=$this->service_->constructUrl( 'MetaAlert', array('mid' => $id) );
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

    if (!$this->isPostBack)
      {
        $srcip=$this->Request->itemAt('srcip');
        $dstip=$this->Request->itemAt('dstip');
        $this->Range->setSrcDst($srcip,$dstip);

        $date_from=$this->Request->itemAt('from');
        $date_to=$this->Request->itemAt('to');
        $this->Range->setDates($date_from,$date_to);

        $type=$this->Request->itemAt('type');
        $this->Range->setType($type);

        $severities=$this->Request->itemAt('severities');
        if( $severities!==null )
          $this->Range->CB->setSelectedValues(explode('.',$severities));
      }

    $parentid=$this->Request->itemAt('parent');

    $this->MetaAlerts->computation_=new ComputeLinkForMetaAlerts($this->Service);

    $this->MetaAlerts->params_=$this->Range->getRangeData();

    if($parentid!=null)
      {
        $this->MetaAlerts->Query="SelectMetaAlertsSummaryByParent";
        $this->MetaAlerts->params_->extra=$parentid;
      }
    elseif($this->Range->Srcip->Text!='any' or $this->Range->Dstip->Text!='any')
      $this->MetaAlerts->Query="SelectMetaAlertsSummaryByIP";
  }
};


?>
