<?php

class DataMiningSeverity extends TPage
{
  // TODO: fix brackets
  public function onLoad($param) {
    parent::onLoad($param);

    $this->generateGraph();
  }


  private function generateGraph()
  {
    $pairs=CSQLMap::get()->queryForList('DMSeverities');

    $ydata = array();
    $xdata = array();

    // TODO: fix indentation
    foreach( $pairs as $e )
      {
        $xdata[] = trim($e->key)." (%d)";
        $ydata[] = $e->value;
      }

    $ydata = implode(',', $ydata);
    $xdata = implode(',', $xdata);

    $this->SeveritiesImg->ImageUrl = $this->getRequest()->constructUrl('graph', "SeverityPie", array( 'xdata' => $xdata, 'ydata' => $ydata, 'ytitle' => 'title'), false);
  }

}


?>
