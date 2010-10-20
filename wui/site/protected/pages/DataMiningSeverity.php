<?php

class DataMiningSeverity extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    if ($this->SeveritiesImg->width == 0)
      $this->SeveritiesImg->width=700;
    if ($this->SeveritiesImg->height == 0)
      $this->SeveritiesImg->height=800;
    $this->generateGraph();
  }

  private function constructUrl()
  {
    $linkdata=array( 'title' => 'Alert count by severity type',
                     'width' => $this->SeveritiesImg->width,
                     'height' => $this->SeveritiesImg->height);

    return $this->getRequest()->constructUrl('graph', "SeverityPie", $linkdata, false);
  }

  private function generateGraph()
  {
    $this->SeveritiesImg->ImageUrl = $this->constructUrl();
  }


}

?>
