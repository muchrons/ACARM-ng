<?php

class Alert extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $this->alertID_=$this->Request->itemAt('id');
    assert( $this->alertID_!==null );
    $this->alert_=CSQLMap::get()->queryForObject('SelectAlert', $this->alertID_);
  }

  public function onLoad()
  {
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

      if($this->alert_->severity!= null)
        {
          $text="<font";
          $value=trim($this->alert_->severity);

          if ($value=="error")
            $text.=" color=\"red\"";
          else
            if ($value=="critical")
              $text.=" color=\"red\"";
            else
              if ($value=="problem")
                $text.=" color=\"#CC3300\"";
              else
                if ($value=="warning")
                  $text.=" color=\"black\"";
                else
                  if ($value=="info")
                    $text.=" color=\"green\"";
                  else
                    if ($value=="debug")
                      $text.=" color=\"blue\"";

          $text.=">".$value."</font>";
          $this->AlertSeverity->Text=$text;
        }

      $analyzers=CSQLMap::get()->queryForList('SelectAnalyzersForAlert', $this->alertID_);

      foreach ($analyzers as $a)
        $data[]=array('name'=>$a->name,'IP'=>$a->ip,'ver'=>$a->version, 'OS'=>$a->os);

      $this->AlertAnalyzers->DataSource=$data;
      $this->AlertAnalyzers->dataBind();

      $hosts=CSQLMap::get()->queryForList('SelectHostsForAlert', $this->alertID_);

      foreach ($hosts as $h)
        if ($h->role == "src")
          $sources[]=array('name'=>$h->name,'IP'=>$h->ip);
        else
          $destinations[]=array('name'=>$h->name,'IP'=>$h->ip);

      $this->AlertSources->DataSource=$sources;
      $this->AlertSources->dataBind();

      $this->AlertDestinations->DataSource=$destinations;
      $this->AlertDestinations->dataBind();

      $this->AlertDescription->Text=($this->alert_->description === null) ? "N/A" : $this->alert_->description;
      }
  }


  private function makeAnalyzersLinkList($idAlert)
  {
    $pairs=CSQLMap::get()->queryForList('SelectAlertAnalyzers', $idAlert);
    $out  ="";
    foreach($pairs as $e)
    {
      $out.=$this->makeLinkTo('Analyzer', $e->id, $e->name) . ' ';
    }
    return $out;
  }

  private function makeHostsLinkList($idAlert, $query)
  {
    $pairs=CSQLMap::get()->queryForList($query, $idAlert);
    $out  ="";
    foreach($pairs as $e)
    {
      $out.=$this->makeLinkTo('ReportedHost', $e->id, $e->name) . ' ';
    }
    return $out;
  }

  private function makeLinkTo($page, $id, $name)
  {
    $url =$this->Service->constructUrl( $page, array('id' => $id) );
    $href="<a href=\"$url\">$name</a>";
    return $href;
  }

  private $alert_;
  private $alertID_;
}

?>
