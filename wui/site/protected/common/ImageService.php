<?php
ini_set("memory_limit","256M"); //override default memory limit
error_reporting(0); //suppress error message of execution time exceeded in order not to corrupt image

function shutdown_func()
{
    Header("Content-Type: image/png");
    $aa=error_get_last();
    $bb=stripos($aa["message"],"Maximum execution time");
    
    if ($bb===false)
        $message=$aa["message"];
      else
        $message="PHP execution time limit exceeded. Please choose smaller range, increase the execution time in php.ini or use faster hardware.";

    printError($message,700,400);
}

register_shutdown_function("shutdown_func");

function printError($message,$width,$height)
{
  $fontnum=2;

  if ($height>1200)
    $height=1200;
  if($width>1920)
    $width=1920;

  $img = imagecreatetruecolor($width,$height);
  $yellow = imagecolorallocate ($img, 0xFF, 0xFF, 0x00);
  $red = imagecolorallocate ($img, 0xFF, 0x00, 0x00);
  $fontw=imagefontwidth($fontnum);
  assert($fontw!=0);
  $numchars=((600-5)/$fontw);
  $msg=explode("\n",wordwrap($message,$numchars,"\n")); //wrap long messages

  imagestring($img, 3, 3, 3, "Exception has been thrown:", $red);

  $hpos=imagefontwidth($fontnum)*4;
  foreach ($msg as $m)
    {
      imagestring ($img, $fontnum, 5, $hpos, $m, $yellow);
      $hpos+=imagefontwidth($fontnum)*2;
    }

  imagepng($img);
  imagedestroy($img);
}


class ImageService extends TService
{
  public function init($config)
  {
    $this->Response->ContentType="image/png";

    $request = Prado::getApplication()->getRequest();
    try
    {
      $this->width = TPropertyValue::ensureInteger($this->getRequestOrThrow($request, 'width',  'You must specify the width of the heatmap'));
      $this->height= TPropertyValue::ensureInteger($this->getRequestOrThrow($request, 'height', 'You must specify the height of the heatmap'));
      $this->from= TPropertyValue::ensureInteger($this->getRequestOrThrow($request, 'from','You must specify the data range.'));
      $this->to= TPropertyValue::ensureInteger($this->getRequestOrThrow($request, 'to','You must specify the data range.'));
    }
    catch(Exception $e)
    {
      $width=($this->width===null)?600:$this->width;
      $height=($this->height===null)?600:$this->height;

      printError($e->getMessage(),$width,$height);
      return;
    }

    if ($this->height>1200)
      $this->height=1200;
    if($this->width>1920)
      $this->width=1920;
    if ($this->height<0)
      $this->height=0;
    if($this->width<0)
      $this->width=0;
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
    try
    {
      $img = $this->createHeatMap();
      imagepng($img);
      imagedestroy($img);
    }
    catch(Exception $e)
    {
      printError($e->getMessage(),$this->width,$this->height);
    }
  }

  private function createHeatMap()
  {
    $data=new HeatmapGetter($this->from, $this->to);

    if ($data->isEmpty())
      throw new Exception("Your query returned no data. The data range is invalid or the database is empty.");

    $img_w=$this->width;
    $img_h=$this->height;

    $img = imagecreatetruecolor($img_w,$img_h);
    $sc = imagecolorallocate ($img, 0xFF, 0xFF, 0x00);

    for ($s=0; $s<$data->getNumSources(); $s++)
      for ($d=0; $d<$data->getNumDestinations(); $d++)
        imagesetpixel($img,$s,$d,$this->color_map($img,$data->getImgData($d,$s),$data->getMaxVal()));

    //Print caption only if the image is big enough
    if($img_w>500 && $img_h>40)
      {
        imagestring ($img, 2, $img_w-100, $img_h-15,  "<- Sources ->", $sc);
        imagestringup ($img, 2, $img_w-15, $img_h-25,  "<- Destinations ->", $sc);
      }

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
