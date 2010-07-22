<?php

class ReportedProcess extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $repProcID=$this->Request->itemAt('id');
    assert( $repProcID!==null );
    $this->repProc_=CSQLMap::get()->queryForObject('SelectReportedProcess', $repProcID);
  }

  public function onLoad()
  {
    if( $this->repProc_===null )
      die("invalid process / process not set");
    // initialization of GridData
    if(!$this->IsPostBack)
    {
      // fill fileds with avlues
      $this->ReportedProcessName->Text     =$this->repProc_->name;
      $this->ReportedProcessPath->Text     =$this->repProc_->path;
      $this->ReportedProcessMD5->Text      =$this->repProc_->md5;
      $this->ReportedProcessPID->Text      =$this->repProc_->pid;
      $this->ReportedProcessUID->Text      =$this->repProc_->uid;
      $this->ReportedProcessUsername->Text =$this->repProc_->username;
      $this->ReportedProcessArguments->Text=$this->repProc_->arguments;
      $this->ReportedProcessRefName->Text  =$this->repProc_->ref_name;
      $this->ReportedProcessRefURL->Text   =$this->repProc_->ref_url;
    }
  }

  private $repProc_;
}

?>
