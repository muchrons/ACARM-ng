<?php

class WebLogs extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $this->data_=CSQLMap::get()->queryForList('SelectWUILogs');
  }

  public function onLoad()
  {
    // initialization of GridData
    if(!$this->IsPostBack)
    {
      $this->DataGrid->DataSource=$this->data_;
      $this->DataGrid->dataBind();
    }
  }

  public function changePage($sender, $param)
  {
    $this->DataGrid->CurrentPageIndex=$param->NewPageIndex;
    $this->DataGrid->DataSource      =$this->data_;
    $this->DataGrid->dataBind();
  }

  public function pagerCreated($sender, $param)
  {
    $param->Pager->Controls->insertAt(0, 'Page: ');
  }

  private $data_;
}

?>
