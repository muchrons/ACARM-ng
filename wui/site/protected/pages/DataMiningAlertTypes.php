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

  public function computeStructure($data_row)
  {
    $ret=new TypeRecord();
    $ret->Count=$data_row->value."&nbsp;";
    $ret->Name=trim($data_row->key);
    return $ret;
  }

  private $service_;
};


class DataMiningAlertTypes extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);

    $this->Alerts->computation_=new ComputeLinkForAlerts($this->Service);
    $this->Alerts->params_=$this->Range->getRangeData();
  }

  private function constructUrl($from,$to,$src,$dst,$severities,$type)
  {
    $linkdata=array( 'srcip' => $src,
                     'dstip' => $dst,
                     'from' => $from,
                     'to' => $to,
                     'severities' => $severities,
                     'type' => $type);

    return $this->getRequest()->constructUrl('page', "Alerts", $linkdata);
  }

  function getSelectedRows()
  {
    $selection=$this->Alerts->getSelectedTypes();

    if($selection=="(^$)")
      {
        echo "No typed were selected. Maybe you have turned off JavaScript.";
        die();
      }

    $range=$this->Range->getRangeData();
    $link=$this->constructUrl($range->date_from, $range->date_to, $range->srct, $range->dstt, $range->severities, $selection);
    $this->Response->redirect($link);
  }
}

?>
