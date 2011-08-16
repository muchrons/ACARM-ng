<?php

class TypeRecord
{
  public $id;
  public $Count;
  public $Name;
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
    $ret=new TypeRecord();
    $ret->Count=$data_row->value."&nbsp";
    $ret->Name="&nbsp".$data_row->key;
    return $ret;
  }

  private $service_;
};


class DataMiningAlertTypes extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);

    $this->Range->CB->setEnabled(false);

    $from=$this->Range->From->Date;
    $to=$this->Range->To->Date;
    $src=$this->Range->srcip->Text;
    $dst=$this->Range->dstip->Text;

    $this->Alerts->computation_=new ComputeLinkForAlerts($this->Service);
    $this->Alerts->params_=$this->Range->getRangeData();
  }

  private function constructUrl($type,$severities,$from,$to,$src,$dst)
  {
    $linkdata=array( 'srcip' => $src,
                     'dstip' => $dst,
                     'from' => $from,
                     'to' => $to,
                     'severities' => $severities,
                     'type' => $type);

    return $this->getRequest()->constructUrl('page', "Alerts", $linkdata);
  }

  private function generateGraph($width,$height,$from,$to,$src,$dst)
  {
    $this->AlertTypes->ImageUrl = $this->constructUrl($width,$height,$from,$to,$src,$dst);
  }
}


?>
