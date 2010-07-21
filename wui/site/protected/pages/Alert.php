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
      // TODO: analyzers
      // TODO: sources
      // TODO: destinations
      $this->AlertDescription->Text=$this->alert_->description;
    }
  }

  /*
  private function 
  $url = $this->Service->constructUrl('PageName',
                                      array('id' => 42) );
      $this->Response->redirect($url);
  */

  private $alert_;
}

?>
