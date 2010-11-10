<?php

class DataMiningAlertTypes extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    if ($this->AlertTypes->width == 0)
      $this->AlertTypes->width=700;
    if ($this->AlertTypes->height == 0)
      $this->AlertTypes->height=700;

    $this->Range->srcip->setEnabled(false);
    $this->Range->dstip->setEnabled(false);
    $this->Range->CB->setEnabled(false);

    $from=$this->Range->From->Date;
    $to=$this->Range->To->Date;
    $src=$this->Range->srcip->Text;
    $dst=$this->Range->dstip->Text;

    $width=$this->AlertTypes->width;
    $height=$this->AlertTypes->height;

    $this->generateGraph($width,$height,$from,$to,$src,$dst);
  }

  private function constructUrl($width,$height,$from,$to,$src,$dst)
  {
    $linkdata=array( 'title' => 'Alert Types',
                     'width' => $width,
                     'height' => $height,
                     'query' => 'DMAlertTypes',
                     'from' => $from,
                     'to' => $to,
                     'src' => $src,
                     'dst' => $dst);

    return $this->getRequest()->constructUrl('graph', "AlertTypes", $linkdata, false);
  }

  private function generateGraph($width,$height,$from,$to,$src,$dst)
  {
    $this->AlertTypes->ImageUrl = $this->constructUrl($width,$height,$from,$to,$src,$dst);
  }
}


?>
