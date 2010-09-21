<?php

include_once('lib/jpgraph/jpgraph.php');
include_once('lib/jpgraph/jpgraph_bar.php');
include_once('lib/jpgraph/jpgraph_log.php');
include_once('lib/jpgraph/jpgraph_led.php');
include_once('lib/jpgraph/jpgraph_line.php');
include_once('lib/jpgraph/jpgraph_pie.php');
include_once('lib/jpgraph/jpgraph_pie3d.php');

 
class GraphService extends TService 
{
  private $type;
  private $xdata = null;
  private $ydata = null;

  private $ytitle = null;
  private $width = 500;
  private $height = 400;
 
  public function init($config) 
  {
    $request = Prado::getApplication()->getRequest();
 
    if ($request->contains('graph')) {
      $this->type = TPropertyValue::ensureString($request['graph']);
    } else {
      throw new TConfigurationException('You must specify the type of the graph');
    }
 
    if ($request->contains('xdata')) {
      $this->xdata = explode( ',', TPropertyValue::ensureString($request['xdata']));
    } else {
      throw new TConfigurationException('You must specify the x data for the graph');
    }
 
    if ($request->contains('ydata')) {
      $this->ydata = explode( ',', TPropertyValue::ensureString($request['ydata']));
    } else {
      throw new TConfigurationException('You must specify the y data for the graph');
    } 
 
    if ($request->contains('ytitle')) {
      $this->ytitle = TPropertyValue::ensureString($request['ytitle']);
    } else {
      throw new TConfigurationException('You must specify the y title for the graph.');
    }
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
 
  public function getShadow() 
  {
    return $this->shadow;
  }
  
  public function setShadow($value) 
  {
    $this->shadow = TPropertyValue::ensureBoolean($value);
  }
 
  public function run() 
  {
    switch( $this->type ) 
      {
      case 1:
        $graph = $this->createStackedBarGraph($this->xdata, $this->ydata, $this->ydata2, $this->ytitle);
        break;
 
      case "AlertTimeSeries":
        $graph = $this->createAlertTimeSeries($this->xdata, $this->ydata, $this->ytitle);
        break;

      case "SeverityPie":
        $graph = $this->createSeverityChart($this->xdata, $this->ydata);
        break;
      case "AlertTypes":
        $graph = $this->createAlertTypeChart($this->xdata, $this->ydata);
        break;
      default:
        $led = new DigitalLED74(4);
        $led->SetSupersampling(5);
        $led->StrokeNumber(' WRONG GRAPH TYPE ',LEDC_YELLOW);
        return;
      }    

    header("Content-type: image/png");    
    $graph->Stroke();
  }

  private function createSeverityChart($x,$y)
  {
    // Create the Pie Graph.
    $graph = new PieGraph(800,600,'auto');

    // Set A title for the plot
    $graph->title->Set("Alert count by severity type");
    $graph->title->SetMargin(8);
    $graph->title->SetFont(FF_VERDANA,FS_BOLD,12);
    $graph->title->SetColor("darkred");

    // Create
    $p1 = new PiePlot3D($y);
    $p1->SetLabels($x);
    $p1->SetLabelPos(1);
    $p1->SetLabelType(PIE_VALUE_PER);    
    $p1->value->SetFont(FF_ARIAL,FS_NORMAL,12);

    $p1->SetTheme("sand");
    $p1->SetLabelType(1);
    $p1->SetSize(0.30);
    $p1->value->SetFormat("%d");

    $graph->SetAntiAliasing();
    $graph->Add($p1);
    return $graph;
  }

  private function createAlertTypeChart($x,$y)
  {
    // Setup the graph.
    $graph = new Graph(800,800);
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

    // Setup X-axis labels
    $graph->xaxis->SetTickLabels($x);
    $graph->xaxis->SetLabelAngle(0);
    $graph->xaxis->SetTitleMargin(0);
    $graph->xaxis->SetLabelMargin(-50);
    $graph->xaxis->SetLabelAlign('left','center');

    // Create the bar pot
    $bplot = new BarPlot($y);
    $bplot->SetWidth(0.6);
    $bplot->value->Show();
    $bplot->value->SetFont(FF_ARIAL,FS_BOLD,8);
    $bplot->value->SetFormat("%d");
    $bplot->value->SetColor("black");
    //$bplot->SetShadow('black@0.9');
    $bplot->SetValuePos('bottom');


    // Setup color for gradient fill style
    $bplot->SetFillGradient("navy:1.9","navy:1.6",GRAD_HOR);

    // Set color for the frame of each bar
    $bplot->SetColor("white");
    $graph->Add($bplot);

    return $graph;
  }

  private function createAlertTimeSeries($x,$y)
  {
    // Setup the graph.
    $graph = new Graph(800,800);
    $graph->img->SetMargin(40,35,45,25);    
    $graph->SetScale("textlin");
    //$graph->SetAngle(90);
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
    //$graph->yscale->ticks->SupressZeroLabel(false);

    // Setup X-axis labels
    //$graph->xaxis->SetTickLabels($x);
    //$graph->xaxis->SetLabelAngle(0);
    //$graph->xaxis->SetTitleMargin(0);
    //$graph->xaxis->SetLabelMargin(-50);
    //$graph->xaxis->SetLabelAlign('left','center');
    //echo ">".implode($x,',')."<";
    //echo ">".implode($y,',')."<";
    // Create the bar pot
    //$bplot = new BarPlot($y);
    $line = new LinePlot($x,array(1,2,3,4,5));
    //$bplot->SetWidth(0.6);
    //$bplot->value->Show();
    /*
    $bplot->value->SetFont(FF_ARIAL,FS_BOLD,8);
    $bplot->value->SetFormat("%d");
    $bplot->value->SetColor("black");
    //$bplot->SetShadow('black@0.9');
    $bplot->SetValuePos('bottom');


    // Setup color for gradient fill style
    $bplot->SetFillGradient("navy:1.9","navy:1.6",GRAD_HOR);
    */
    // Set color for the frame of each bar
    //$bplot->SetColor("white");
    $graph->Add($line);
    
    return $graph;
  }



}
 
?>