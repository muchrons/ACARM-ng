<?php

class Alert extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $alertID=$this->Request->itemAt('id');
    assert( $alertID!==null );
    $this->alert_=CSQLMap::get()->queryForObject('SelectAlert', $alertID);
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
      $this->AlertDetectTime->Text =$this->alert_->detect;
      $this->AlertCreateTime->Text =$this->alert_->create;
      $this->AlertSeverity->Text   =$this->alert_->severity;
      $this->AlertCertainty->Text  =$this->alert_->certainty;
      $this->AlertAnalyzers->Text  =$this->makeAnalyzersLinkList($this->alert_->id);
      $this->AlertSource->Text     =$this->makeHostsLinkList($this->alert_->id, 'SelectAlertSourceHosts');
      $this->AlertDestination->Text=$this->makeHostsLinkList($this->alert_->id, 'SelectAlertDestinationHosts');
      $this->AlertDescription->Text=$this->alert_->description;
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
}

?>
