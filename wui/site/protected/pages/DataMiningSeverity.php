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

    $from=$this->Range->From->Date;
    $to=$this->Range->To->Date;
    $severities=implode($this->Range->CB->SelectedValues);
    $src=$this->Range->srcip->Text;
    $dst=$this->Range->dstip->Text;

    $width=$this->SeveritiesImg->width;
    $height=$this->SeveritiesImg->height;

    $this->generateGraph($width,$height,$from,$to,$severities,$src,$dst);
  }

  private function constructUrl($width,$height,$from,$to,$severities,$src,$dst)
  {
    $linkdata=array( 'title' => 'Alert count by severity type',
                     'width' => $this->SeveritiesImg->width,
                     'height' => $this->SeveritiesImg->height,
                     'from' => $from,
                     'to' => $to,
                     'severities' => $severities,
                     'src' => $src,
                     'dst' => $dst);

    return $this->getRequest()->constructUrl('graph', "SeverityPie", $linkdata, false);
  }

  private function generateGraph($width,$height,$from,$to,$severities,$src,$dst)
  {
    $this->SeveritiesImg->ImageUrl = $this->constructUrl($width,$height,$from,$to,$severities,$src,$dst);
  }


}

?>
