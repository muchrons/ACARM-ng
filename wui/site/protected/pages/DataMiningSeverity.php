<?php

class DataMiningSeverity extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $g=new GdChecker(); //checks if GD is installed and available

    if ($this->SeveritiesImg->width == 0)
      $this->SeveritiesImg->width=700;
    if ($this->SeveritiesImg->height == 0)
      $this->SeveritiesImg->height=800;

    if (!$this->isPostBack)
      {
        $srcip=$this->Request->itemAt('srcip');
        if( $srcip!==null )
          $this->Range->srcip->Text=$srcip;

        $dstip=$this->Request->itemAt('dstip');
        if( $dstip!==null )
          $this->Range->dstip->Text=$dstip;

        $date_from=$this->Request->itemAt('from');
        if( $date_from!==null )
          $this->Range->From->Text=$date_from;

        $date_to=$this->Request->itemAt('to');
        if( $date_to!==null )
          $this->Range->To->Text=$date_to;
      }

    $this->Range->srcip->setEnabled(false);
    $this->Range->dstip->setEnabled(false);

    $from=$this->Range->From->Date;
    $to=$this->Range->To->Date;
    $severities=$this->Range->getRangeData()->severities;

    $src=$this->Range->srcip->Text;
    $dst=$this->Range->dstip->Text;

    $width=$this->SeveritiesImg->width;
    $height=$this->SeveritiesImg->height;

    if ($src=='any' && $dst=='any')
      $query='DMSeverities';
    else
      $query='DMSeveritiesIP';

    $this->setCallback($from,$to,$src,$dst,$severities);
    $this->generateGraph($width,$height,$query,$from,$to,$severities,$src,$dst);
  }

  private function constructUrl($width,$height,$query,$from,$to,$severities,$src,$dst)
  {
    $linkdata=array( 'title' => 'Alert count by severity type',
                     'width' => $width,
                     'height' => $height,
                     'query' => $query,
                     'from' => $from,
                     'to' => $to,
                     'severities' => $severities,
                     'src' => $src,
                     'dst' => $dst);

    return $this->getRequest()->constructUrl('graph', "SeverityPie", $linkdata, false);
  }

  private function constructUrlForCallbacks($type,$from,$to,$srcip,$dstip,$severities)
  {
    $linkdata=array( 'srcip' => $srcip,
                     'dstip' => $dstip,
                     'from' => $from,
                     'to' => $to,
                     'severities' => $severities);

    return $this->getRequest()->constructUrl('page', $type, $linkdata);
  }

  private function setCallback($from,$to,$srcip,$dstip,$severities)
  {
    $this->AlertsLink->NavigateUrl=$this->constructUrlForCallbacks("Alerts",$from,$to,$srcip,$dstip,$severities);
  }

  private function generateGraph($width,$height,$query,$from,$to,$severities,$src,$dst)
  {
    $this->SeveritiesImg->ImageUrl = $this->constructUrl($width,$height,$query,$from,$to,$severities,$src,$dst);
  }


}

?>
