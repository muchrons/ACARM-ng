<?php

class ReportedHost extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $repHostID=$this->Request->itemAt('id');
    assert( $repHostID!==null );
    $this->repHost_=CSQLMap::get()->queryForObject('SelectReportedHost', $repHostID);
  }

  public function onLoad()
  {
    if( $this->repHost_===null )
      die("invalid host / host not set");
    // initialization of GridData
    if(!$this->IsPostBack)
    {
      // fill fileds with avlues
      $this->ReportedHostRole->Text   =$this->repHost_->role;
      $this->ReportedHostIP->Text     =$this->repHost_->ip;
      $this->ReportedHostMask->Text   =$this->repHost_->mask;
      $this->ReportedHostOS->Text     =$this->repHost_->os;
      $this->ReportedHostName->Text   =$this->repHost_->name;
      $this->ReportedHostRefName->Text=$this->repHost_->ref_name;
      $this->ReportedHostRefURL->Text =$this->repHost_->ref_url;
    }
  }

  private $repHost_;
}

?>
