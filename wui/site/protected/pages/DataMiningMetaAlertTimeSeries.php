<?php

class DataMiningMetaAlertTimeSeries extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    if ($this->AlertTS->width == 0)
      $this->AlertTS->width=700;
    if ($this->AlertTS->height == 0)
      $this->AlertTS->height=800;

    $this->Range->srcip->setEnabled(false);
    $this->Range->dstip->setEnabled(false);

    $this->Range->CB->setEnabled(false);

    $from=$this->Range->From->Date;
    $to=$this->Range->To->Date;

    $src=$this->Range->srcip->Text;
    $dst=$this->Range->dstip->Text;

    $width=$this->AlertTS->width;
    $height=$this->AlertTS->height;

    $this->generateGraph($width,$height,$from,$to,$src,$dst);
  }

  private function constructUrl($width,$height,$from,$to,$src,$dst)
  {
    $linkdata=array( 'title' => 'Meta-alerts time series',
                     'query' => 'DMMetaAlertCountTimeSeries',
                     'width' => $width,
                     'severities' => 'all',
                     'height' => $height,
                     'from' => $from,
                     'to' => $to,
                     'src' => $src,
                     'dst' => $dst);

    return $this->getRequest()->constructUrl('graph', "MetaAlertTimeSeries", $linkdata, false);
  }

  private function generateGraph($width,$height,$from,$to,$src,$dst)
  {
    $this->AlertTS->ImageUrl = $this->constructUrl($width,$height,$from,$to,$src,$dst);
  }
}

?>
