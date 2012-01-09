<?php

class DataMiningHeatMap extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $g=new GdChecker(); //checks if GD is installed and available
    $this->IPs->date_from=$this->Range->getFrom()->Text." 0:00:00";
    $this->IPs->date_to=$this->Range->getTo()->Text." 23:59:59";
  }

  private function constructUrlForCallbacks($type,$srcip,$dstip,$from,$to)
  {
    $linkdata=array( 'srcip' => $srcip,
                     'dstip' => $dstip,
                     'from' => $from,
                     'to' => $to);

    return $this->getRequest()->constructUrl('page', $type, $linkdata);
  }

  public function getSrcIP()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('SrcIP');
  }

  public function setSrcIP()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('SrcIP');
  }

  public function clicked($sender,$param)
  {
    $longitude = $param->CallbackParameter->longitude;
    $latitude = $param->CallbackParameter->latitude;

    $this->SrcIP->Text=$longitude;
    $this->DstIP->Text=$latitude;

    $query_param=new CDMQuad();
    $query_param->value1=$param->CallbackParameter->minAlert;
    $query_param->value2=$param->CallbackParameter->maxAlert;
    $query_param->value3=$param->CallbackParameter->longitude;
    $query_param->value4=$param->CallbackParameter->latitude;

    $data=SQLWrapper::queryForList("DMHeatMapNumAlerts",$query_param);

    $this->NoAlerts->Text=$data;

    $this->AlertsLink->NavigateUrl=$this->constructUrlForCallbacks("Alerts",$longitude,$latitude,$this->Range->From->Text,$this->Range->To->Text);
    $this->SeveritiesLink->NavigateUrl=$this->constructUrlForCallbacks("DataMiningSeverity",$longitude,$latitude,$this->Range->From->Text,$this->Range->To->Text);
  }

  public function btn($sender,$param)
  {
    $this->SrcIP->Text=$this->longitude->Text;
  }

  private $minAlert;
  private $maxAlert;

  private $sources;
  private $destinations;
}

?>
