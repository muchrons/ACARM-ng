<?php

// TODO: 'jpgraph' should be moved somewhere to protected/ directory, where all the sources are (access to this dir is protected with .htaccess)
require_once('lib/jpgraph/jpgraph.php');
require_once('lib/jpgraph/jpgraph_bar.php');
require_once('lib/jpgraph/jpgraph_log.php');
require_once('lib/jpgraph/jpgraph_led.php');
require_once('lib/jpgraph/jpgraph_pie.php');
require_once('lib/jpgraph/jpgraph_pie3d.php');
require_once('lib/jpgraph/jpgraph_line.php');
require_once('lib/jpgraph/jpgraph_date.php');


class GraphService extends TService
{
  public function init($config)
  {
    $request = Prado::getApplication()->getRequest();
    $this->Response->ContentType="image/png";   // TODO: move this line one line up - it interleaves assign/check sequence for $request

    if (!$request->contains('graph'))
      throw new TConfigurationException('You must specify the type of the graph');
    $this->type = TPropertyValue::ensureString($request['graph']);

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
    switch( $this->type )
    {
    case "AlertTimeSeries":
      $graph = $this->createAlertTimeSeries($this->query,$this->title);
      break;
    case "SeverityPie":
      $graph = $this->createSeverityChart('DMSeverities');
      break;
    case "AlertTypes":
      $graph = $this->createAlertTypeChart('DMAlertTypes');
      break;
    default:
      // Display LED screen with an error message in place of a graph
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

  private function issueQuery2dTime($q,$severity)
  {
    $pairs=CSQLMap::get()->queryForList($q,$severity);

    $xdata = array();
    $ydata = array();

    foreach( $pairs as $e )
    {
      $xdata[] = strtotime($e->key);
      $ydata[] = ($e->value===null)?0:$e->value;
    }

    return array($xdata, $ydata);
  }


  private function createSeverityChart($q)
  {
    // Create the Pie Graph.
    $graph = new PieGraph($this->width,$this->height,'auto');

    // Set A title for the plot
    $graph->title->Set($this->title);
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
    $graph = new Graph($this->width,$this->height);
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
    $graph->SetMargin(50,40,30,130);
    $graph->SetScale('datlin',0,100);
    $graph->title->Set($this->title);

    $dataWarning=$this->issueQuery2dTime($q,'warning');
    $dataCritical=$this->issueQuery2dTime($q,'critical');
    $dataProblem=$this->issueQuery2dTime($q,'problem');
    $dataError=$this->issueQuery2dTime($q,'error');

    $maxval=max($dataWarning[1],$dataError[1],$dataProblem[1],$dataCritical[0]);

    if (count($dataWarning[0])==0 && count($dataCritical[0])==0 )
      {
        $led = new DigitalLED74(2);
        $led->SetSupersampling(5);
        $led->StrokeNumber('NO DATA. Maybe database is empty?',LEDC_GREEN);
        die();
      }

    $graph->SetScale('datlin',0,maxval);
    $graph->xaxis->SetLabelAngle(90);

    $line[] = new LinePlot($dataCritical[1],$dataCritical[0]);
    end($line)->setLegend("Critical");
    end($line)->SetFillColor('red');

    $line[] = new LinePlot($dataError[1],$dataError[0]);
    end($line)->setLegend("Error");
    end($line)->SetFillColor('orange');

    $line[] = new LinePlot($dataProblem[1],$dataProblem[0]);
    end($line)->setLegend("Problem");
    end($line)->SetFillColor('yellow');

    $line[] = new LinePlot($dataWarning[1],$dataWarning[0]);
    end($line)->setLegend("Warning");
    end($line)->SetFillColor('lightblue@0.3');

    $accplot = new AccLinePlot($line);

    $graph->Add($accplot);
    //$graph->Add($line[0]);
    return $graph;
  }


  private $query = null;
  private $type;
  private $width;
  private $height;
  private $title;
}

?>
