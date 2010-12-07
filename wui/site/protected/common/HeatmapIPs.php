<?php

class HeatmapIPs extends TTemplateControl
{
  public function onLoad()
  {
    $range=CSQLMap::get()->queryForObject("DMHeatMapHostcount");
    $this->minAlert=$range->key;
    $this->maxAlert=$range->value;
  }

  public function renderControl($param)
  {
    parent::renderControl($param);

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
}
?>