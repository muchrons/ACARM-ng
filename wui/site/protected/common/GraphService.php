<?php

// TODO: 'jpgraph' should be moved somewhere to protected/ directory, where all the sources are (access to this dir is protected with .htaccess)
require_once('lib/jpgraph/jpgraph.php');
require_once('lib/jpgraph/jpgraph_bar.php');
require_once('lib/jpgraph/jpgraph_log.php');
require_once('lib/jpgraph/jpgraph_pie.php');
require_once('lib/jpgraph/jpgraph_pie3d.php');
require_once('lib/jpgraph/jpgraph_line.php');
require_once('lib/jpgraph/jpgraph_date.php');

function severityNameToColor($sev)
{
  $name=substr($sev,0,4);

  switch ($name)
    {
    case 'root':
    case 'high':
      return "#ff0000";
    case 'medi':
      return "#ff8800";
    case 'all':
    case 'low':
      return "#ffee00";
    case 'info':
      return "#00ff00";
    case 'debu':
      return "#2d88ff";
    }
  return "#cccccc";
}

function TimeCallbackHours($aVal)
{
  return  Date ( 'd-m-y H:i' , $aVal );
}

function TimeCallbackDays($aVal)
{
  return  Date ( 'd-m-y' , $aVal );
}

function TimeCallbackNull($aVal)
{
  return  "";
}

function getRequestOrDefault($request, $field, $value)
{
  if ($request->contains($field))
    return $request[$field];
  return $value;
}

function getRequestOrThrow($request, $field, $text)
{
  if (!$request->contains($field))
    throw new TConfigurationException($text);
  return $request[$field];
}

function getCParamRange($request)
{
  $qparam=new CParamRange();
  $qparam->src=getRequestOrDefault($request, 'src', null);
  $qparam->dst=getRequestOrDefault($request, 'dst', null);

  if ($qparam->dst=='any')
    {
      $qparam->dst='0.0.0.0';
      $qparam->ignoredst=1;
    }
    else
      $qparam->ignoredst=0;

    if ($qparam->src=='any')
    {
      $qparam->src='0.0.0.0';
      $qparam->ignoresrc=1;
    }
    else
      $qparam->ignoresrc=0;

    $qparam->date_from=getRequestOrDefault($request, 'from', '1970-01-01');
    $qparam->date_to=getRequestOrDefault($request, 'to', date("Y-m-d H:i:s"));
    $qparam->severities=getRequestOrDefault($request, 'severities', null);
    //set hour in date_to to 23:59:59 regardless of its actual value
    $qparam->date_to=date("Y-m-d 23:59:59",strtotime($qparam->date_to));
    return $qparam;
}

class GraphService extends TService
{
  public function init($config)
  {
    $this->Response->ContentType="image/png";
    $request = Prado::getApplication()->getRequest();

    $width = TPropertyValue::ensureInteger(getRequestOrDefault($request, 'width',  300));
    $height= TPropertyValue::ensureInteger(getRequestOrDefault($request, 'height', 200));

    if($height>1200) $height=1200;
    if($width>1920)  $width=1920;
    if($height<0)    $height=0;
    if($width<0)     $width=0;

    $this->setWidth($width);
    $this->setHeight($height);

    $title = TPropertyValue::ensureString(getRequestOrDefault($request, 'title', 'No title'));
    $type  = TPropertyValue::ensureString(getRequestOrDefault($request, 'graph', 'You must specify the type of the graph'));

    $this->setTitle($title);
    $this->setType($type);

    $query = TPropertyValue::ensureString(getRequestOrDefault($request, 'query', null));
    $this->setQuery($query);

    $queryParams=getCParamRange($request);
    $this->setQueryParams($queryParams);
  }

  public function run()
  {
    switch($this->getType())
    {
    case "TimeSeries":
      $graph = $this->createTimeSeries();
      break;
    case "SeverityPie":
      $graph = $this->createSeverityChart();
      break;
    default:
      $this->printError("Wrong graph type: ".$this->getType());
      return;
    }
    $graph->Stroke();
  }

