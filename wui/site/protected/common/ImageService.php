<?php
ini_set("memory_limit","256M"); //override default memory limit

class Pair
{
  public function __construct($f,$s)
  {
    $this->first=$f;
    $this->second=$s;
  }

  public static function compare_rev($a, $b)
  {
    if ($a->second > $b->second) return -1;
    else if($a->second == $b->second) return 0;
    else return 1;
  }

  public $first;
  public $second;
};

class ImageService extends TService
{
  public function init($config)
  {
    $request = Prado::getApplication()->getRequest();
    $this->Response->ContentType="image/png";   // TODO: move this line one line up - it interleaves assign/check sequence for $request

    if (!$request->contains('width'))
      throw new TConfigurationException('You must specify the width of the graph');
    $this->width = TPropertyValue::ensureInteger($request['width']);

    if (!$request->contains('height'))
      throw new TConfigurationException('You must specify the height of the graph');
    $this->height = TPropertyValue::ensureInteger($request['height']);

    if ($request->contains('title'))
      $this->title = TPropertyValue::ensureString($request['title']);
    else
      $this->title="No title";

    if ($request->contains('query'))
      $this->query = TPropertyValue::ensureString($request['query']);
    else
      $query=null;
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
    $img = $this->createHeatMap($this->query,$this->title);
    imagepng($img);
    imagedestroy($img);
    //$this->sort_test();
  }

  private function issueQuery2d()
  {
    $data=CSQLMap::get()->queryForList("DMHeatMap");
    return $data;
  }

  private function createHeatMap($q)
  {
    //get data
    $data=$this->issueQuery2d();

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

    //imagestring ($img, 2, 10, $img_h-30,  "Max alerts: $max_val", $sc);
    //imagestring ($img, 2, 10, $img_h-50,  "-$f-", $sc);

    return $img;
  }

  private function sort_test()
  {
    $img_data_maxSrc[0]=5;
    $img_data_maxSrc[1]=3;
    $img_data_maxSrc[2]=7;
    $img_data_maxSrc[3]=1;

    asort($img_data_maxSrc);

    foreach ($img_data_maxSrc as $key => $val) {
      echo "$key = $val<br>";
    }
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

  private $query = null;
  private $width;
  private $height;
  private $title;
}

?>
