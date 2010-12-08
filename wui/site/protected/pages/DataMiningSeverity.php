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


    $from=$this->Range->From->Date;
    $to=$this->Range->To->Date;
    $severities=implode('.',$this->Range->CB->SelectedValues);
    $src=$this->Range->srcip->Text;
    $dst=$this->Range->dstip->Text;

    $width=$this->SeveritiesImg->width;
    $height=$this->SeveritiesImg->height;

    if ($src=='any' && $dst=='any')
      $query='DMSeverities';
    else
      $query='DMSeveritiesIP';

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

  private function generateGraph($width,$height,$query,$from,$to,$severities,$src,$dst)
  {
    $this->SeveritiesImg->ImageUrl = $this->constructUrl($width,$height,$query,$from,$to,$severities,$src,$dst);
  }


}

?>