  private function getType()
  {
    return $this->type;
  }

  private function setType($type)
  {
    $this->type=$type;
  }

  private function getQuery()
  {
    return $this->query;
  }

  private function setQuery($query)
  {
    $this->query=$query;
  }

  private function getTitle()
  {
    return $this->title;
  }

  private function setTitle($title)
  {
    $this->title=$title;
  }

  private function getQueryParams()
  {
    return $this->queryParams;
  }

  private function setQueryParams($queryParams)
  {
    $this->queryParams=$queryParams;
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

  private function printError($message)
  {
    $fontnum=2;
    $img = imagecreatetruecolor($this->getWidth(),$this->getHeight());
    $yellow = imagecolorallocate ($img, 0xFF, 0xFF, 0x00);
    $red = imagecolorallocate ($img, 0xFF, 0x00, 0x00);
    $fontw=imagefontwidth($fontnum);
    assert($fontw!=0);
    $numchars=(($this->getWidth()-5)/$fontw);
    $msg=explode("\n",wordwrap($message,$numchars,"\n")); //wrap long messages

    imagestring($img, 3, 3, 3, "Exception was thrown:", $red);

    $hpos=imagefontwidth($fontnum)*4;
    foreach ($msg as $m)
    {
      imagestring ($img, $fontnum, 5, $hpos, $m, $yellow);
      $hpos+=imagefontwidth($fontnum)*2;
    }

    imagepng($img);
    imagedestroy($img);
  }

  private function issueQuery2d()
  {
    try
      {
        $pairs=SQLWrapper::queryForObject($this->getQuery(),$this->getQueryParams());
      }
    catch(Exception $e)
      {
        $this->printError($e->getMessage());
        return;
      }

    $names=array("high","medium","low","info","debug");
    $ydata[0] = $pairs->high===null?0:$pairs->high;
    $ydata[1] = $pairs->medium===null?0:$pairs->medium;
    $ydata[2] = $pairs->low===null?0:$pairs->low;
    $ydata[3] = $pairs->info===null?0:$pairs->info;
    $ydata[4] = $pairs->debug===null?0:$pairs->debug;
    return array($names, $ydata);
  }

  private function issueQuery2dTime()
  {
    try
    {
      $pairs=SQLWrapper::queryForList($this->getQuery(),$this->getQueryParams());
    }
    catch(Exception $e)
    {
      $this->printError($e->getMessage());
      return;
    }

    $cnt=count($pairs);
    if ($cnt == 0)
      return null;

    $xdata=array();
    $ydata=array();

    $names=array();
    foreach($pairs[0] as $a=>$b)
      {
        if ($a=="create_time")
          continue;
        $names[]=$a;
        $ydata[$a]=array();
      }

    $result[]=$pairs[0];
    $last_result=strtotime($pairs[0]->create_time);

    for($i=1; $i<$cnt; $i++)
      {
        $min=strtotime($pairs[$i]->create_time);

        if(($min-$last_result)>3600)
          for ($j=($last_result+3600); $j<$min; $j+=3600)
            {
              $tmp=clone $pairs[0];
              $tmp->create_time=date('Y-m-d H:00:00',$j);
              foreach($names as $name)
                $tmp->$name=0;
              $result[]=$tmp;
            }
        $result[]=$pairs[$i];
        $last_result=strtotime($pairs[$i]->create_time);
      }

    $pairs=$result;

    $cnt=count($pairs);

    $groupby=floor($cnt/168);
    if ($groupby==0)
      $groupby=1;

    $div=floor($cnt/$groupby);
    $mod=$cnt%$groupby;


    for($i=$mod; $i<$cnt; $i+=$groupby)
    {
      $idx=floor(($i-$mod)/$groupby);
      $xdata[$idx] = strtotime($pairs[$i-$mod]->create_time);

      foreach($names as $name)
        $ydata[$name][$idx]=$pairs[$i]->$name;

      for($j=1; $j<$groupby; $j++)
        foreach($names as $name)
          $ydata[$name][$idx]=$pairs[$i+$j]->$name;

      foreach($names as $name)
        $ydata[$name][$idx]/=$groupby;
    }

    return array_merge(array($names, $xdata), $ydata);
  }

  private function createSeverityChart()
  {
    $raw_data=$this->issueQuery2d();
    $severities=$this->getQueryParams()->severities;

    $data=array();
    $data[0]=array();
    $data[1]=array();

    for ($i=0; $i<count($raw_data[0]); $i++)
      {
        if (strpos($severities,$raw_data[0][$i])===false)
          continue;
        $data[0][]=$raw_data[0][$i];
        $data[1][]=$raw_data[1][$i];
      }

    if (array_sum($data[1])==0)
      {
        $data[0]=array(0);
        $data[1]=array(1);
        $empty=true;
      }
    else
      $empty=false; //mark plot as empty for color proper color selection

    // Create the Pie Graph.
    $graph = new PieGraph($this->getWidth(),$this->getHeight(),'auto');
    // Create Plot
    $p1 = new PiePlot3D($data[1]);
    $graph->Add($p1);
    $graph->SetShadow();

    $p1->SetLabelType(PIE_VALUE_PER);
    $p1->value->SetFont(FF_ARIAL,FS_NORMAL,13);
    $p1->value->SetFormat("%d");
    $p1->SetLabelType(1);
    $colors=array_map("severityNameToColor",$data[0]);

    if ($empty)
      {
        $colors=array('#aaaaaa');
        $p1->value->Show(false);
      }
    $p1->SetSliceColors($colors);

    // Set different title for an empty plot
    if ($empty)
      $graph->title->Set("No data matching your criteria");
    else
      {
        $p1->SetLegends($data[0]);
        $graph->title->Set($this->getTitle());
      }

    $graph->title->SetMargin(4);
    $graph->SetMargin(60,10,0,0);
    $graph->legend->SetPos(0.5,0.05,'center','top');
    $graph->legend->SetColumns(5);

    $graph->SetAntiAliasing();
    return $graph;
  }

  private function createTimeSeries()
  {
    $graph = new Graph($this->getWidth(),$this->getHeight());
    $graph->title->Set($this->getTitle());
    $data=$this->issueQuery2dTime();


    if($data[1]===null)
      $tickType='TimeCallbackNull';
    else
      if ((end($data[1])-$data[1][0])>(7*24*3600))
        $tickType='TimeCallbackDays';
      else
        $tickType='TimeCallbackHours';

    $severities=$this->getQueryParams()->severities;

    $line=array();
    $maxval=0;

    if($data!=null)
      foreach($data[0] as $name)
        {
          if (strpos($severities,$name)===false)
            continue; //if we want to skip this severity
          $maxval+=max($data[$name]);
          $line[] = new LinePlot($data[$name],$data[1]);
          end($line)->setLegend($name);
          end($line)->SetFillColor(severityNameToColor($name));
        }
    if(count($line)==0)
      {
        $line[0] = new LinePlot(array(0),array(1));
        $line[0]->setLegend("no data for given query");
      }

    $graph->SetScale('datlin',0,$maxval);
    $graph->xaxis->SetLabelAngle(60);

    $graph->legend->SetPos(0.5,0.05,'center','top');
    $graph->SetMargin(60,10,0,0);
    $graph->legend->SetColumns(5);
    $graph->SetTickDensity( TICKD_DENSE, TICKD_SPARSE );

    $graph->xaxis->SetLabelFormatCallback($tickType);
    $graph->yaxis->title->Set("Average number of alerts per hour.");
    $graph->yaxis->title->SetMargin(16);
    $graph->yaxis->title->SetFont(FF_ARIAL,FS_NORMAL,10);

    $graph->xgrid->SetColor('gray');
    $graph->xgrid->Show();
    $accplot = new AccLinePlot($line);
    $graph->Add($accplot);
    return $graph;
  }

  private $title;
  private $type;
  private $query;
  private $queryParams;
}

?>
