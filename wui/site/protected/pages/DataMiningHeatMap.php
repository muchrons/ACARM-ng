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

    //    if (!$this->IsPostBack)
      {
        $range=CSQLMap::get()->queryForObject("DMHeatMapHostcount");
        $this->minAlert=$range->key;
        $this->maxAlert=$range->value;

        $data=CSQLMap::get()->queryForList("DMHeatMapRange",$range);

        $src=0;
        $dst=0;

        //get arrays of sources and destinations
        foreach( $data as $e )
          {
            if ($sources[$e->source] === null)
              $sources[$e->source]=$src++;
            if ($destinations[$e->destination] === null)
              $destinations[$e->destination]=$dst++;
          }

        //change mapping from IP->number to number->IP
        foreach ($sources as $k => $v)
          $this->sources[$v]=$k;

        foreach ($destinations as $k => $v)
          $this->destinations[$v]=$k;
      }
  }

  private function constructUrl()
  {
    $linkdata=array( 'title' => 'HeatMap',
                     'width' => $this->HeatMap->width,
                     'height' => $this->HeatMap->height);

    return $this->getRequest()->constructUrl('image', "HeatMap", $linkdata, false);
  }

  private function generateGraph()
  {
    $this->HeatMap->ImageUrl = $this->constructUrl();
  }


  public function clicked($sender,$param)
  {
    $this->SrcIP->Text=$this->sources[$this->longitude->Value];
    $this->DstIP->Text=$this->destinations[$this->latitude->Value];
  }

  private $minAlert;
  private $maxAlert;

  private $sources;
  private $destinations;
}

?>
