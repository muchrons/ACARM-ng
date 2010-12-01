<?php

class DataMiningHeatMap extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    /*
    if ($this->HeatMap->width == 0)
      $this->HeatMap->width=700;
    if ($this->HeatMap->height == 0)
      $this->HeatMap->height=800;
    $this->generateGraph();
    */

  }

  private function constructUrl()
  {
    $linkdata=array( 'title' => 'HeatMap',
                     'width' => $this->HeatMap->width,
                     'height' => $this->HeatMap->height);

    return $this->getRequest()->constructUrl('image', "HeatMap", $linkdata, false);
  }

  private function constructUrlForCallbacks($type,$srcip,$dstip)
  {
    $linkdata=array( 'srcip' => $srcip,
                     'dstip' => $dstip);

    return $this->getRequest()->constructUrl('page', $type, $linkdata);
  }

  private function generateGraph()
  {
    $this->HeatMap->ImageUrl = $this->constructUrl();
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

    $data=CSQLMap::get()->queryForList("DMHeatMapNumAlerts",$query_param);

    $this->NoAlerts->Text=$data;


    $this->AlertsLink->NavigateUrl=$this->constructUrlForCallbacks("Alerts",$longitude,$latitude);
    $this->SeveritiesLink->NavigateUrl=$this->constructUrlForCallbacks("DataMiningSeverity",$longitude,$latitude);
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
