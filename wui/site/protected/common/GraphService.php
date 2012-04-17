<?php

// TODO: 'jpgraph' should be moved somewhere to protected/ directory, where all the sources are (access to this dir is protected with .htaccess)
require_once('lib/jpgraph/jpgraph.php');
require_once('lib/jpgraph/jpgraph_bar.php');
require_once('lib/jpgraph/jpgraph_log.php');
require_once('lib/jpgraph/jpgraph_pie.php');
require_once('lib/jpgraph/jpgraph_pie3d.php');
require_once('lib/jpgraph/jpgraph_line.php');
require_once('lib/jpgraph/jpgraph_date.php');

function severityToName($sev)
{
  //$name=substr($sev,0,4); //strip "(%d)"

  switch ($sev)
    {
    case 4:
      return "high";
    case 3:
      return "medium";
    case 2:
      return "low";
    case 1:
      return "info";
    case 0:
      return "debug";
    }
  return "unknown";
}

function nameToSeverity($sev)
{
  $name=substr($sev,0,4); //strip "(%d)"

  switch ($name)
    {
    case "high":
      return 4;
    case "medi":
      return 3;
    case 'low':
      return 2;
    case 'info':
      return 1;
    case 'debu':
      return 0;
    }
  return -1;
}

function severityToColor($sev)
{
  //$name=substr($sev,0,4); //strip "(%d)"

  switch ($sev)
    {
    case 4:
      return "#ff0000";
    case 3:
      return "#ff8800";
    case 2:
      return "#ffee00";
    case 1:
      return "#00ff00";
    case 0:
      return "#2d88ff";
    }
  return "#cccccc";
}


class GraphParams
{
  function __construct($request)
  {
    $this->type  = TPropertyValue::ensureString($this->getRequestOrDefault($request, 'graph', 'You must specify the type of the graph'));
    $this->width = TPropertyValue::ensureInteger($this->getRequestOrDefault($request, 'width',  300));
    $this->height= TPropertyValue::ensureInteger($this->getRequestOrDefault($request, 'height', 200));
    $this->title = TPropertyValue::ensureString($this->getRequestOrDefault($request, 'title', 'No title'));
    $this->query = TPropertyValue::ensureString($this->getRequestOrDefault($request, 'query', null));

    if ($this->height>1200)
      $this->height=1200;
    if($this->width>1920)
      $this->width=1920;
    if ($this->height<0)
      $this->height=0;
    if($this->width<0)
      $this->width=0;

    $this->qparam=new CParamRange();
    $this->qparam->src=$this->getRequestOrDefault($request, 'src', null);
    $this->qparam->dst=$this->getRequestOrDefault($request, 'dst', null);

    if ($this->qparam->dst=='any')
    {
      $this->qparam->dst='0.0.0.0';
      $this->qparam->ignoredst=1;
    }
    else
      $this->qparam->ignoredst=0;

    if ($this->qparam->src=='any')
    {
      $this->qparam->src='0.0.0.0';
      $this->qparam->ignoresrc=1;
    }
    else
      $this->qparam->ignoresrc=0;
    $this->qparam->date_from=$this->getRequestOrDefault($request, 'from', '1970-01-01');
    $this->qparam->date_to=$this->getRequestOrDefault($request, 'to', date("Y-m-d H:i:s"));
    $this->qparam->severities=$this->getRequestOrDefault($request, 'severities', null);
    //set hour in date_to to 23:59:59 regardless of its actual value
    $this->qparam->date_to=date("Y-m-d 23:59:59",strtotime($this->qparam->date_to));
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

  public $type;  //type of the graph
  public $width;
  public $height;
  public $title;
  public $query; //DB query name
  public $qparam; //parameters for queries
}


class GraphService extends TService
{
  public function init($config)
  {
    $this->Response->ContentType="image/png";
    $request = Prado::getApplication()->getRequest();
    $this->params=new GraphParams($request);
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
    switch( $this->params->type )
    {
    case "AlertTimeSeries":
      $graph = $this->createAlertTimeSeries();
      break;
    case "SeverityPie":
      $graph = $this->createSeverityChart();
      break;
    case "MetaAlertTimeSeries":
      $graph = $this->createMetaAlertTimeSeries();
      break;
    default:
      $this->printError("Wrong graph type: ".$this->params->type);
      return;
    }
    $graph->Stroke();
  }

