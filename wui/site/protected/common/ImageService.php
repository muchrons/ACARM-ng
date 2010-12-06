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
    $this->from= TPropertyValue::ensureInteger($this->getRequestOrThrow($request, 'from','You must specify the data range.'));
    $this->to= TPropertyValue::ensureInteger($this->getRequestOrThrow($request, 'to','You must specify the data range.'));
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

  private function createHeatMap()
  {
    $data=new HeatmapGetter($this->from, $this->to);

    $img_w=$this->width;
    $img_h=$this->height;

    if($data->getNumSources() < $img_w)
      $img_w=$data->getNumSources();

    if($data->getNumDestinations() < $img_h)
      $img_h=$data->getNumDestinations();

    $img = imagecreatetruecolor($img_w,$img_h);
    $sc = imagecolorallocate ($img, 0xFF, 0xFF, 0x00);

    for ($s=0; $s<$img_w; $s++)
      for ($d=0; $d<$img_h; $d++)
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
