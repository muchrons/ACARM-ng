<?php

class ReportedService extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $repServiceID=$this->Request->itemAt('id');
    assert( $repServiceID!==null );
    $this->repService_=CSQLMap::get()->queryForObject('SelectReportedService', $repServiceID);
  }

  public function onLoad()
  {
    if( $this->repService_===null )
      die("invalid host / host not set");
    // initialization of GridData
    if(!$this->IsPostBack)
    {
      // fill fileds with avlues
      $this->ReportedServiceName->Text    =$this->repService_->name;
      $this->ReportedServicePort->Text    =$this->repService_->port;
      $this->ReportedServiceProtocol->Text=$this->repService_->protocol;
      $this->ReportedServiceRefName->Text =$this->repService_->ref_name;
      $this->ReportedServiceRefURL->Text  =$this->repService_->ref_url;
    }
  }

  private $repService_;
}

?>
