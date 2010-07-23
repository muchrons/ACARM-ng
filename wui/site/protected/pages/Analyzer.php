<?php

class Analyzer extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $analyzerID=$this->Request->itemAt('id');
    assert( $analyzerID!==null );
    $this->analyzer_=CAnalyzerRecord::finder()->findByPk($analyzerID);
  }

  public function onLoad()
  {
    if( $this->analyzer_===null )
      die("invalid analyzer / analyzer not set");
    // initialization of GridData
    if(!$this->IsPostBack)
    {
      // fill fileds with avlues
      $this->AnalyzerID->Text     =$this->analyzer_->sys_id;
      $this->AnalyzerName->Text   =$this->analyzer_->name;
      $this->AnalyzerVersion->Text=$this->analyzer_->version;
      $this->AnalyzerOS->Text     =$this->analyzer_->os;
      $this->AnalyzerIP->Text     =$this->analyzer_->ip;
    }
  }

  private $analyzer_;
}

?>
