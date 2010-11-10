<?php

class DataMiningAlertTimeSeries extends TPage
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

    $from=$this->Range->From->Date;
    $to=$this->Range->To->Date;
    $severities=implode('.',$this->Range->CB->SelectedValues);
    $src=$this->Range->srcip->Text;
    $dst=$this->Range->dstip->Text;

    $width=$this->AlertTS->width;
    $height=$this->AlertTS->height;

    $this->generateGraph($width,$height,$from,$to,$severities,$src,$dst);
  }

  private function constructUrl($width,$height,$from,$to,$severities,$src,$dst)
  {
    $linkdata=array( 'title' => 'Alerts time series',
                     'query' => 'DMAlertCountTimeSeries',
                     'width' => $width,
                     'height' => $height,
                     'from' => $from,
                     'to' => $to,
                     'severities' => $severities,
                     'src' => $src,
                     'dst' => $dst);

    return $this->getRequest()->constructUrl('graph', "AlertTimeSeries", $linkdata, false);
  }

  private function generateGraph($width,$height,$from,$to,$severities,$src,$dst)
  {
    $this->AlertTS->ImageUrl = $this->constructUrl($width,$height,$from,$to,$severities,$src,$dst);
  }
}

?>
