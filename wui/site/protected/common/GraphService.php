<?php

// TODO: i suggest using 'require_once' (stop on error) instead of 'include_once' (flood output with warnings on error)
// TODO: 'jpgraph' should be moved somewhere to protected/ directory, where all the sources are (access to this dir is protected with .htaccess)
include_once('lib/jpgraph/jpgraph.php');
include_once('lib/jpgraph/jpgraph_bar.php');
include_once('lib/jpgraph/jpgraph_log.php');
include_once('lib/jpgraph/jpgraph_led.php');
include_once('lib/jpgraph/jpgraph_line.php');
include_once('lib/jpgraph/jpgraph_pie.php');
include_once('lib/jpgraph/jpgraph_pie3d.php');
include_once('lib/jpgraph/jpgraph_line.php');   // TODO: this file is included twice
include_once('lib/jpgraph/jpgraph_date.php');


class GraphService extends TService
{
  public function init($config)
  {
    $request = Prado::getApplication()->getRequest();
    $this->Response->ContentType="image/png";   // TODO: move this line one line up - it interleaves assign/check sequence for $request

    // TODO: refactor this logic to if(null) throw; do_the_thing(); instead of if/else
    if ($request->contains('graph'))
      $this->type = TPropertyValue::ensureString($request['graph']);
    else
      throw new TConfigurationException('You must specify the type of the graph');

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
    switch( $this->type )
    {
    case "AlertTimeSeries":
      $graph = $this->createAlertTimeSeries($this->query);
      break;
    case "SeverityPie":
      $graph = $this->createSeverityChart('DMSeverities');
      break;
    case "AlertTypes":
      $graph = $this->createAlertTypeChart('DMAlertTypes');
      break;
    default:
      // TODO: describe these calls and parameters - they are non-obvious
      $led = new DigitalLED74(4);
      $led->SetSupersampling(5);
      $led->StrokeNumber(' WRONG GRAPH TYPE ',LEDC_YELLOW);
      return;
    }

    $graph->Stroke();
  }

  private function issueQuery2d($q,$append="")
  {
    $pairs=CSQLMap::get()->queryForList($q);

    $xdata = array();
    $ydata = array();

    foreach( $pairs as $e )
    {
      $xdata[] = trim($e->key).$append;
      $ydata[] = $e->value;
    }

    return array($xdata, $ydata);
  }

  private function issueQuery2dTime($q)
  {
    // TODO: use issueQuery2d() implementation here
    $pairs=CSQLMap::get()->queryForList($q);

    $xdata = array();
    $ydata = array();

    foreach( $pairs as $e )
    {
      $xdata[] = strtotime($e->key);
      $ydata[] = $e->value;
    }

    return array($xdata, $ydata);
  }


  private function createSeverityChart($q)
  {
    // Create the Pie Graph.
    $graph = new PieGraph(690,400,'auto');  // TODO: widht/height?

    // Set A title for the plot
    $graph->title->Set("Alert count by severity type");
    $graph->title->SetMargin(8);
    $graph->title->SetFont(FF_VERDANA,FS_BOLD,12);
    $graph->title->SetColor("darkred");

    $data=$this->issueQuery2d($q," (%d)");

    // Create
    $p1 = new PiePlot3D($data[1]);
    $p1->SetLabels($data[0]);
    $p1->SetLabelPos(1);
    $p1->SetLabelType(PIE_VALUE_PER);
    $p1->value->SetFont(FF_ARIAL,FS_NORMAL,12);
    $p1->SetSliceColors(array('#ff0000','#ff00000','#b74700','#669900','#15eb00')); 
    //    $p1->SetTheme("sand");
    $p1->SetLabelType(1);
    $p1->SetSize(0.50);
    $p1->value->SetFormat("%d");

    $graph->SetAntiAliasing();
    $graph->Add($p1);
    return $graph;
  }

  private function createAlertTypeChart($q)
  {
    // Setup the graph.
    $graph = new Graph(800,800);    // TODO: widht/height?
    $graph->img->SetMargin(40,35,45,25);
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

    $data=$this->issueQuery2d($q);

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

  private function createAlertTimeSeries($q)
  {
    $graph = new Graph($this->width,$this->height);
    $graph->SetMargin(40,40,30,130);
    $graph->SetScale('datlin',0,100);
    $graph->title->Set("Example on Date scale");

    $data=$this->issueQuery2dTime($q);

    if (count($data[0])==0)
      {
        $led = new DigitalLED74(2);
        $led->SetSupersampling(5);
        $led->StrokeNumber('NO DATA. Maybe database is empty?',LEDC_GREEN);
        die();
      }

    $graph->SetScale('datlin',0,max($data[1]));
    $graph->xaxis->SetLabelAngle(90);

    $line = new LinePlot($data[1],$data[0]);
    $line->SetFillColor('lightblue@0.5');
    $graph->Add($line);
    return $graph;
  }


  private $query = null;
  private $type;
  private $width;
  private $height;
}

?>
