<?php

class DataMiningTimeSeries extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    if (!$this->isPostBack)
      {
        $this->type=$this->Request->itemAt('type');
        if ($this->type===null)
          $this->type="Alert";
        if($this->type!="Alert" && $this->type!="MetaAlert")
          die("Wrong type of time series");
        $this->Range->setSev($this->type);
      }
    $this->type=$this->Range->getSev();
    $g=new GdChecker(); //checks if GD is installed and available

    if ($this->TS->width == 0)
      $this->TS->width=700;
    if ($this->TS->height == 0)
      $this->TS->height=800;

    $this->Range->srcip->setEnabled(false);
    $this->Range->dstip->setEnabled(false);

    $from=$this->Range->FromDate();
    $to=$this->Range->ToDate();

    $severities=$this->Range->getRangeData()->severities;
    $src=$this->Range->srcip->Text;
    $dst=$this->Range->dstip->Text;

    $width=$this->TS->width;
    $height=$this->TS->height;

    $this->generateGraph($width,$height,$from,$to,$severities,$src,$dst);
  }

  private function constructUrl($width,$height,$from,$to,$severities,$src,$dst)
  {
    if($this->type=="Alert")
      {
        $title='Alerts time series';
        $query='DMAlertCountTimeSeries';
      }
    else
      {
        $title='Meta-alerts time series';
        $query='DMMetaAlertCountTimeSeries';
      }

    $linkdata=array( 'title' => $title,
                     'query' => $query,
                     'width' => $width,
                     'height' => $height,
                     'from' => $from,
                     'to' => $to,
                     'severities' => $severities,
                     'src' => $src,
                     'dst' => $dst);

    return $this->getRequest()->constructUrl('graph', "TimeSeries", $linkdata, false);
  }

  private function generateGraph($width,$height,$from,$to,$severities,$src,$dst)
  {
    $this->TS->ImageUrl = $this->constructUrl($width,$height,$from,$to,$severities,$src,$dst);
  }
  private $type;
}

?>