  private function printError($message)
  {
    $fontnum=2;
    $img = imagecreatetruecolor($this->params->width,$this->params->height);
    $yellow = imagecolorallocate ($img, 0xFF, 0xFF, 0x00);
    $red = imagecolorallocate ($img, 0xFF, 0x00, 0x00);
    $fontw=imagefontwidth($fontnum);
    assert($fontw!=0);
    $numchars=(($this->params->width-5)/$fontw);
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

  private function issueQuery2d($param)
  {
    try
    {
      $pairs=SQLWrapper::queryForList($param->query,$param->qparam);
    }
    catch(Exception $e)
    {
      $this->printError($e->getMessage());
      return;
    }

    if (count($pairs) == 0)
      return null;

    foreach( $pairs as $e )
    {
      $xdata[] = trim($e->key);
      $ydata[] = $e->value;
    }

    return array($xdata, $ydata);
  }

  private function issueQuery2dTime($param)
  {
    try
    {
      $pairs=SQLWrapper::queryForList($param->query,$param->qparam);
    }
    catch(Exception $e)
    {
      $this->printError($e->getMessage());
      return;
    }

    $cnt=count($pairs);
    if ($cnt == 0)
      return null;

    $groupby=floor($cnt/168);
    if ($groupby==0)
      $groupby=1;

    $div=floor($cnt/$groupby);
    $mod=$cnt%$groupby;

    $xdata=array();
    $Hdata=array();
    $Mdata=array();
    $Ldata=array();
    $Idata=array();
    $Ddata=array();

    for($i=$mod; $i<$cnt; $i+=$groupby)
    {
      $idx=floor(($i-$mod)/$groupby);
      $xdata[$idx] = strtotime($pairs[$i-$mod]->create_time);
      $Hdata[$idx] = $pairs[$i]->high;
      $Mdata[$idx] = $pairs[$i]->medium;
      $Ldata[$idx] = $pairs[$i]->low;
      $Idata[$idx] = $pairs[$i]->info;
      $Ddata[$idx] = $pairs[$i]->debug;

      for($j=1; $j<$groupby; $j++)
        {
          $Hdata[$idx] += $pairs[$i+$j]->high;
          $Mdata[$idx] += $pairs[$i+$j]->medium;
          $Ldata[$idx] += $pairs[$i+$j]->low;
          $Idata[$idx] += $pairs[$i+$j]->info;
          $Ddata[$idx] += $pairs[$i+$j]->debug;
        }
      $Hdata[$idx]/=$groupby;
      $Mdata[$idx]/=$groupby;
      $Ldata[$idx]/=$groupby;
      $Idata[$idx]/=$groupby;
      $Ddata[$idx]/=$groupby;
    }
    $names=array("high","medium","low","info","debug");
    return array($names, $xdata, $Hdata, $Mdata, $Ldata, $Idata, $Ddata);
  }

  private function issueQuery2dTime2($param)
  {
    try
    {
      $pairs=SQLWrapper::queryForList($param->query,$param->qparam);
    }
    catch(Exception $e)
    {
      $this->printError($e->getMessage());
      return;
    }

    if (count($pairs) == 0)
      return null;

    foreach( $pairs as $e )
    {
      $xdata[] = strtotime($e->key);
      $ydata[] = ($e->value === null)?0:$e->value;
    }

    return array($xdata, $ydata, $param->qparam->severities);
  }


  private function createSeverityChart()
  {
    $data=$this->issueQuery2d($this->params);

    if ($data === null)
    {
      $data[0]=array(0);
      $data[1]=array(1);
      $empty=true;
    }
    else
      $empty=false; //mark plot as empty for color proper color selection

    // Create the Pie Graph.
    $graph = new PieGraph($this->params->width,$this->params->height,'auto');
    // Create Plot
    $p1 = new PiePlot3D($data[1]);
    $graph->Add($p1);


    $severities=array_map("severityToName",$data[0]);
    $p1->SetLabelType(PIE_VALUE_PER);
    $p1->value->SetFont(FF_ARIAL,FS_NORMAL,13);
    $p1->value->SetFormat("%d");
    $p1->SetLabelType(1);

    $colors=array_map("severityToColor",$data[0]);

    if ($empty)
      {
        $colors=array('#aaaaaa');
        $p1->value->Show(false);
      }


    $p1->SetSize(0.50);
    $p1->SetSliceColors($colors);
    //$p1->SetGuideLines();
    //$p1->SetGuideLinesAdjust(1.4);


    // Set different title for an empty plot
    if ($empty)
      $graph->title->Set("No data matching your criteria");
    else
      {
        $p1->SetLegends($severities);
        $graph->title->Set($this->params->title);
      }

    $graph->title->SetMargin(4);
    $graph->title->SetFont(FF_VERDANA,FS_BOLD,12);
    $graph->title->SetColor("darkred");

    $graph->legend->SetPos(0.5,0.05,'center','top');
    $graph->legend->SetColumns(5);

    $graph->SetAntiAliasing();

    return $graph;
  }

  private function createAlertTimeSeries()
  {
    $graph = new Graph($this->params->width,$this->params->height);
    $graph->title->Set($this->params->title);
    $data=$this->issueQuery2dTime($this->params);

    if($data[1]===null)
      $tickType='TimeCallbackNull';
    else
      if ((end($data[1])-$data[1][0])>(7*24*3600))
        $tickType='TimeCallbackDays';
      else
        $tickType='TimeCallbackHours';

    $severities=$this->params->qparam->severities;

    $types=count($data[0]);
    $line=array();
    $maxval=0;

    for ($i=0; $i<$types; $i++)
      {
        if (strpos($severities,$data[0][$i])===false)
          continue; //if we want to skip this severity

        $type=$i+2; //skip labels and x axis
        $maxval+=max($data[$type]);

        $line[] = new LinePlot($data[$type],$data[1]);
        end($line)->setLegend($data[0][$i]);
        end($line)->SetFillColor(severityToColor(nameToSeverity($data[0][$i])));
      }

    if(count($line)==0)
    {
      $line[0] = new LinePlot(array(0),array(1));
      $line[0]->setLegend("no data for given query");
      $empty=true;
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

  private function createMetaAlertTimeSeries()
  {
    $graph = new Graph($this->params->width,$this->params->height);
    $graph->title->Set($this->params->title);

    $severities=explode(".",$this->params->qparam->severities);

    $params=$this->params;

    $diffF=strtotime($params->qparam->date_from);
    $diffT=strtotime($params->qparam->date_to);

    $params->qparam->extra='hour';

    $data=array();

    foreach ($severities as $s)
    {
      $params->qparam->severities=$s;
      $d=$this->issueQuery2dTime2($params);
      if ($d != null)
      {
        $data[0][]=$d;
        $data[1][]=$s;
      }
    }

    $count=0;

    if (isset($data[0]))
      $count=count($data[0]);

    //if there is no data series or x-axis is empty (wrong data range)
    if (($count==0) || ($count>0 && count($data[0][0][1])==0))
    {
      $data[0][0][1]=array(1);
      $data[0][0][0]=array(1);
      $data[1][0]="no data for given query";
      $count=1;
    }


    $maxval=0;
    for ($i=0; $i<$count; $i++)
      $maxval+=max($data[0][$i][1]);

    $graph->SetScale('datlin',0,$maxval);
    $graph->xaxis->SetLabelAngle(60);

    for ($i=0; $i<$count; $i++)
    {
      if (count($data[0][$i][1])==0)
        continue;
      $line[] = new LinePlot($data[0][$i][1],$data[0][$i][0]);
      end($line)->setLegend($data[1][$i]);
      end($line)->SetFillColor(severityToColor(nameToSeverity($data[1][$i])));
    }
    $graph->legend->SetPos(0.5,0.05,'center','top');
    $graph->legend->SetColumns(5);
    $graph->SetTickDensity( TICKD_DENSE, TICKD_SPARSE );
    $graph->xaxis->SetLabelFormatCallback('TimeCallbackHours');
    $graph->xgrid->SetColor('gray');
    $graph->xgrid->Show();
    $accplot = new AccLinePlot($line);
    $graph->Add($accplot);
    return $graph;
  }

  private $params;
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

?>
