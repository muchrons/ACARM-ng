<?php

class Analyzer extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $analyzerID=$this->Request->itemAt('id');
    assert( $analyzerID!==null );
    $this->analyzer_=SQLWrapper::queryForObject('SelectAnalyzer', $analyzerID);
  }

  public function onLoad($param)
  {
    if( $this->analyzer_===null )
      die("invalid analyzer / analyzer not set");

    if(!$this->IsPostBack)
    {
      $this->AnalyzerID->Text=$this->analyzer_->sys_id;

      $this->AnalyzerName->Text  = $this->getValueOrNA($this->analyzer_->name);
      $this->AnalyzerVersion->Text=$this->getValueOrNA($this->analyzer_->version);
      $this->AnalyzerOS->Text   =  $this->getValueOrNA($this->analyzer_->os);
      $this->AnalyzerIP->Text   =  $this->getValueOrNA($this->analyzer_->ip);
    }
  }

  private function getValueOrNA($value)
  {
    if ($value === null)
      return "N/A";
    return $value;
  }

  private $analyzer_;
}

?>
