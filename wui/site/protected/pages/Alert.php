<?php

class Alert extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $this->alertID_=$this->Request->itemAt('id');
    assert( $this->alertID_!==null );
    $this->alert_=SQLWrapper::queryForObject('SelectAlert', $this->alertID_);
  }

  public function onLoad($param)
  {
    parent::onLoad($param);

    if( $this->alert_===null )
      die("invalid alert / alert not set");
    // initialization of GridData
    if(!$this->IsPostBack)
    {
      // fill fileds with avlues
      $this->AlertName->Text       =$this->alert_->name;

      if ($this->alert_->detect !=null)
        $this->AlertDetectTime->Text =$this->alert_->detect;

      if ($this->alert_->create !=null)
        $this->AlertCreateTime->Text =$this->alert_->create;

      $this->AlertCertainty->Text=($this->alert_->certainty === null) ? "N/A" : $this->alert_->certainty;

      if($this->alert_->description!= null)
        $this->AlertDescription->Text=$this->alert_->description;

      if($this->alert_->severity!= null)
      {
        $text="<font";
        $value=trim($this->alert_->severity);

        if ($value=="4")
          $text.=" color=\"red\"";
        else
          if ($value=="3")
            $text.=" color=\"#CC3300\"";
          else
            if ($value=="2")
              $text.=" color=\"black\"";
            else
              if ($value=="1")
                $text.=" color=\"green\"";
              else
                if ($value=="0")
                  $text.=" color=\"blue\"";

        $text.=">".$this->severityToName($value)."</font>";
        $this->AlertSeverity->Text=$text;
      }

      //Get all analyzers for the alert
      $analyzers=SQLWrapper::queryForList('SelectAnalyzersForAlert', $this->alertID_);

      foreach ($analyzers as $a)
        $data[]=array('link'=>$this->makeAnalyzerLink($a->id),'name'=>$a->name,'IP'=>$a->ip,'ver'=>$a->version, 'OS'=>$a->os);
      //data can be null
      $this->AlertAnalyzers->DataSource=$data;
      $this->AlertAnalyzers->dataBind();

      //Get all hosts for the alert
      $hosts=SQLWrapper::queryForList('SelectHostsForAlert', $this->alertID_);
      $sources=array();
      $destinations=array();
      foreach ($hosts as $h)
        if ($h->role == "src")
          $sources[]=array('link'=>$this->makeHostLink($h->id),'name'=>$h->name,'IP'=>$h->ip);
        else
          $destinations[]=array('link'=>$this->makeHostLink($h->id),'name'=>$h->name,'IP'=>$h->ip);

      $this->AlertSources->DataSource=$sources; //can be null
      $this->AlertSources->dataBind();

      $this->AlertDestinations->DataSource=$destinations; //can be null
      $this->AlertDestinations->dataBind();
    }
  }

  private function makeHostLink($id)
  {
    $out=$this->makeLinkTo('Host', $id, '<img src="pics/dot.png" border="0">');
    return $out;
  }

  private function makeAnalyzerLink($id)
  {
    $out=$this->makeLinkTo('Analyzer', $id, '<img src="pics/dot.png" border="0">');
    return $out;
  }

  private function makeLinkTo($page, $id, $name)
  {
    $url =$this->Service->constructUrl( $page, array('id' => $id) );
    $href="<a href=\"$url\">$name</a>";
    return $href;
  }

  private function severityToName($n)
  {
    if ($n=="0")
      return "debug";
    if ($n=="1")
      return "info";
    if ($n=="2")
      return "low";
    if ($n=="3")
      return "medium";
    if ($n=="4")
      return "high";
    return "unknown";
  }

  private $alert_;
  private $alertID_;
}

?>
