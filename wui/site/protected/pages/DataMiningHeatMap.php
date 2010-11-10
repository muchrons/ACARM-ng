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

  private function generateGraph()
  {
    $this->HeatMap->ImageUrl = $this->constructUrl();
  }


  public function buttonClicked($sender,$param)
  {
    if($param instanceof TCommandEventParameter)
      {
        $this->Result2->Text="Command name: $param->CommandName, Command parameter: $param->CommandParameter";
      }
    else
      {
        $this->Result->Text="You clicked at ($param->X,$param->Y)";
      }
  }

}

?>
