<?php
ini_set("memory_limit","256M"); //override default memory limit

class ImageService extends TService
{
  public function init($config)
  {
    $this->Response->ContentType="image/png";

    $request = Prado::getApplication()->getRequest();

    $this->width = TPropertyValue::ensureInteger($this->getRequestOrThrow($request, 'width',  'You must specify the width of the heatmap'));
    $this->height= TPropertyValue::ensureInteger($this->getRequestOrThrow($request, 'height', 'You must specify the height of the heatmap'));
    $this->from= TPropertyValue::ensureInteger($this->getRequestOrDefault($request, 'from',0));
    $this->to= TPropertyValue::ensureInteger($this->getRequestOrDefault($request, 'to',0));
  }

  private function getRequestOrDefault($request, $field, $value)
  {
    if ($request->contains($field))
      return $request[$field];
    return $value;
  }

  private function getRequestOrThrow($request, $field, $text)
  {
    if (!$request->contains($field))
      throw new TConfigurationException($text);
    return $request[$field];
  }

  public function getWidth()
  {
    return $this->width;
  }

  public function setWidth($value)
  {
    $this->width = TPropertyValue::ensureInteger($value);
  }

  public function getHeight()
  {
    return $this->height;
  }

  public function setHeight($value)
  {
    $this->height = TPropertyValue::ensureInteger($value);
  }

  public function run()
  {
    $img = $this->createHeatMap();
    imagepng($img);
    imagedestroy($img);
  }

  private function issueQuery2d($from, $to)
  {
    if ($from==0 && $to==0)
      return CSQLMap::get()->queryForList("DMHeatMap");

    $range=new CDMPair();
    $range->key=$from;
    $range->value=$to;
    return CSQLMap::get()->queryForList("DMHeatMapRange",$range);
  }

  private function createHeatMap()
  {
    //get data
    $data=$this->issueQuery2d($this->from, $this->to);

    $src=0;
    $dst=0;

    //get arrays of sources and destinations
    foreach( $data as $e )
      {
        if ($sources[$e->source] === null)
          $sources[$e->source]=$src++;
        if ($destinations[$e->destination] === null)
          $destinations[$e->destination]=$dst++;
      }

    //fill matrix with number of alerts between given source and destination
    $max_val=1;
    foreach( $data as $e )
      {
        $val=$img_data[$destinations[$e->destination]][$sources[$e->source]]=$e->count;
        if ($val>$max_val)
          $max_val=$val;
      }

    //sort columns and rows by total number of alerts per host
    $sum_alerts=0;
    for ($i=0; $i<$src; $i++)
      $img_data_maxSrc[$i]=new Pair($i,0);

    for ($i=0; $i<$dst; $i++)
      $img_data_maxDst[$i]=new Pair($i,0);

    for ($i=0; $i<$src; $i++)
      for ($j=0; $j<$dst; $j++)
        {
          $img_data_maxSrc[$i]->second+=$img_data[$j][$i];
          $img_data_maxDst[$j]->second+=$img_data[$j][$i];
          $sum_alerts+=$img_data[$j][$i];
        }

    usort($img_data_maxSrc, array("Pair", "compare_rev"));
    usort($img_data_maxDst, array("Pair", "compare_rev"));

    //TODO show only n% of interesting data

    $img_w=$this->width;
    $img_h=$this->height;

    if($src<$img_w)
      $img_w=$src;

    if($dst<$img_h)
      $img_h=$dst;

    $img = imagecreatetruecolor($img_w,$img_h);
    $sc = imagecolorallocate ($img, 0xFF, 0xFF, 0x00);

    for ($i=0; $i<$img_w; $i++)
      for ($j=0; $j<$img_h; $j++)
        imagesetpixel($img,$i,$j,$this->color_map($img,$img_data[$img_data_maxDst[$j]->first][$img_data_maxSrc[$i]->first],$max_val));

    $f=$img_data_maxDst[1]->first;

    //Print caption only if the image is big enough
    if($img_w>200 && $img_h>40)
      imagestring ($img, 2, 10, $img_h-30,  "Max alerts: $max_val", $sc);
    return $img;
  }

  private function color_map($img,$number,$max)
  {
    if ($number>10000)
      return imagecolorallocate($img,255,255,255);
    if ($number>1000)
      return imagecolorallocate($img,230,0,0);
    if ($number>100)
      return imagecolorallocate($img,200,0,0);
    if ($number>10)
      return imagecolorallocate($img,200,100,0);
    if ($number>0)
      return imagecolorallocate($img,0,255,0);
    return imagecolorallocate($img,0,0,0);
  }

  private $width;
  private $height;
  private $from;
  private $to;
}

?>
