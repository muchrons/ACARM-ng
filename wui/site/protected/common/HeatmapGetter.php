<?php

class HeatmapGetter
{
  function __construct($from,$to)
  {
    $range=new CDMPair();
    $range->key=$from;
    $range->value=$to;
    $data=CSQLMap::get()->queryForList("DMHeatMapRange",$range);

    $this->src=0;
    $this->dst=0;

    $this->empty=($data[0]===null);

    //get arrays of sources and destinations
    foreach( $data as $e )
      {
        if ($sources[$e->source] === null)
          {
            $this->sources[$this->src]=$e->source;
            $sources[$e->source]=$this->src++;
          }
        if ($destinations[$e->destination] === null)
          {
            $this->destinations[$this->dst]=$e->destination;
            $destinations[$e->destination]=$this->dst++;
          }
      }

    //fill matrix with number of alerts between given source and destination

    $sum_alerts=0;
    for ($i=0; $i < $this->src; $i++)
      $this->img_data_maxSrc[$i]=new Pair($i,0);

    for ($i=0; $i < $this->dst; $i++)
      $this->img_data_maxDst[$i]=new Pair($i,0);

    $max_val=1;
    foreach( $data as $e )
      {
        $d=$destinations[$e->destination];
        $s=$sources[$e->source];

        $this->img_data[$d][$s]=$e->count;

        if ($e->count>$this->max_val)
          $this->max_val=$e->count;

        $this->img_data_maxSrc[$s]->second+=$e->count;
        $this->img_data_maxDst[$d]->second+=$e->count;
      }

    //sort columns and rows by total number of alerts per host
    if ($this->img_data_maxSrc!=null)
      usort($this->img_data_maxSrc, array("Pair", "compare_rev"));
    if ($this->img_data_maxDst!=null)
      usort($this->img_data_maxDst, array("Pair", "compare_rev"));
  }

  public function getImgData($d,$s)
  {
    return $this->img_data[$this->img_data_maxDst[$d]->first][$this->img_data_maxSrc[$s]->first];
  }

  public function getSortedSource($i)
  {
    return $this->sources[$this->img_data_maxSrc[$i]->first];
  }

  public function getSortedDestination($i)
  {
    return $this->destinations[$this->img_data_maxDst[$i]->first];
  }

  public function getNumSources()
  {
    return $this->src;
  }

  public function getNumDestinations()
  {
    return $this->dst;
  }

  public function getMaxVal()
  {
    return $this->max_val;
  }

  public function isEmpty()
  {
    return $this->empty;
  }

  private $dst;
  private $src;
  private $sources;
  private $destinations;
  private $img_data;
  private $img_data_maxDst;
  private $img_data_maxSrc;
  private $max_val;
  private $empty;
}

?>