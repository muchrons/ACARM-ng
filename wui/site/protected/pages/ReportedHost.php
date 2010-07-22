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
      $this->ReportedHostRole->Text     =($this->repHost_->role==='src')?'source':'destination';
      $this->ReportedHostIP->Text       =$this->repHost_->ip;
      $this->ReportedHostMask->Text     =$this->repHost_->mask;
      $this->ReportedHostOS->Text       =$this->repHost_->os;
      $this->ReportedHostName->Text     =$this->repHost_->name;
      $this->ReportedHostProcesses->Text=$this->makeProcessesLinkList($this->repHost_->id);
      $this->ReportedHostServices->Text =$this->makeServicesLinkList($this->repHost_->id);
      $this->ReportedHostRefName->Text  =$this->repHost_->ref_name;
      $this->ReportedHostRefURL->Text   =$this->repHost_->ref_url;
    }
  }

  private function makeProcessesLinkList($idRepHost)
  {
    return $this->makeLinkList($idRepHost, 'SelectReportedHostsProcesses', 'ReportedProcess');
  }

  private function makeServicesLinkList($idRepHost)
  {
    return $this->makeLinkList($idRepHost, 'SelectReportedHostsServices', 'ReportedService');
  }

  private function makeLinkList($idRepHost, $query, $page)
  {
    $pairs=CSQLMap::get()->queryForList($query, $idRepHost);
    $out  ="";
    foreach($pairs as $e)
    {
      $out.=$this->makeLinkTo($page, $e->id, $e->name) . ' ';
    }
    return $out;
  }

  private function makeLinkTo($page, $id, $name)
  {
    $url =$this->Service->constructUrl( $page, array('id' => $id) );
    $href="<a href=\"$url\">$name</a>";
    return $href;
  }

  private $repHost_;
}

?>
