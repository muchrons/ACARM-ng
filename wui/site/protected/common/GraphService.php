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

    $qparam=new CParamRange();

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
    case "AlertTypes":
      $graph = $this->createAlertTypeChart();
      break;
    case "MetaAlertTimeSeries":
      $graph = $this->createAlertTimeSeries();
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

    // Create Plot
    $p1 = new PiePlot3D($data[1]);
    $severities=array_map("severityToName",$data[0]);
    $p1->SetLabels(array_map(function($i) {return $i." (%d)";}, $severities));
    $p1->SetLabelPos(1);
    $p1->SetLabelType(PIE_VALUE_PER);
    $p1->value->SetFont(FF_ARIAL,FS_NORMAL,12);
    $p1->value->SetFormat("%d");
    $p1->SetLabelType(1);

    $colors=array_map("severityToColor",$data[0]);

    if ($empty)
      {
        $colors=array('#aaaaaa');
        $p1->value->Show(false);
      }

    $p1->SetSliceColors($colors);
    $p1->SetSize(0.50);

    // Create the Pie Graph.
    $graph = new PieGraph($this->params->width,$this->params->height,'auto');

    // Set different title for an empty plot
    if ($empty)
      $graph->title->Set("No data matching your criteria");
    else
      $graph->title->Set($this->params->title);

    $graph->title->SetMargin(8);
    $graph->title->SetFont(FF_VERDANA,FS_BOLD,12);
    $graph->title->SetColor("darkred");
    $graph->SetAntiAliasing();
    $graph->Add($p1);
    return $graph;
  }

  private function createAlertTypeChart()
  {
    // Setup the graph.
    $graph = new Graph($this->params->width,$this->params->height);
    $graph->img->SetMargin(60,55,95,95);
    $graph->SetScale("textlog");
    $graph->SetAngle(90);
    $graph->SetMarginColor("white");

    // Set up the title for the graph
    $graph->title->Set("Alert count by alert type (log scale)");
    $graph->title->SetMargin(8);
    $graph->title->SetFont(FF_VERDANA,FS_BOLD,12);
    $graph->title->SetColor("darkred");

    // Setup font for axis
    $graph->xaxis->SetFont(FF_VERDANA,FS_NORMAL,10);
    $graph->yaxis->SetFont(FF_VERDANA,FS_NORMAL,10);

    // Show 0 label on Y-axis (default is not to show)
    $graph->yscale->ticks->SupressZeroLabel(false);

    $data=$this->issueQuery2d($this->params);

    if ($data === null)
    {
      $data[0]=array("No data for given query, or empty database.");
      $data[1]=array(1);
    }

    // Setup X-axis labels
    $graph->xaxis->SetTickLabels($data[0]);
    $graph->xaxis->SetLabelAngle(0);
    $graph->xaxis->SetTitleMargin(0);
    $graph->xaxis->SetLabelMargin(-50);
    $graph->xaxis->SetLabelAlign('left','center');

    // Create the bar pot
    $bplot = new BarPlot($data[1]);
    $bplot->SetWidth(0.6);
    $bplot->value->Show();
    $bplot->value->SetFont(FF_ARIAL,FS_BOLD,8);
    $bplot->value->SetFormat("%d");
    $bplot->value->SetColor("black");
    $bplot->SetValuePos('bottom');

    // Setup color for gradient fill style
    $bplot->SetFillGradient("navy:1.9","navy:1.6",GRAD_HOR);

    // Set color for the frame of each bar
    $bplot->SetColor("white");
    $graph->Add($bplot);

    return $graph;
  }

  private function createAlertTimeSeries()
  {
    $graph = new Graph($this->params->width,$this->params->height);
    $graph->SetMargin(50,40,30,130);
    $graph->SetScale('datlin',0,100);
    $graph->title->Set($this->params->title);

    $severities=explode(".",$this->params->qparam->severities);

    $params=$this->params;

    $diffF=strtotime($params->qparam->date_from);
    $diffT=strtotime($params->qparam->date_to);

    //resolution of the plot depends on the range of data
    if (($diffT-$diffF)>=(7*24*3600))
      $params->qparam->extra='day';
    else
      $params->qparam->extra='hour';

    $data=array();

    foreach ($severities as $s)
    {
      $params->qparam->severities=$s;
      $d=$this->issueQuery2dTime($params);
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
    $graph->xaxis->SetLabelAngle(90);

    for ($i=0; $i<$count; $i++)
    {
      if (count($data[0][$i][1])==0)
        continue;
      $line[] = new LinePlot($data[0][$i][1],$data[0][$i][0]);
      end($line)->setLegend($data[1][$i]);
      end($line)->SetFillColor(severityToColor(nameToSeverity($data[1][$i])));
    }

    $accplot = new AccLinePlot($line);

    $graph->Add($accplot);
    return $graph;
  }

  private $params;
}

?>
