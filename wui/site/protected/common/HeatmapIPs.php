<?php

class HeatmapIPs extends TTemplateControl
{
  public function onInit($param)
  {
    parent::onInit($param);
    $this->date_from='1970-01-01 00:00:00';
    $this->date_to=date("Y-m-d H:i:s");
  }

  public function renderControl($param)
  {
    parent::renderControl($param);

    $param=new CDMPair();
    $param->key=$this->date_from;
    $param->value=$this->date_to;

    $range=CSQLMap::get()->queryForObject("DMHeatMapHostcount",$param);
    if ($range->key!=null)
      $this->minAlert=$range->key;
    else
      $this->minAlert=0;

    if ($range->value!=null)
      $this->maxAlert=$range->value;
    else
      $this->maxAlert=0;

    $data=new HeatmapGetter($this->minAlert, $this->maxAlert);

    echo "<script language='JavaScript'>\n";
    echo "var minAlert=".$this->minAlert.";\n";;
    echo "var maxAlert=".$this->maxAlert.";\n";;
    echo "var sources = new Array(); \n";
    echo "var destinations = new Array();\n";

    $src_count=$data->getNumSources();

    for ($i=0; $i<$src_count; $i++)
      echo "sources[$i] = \"".$data->getSortedSource($i)."\"; ";

    echo "\n";

    $dst_count=$data->getNumDestinations();

    for ($i=0; $i<$dst_count; $i++)
      echo "destinations[$i] = \"".$data->getSortedDestination($i)."\"; ";

    echo "\n</script>\n";
  }

  private $maxAlert;
  private $minAlert;
  public $date_from;
  public $date_to;
}
?>