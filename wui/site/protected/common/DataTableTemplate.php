<?php

class DataTableTemplate extends TTemplateControl
{
  function __construct()
  {
    parent::__construct();
  }

  public function onLoad($param)
  {
    parent::onLoad($param);

    if ($this->DataGrid === null)
     return;

    $this->DataGrid->DataSource=$this->getDataRows($this->DataGrid->PageSize,$this->DataGrid->CurrentPageIndex);
    $this->DataGrid->VirtualItemCount=$this->getRowCount();
    if ($this->DataGrid->VirtualItemCount==0)
      {
        $this->DataGrid->reset();
        $this->DataGrid->DataSource=array(array('Info'=>'Your query returned no data. Take a deep breath and try again.'));
        $this->DataGrid->VirtualItemCount=1;
      }
    $this->DataGrid->dataBind();
  }

  private function getRowCount()
  {
    if ($this->params_===null)
      return CSQLMap::get()->queryForObject($this->query_.'Count');
    else
      return CSQLMap::get()->queryForObject($this->query_.'RangeCount',$this->params_);
  }

  private function getDataRows($perPage, $pageNumber)
  {
    if ($this->params_===null)
      {
        $sqlmap_param=new CParamLimitOffset;
        $sqlmap_param->limit=$perPage;
        $sqlmap_param->offset=$perPage*$pageNumber;
        $data=CSQLMap::get()->queryForList($this->query_,$sqlmap_param);
      }
    else
      {
        $this->params_->limit=$perPage;
        $this->params_->offset=$perPage*$pageNumber;
        $data=CSQLMap::get()->queryForList($this->query_.'Range',$this->params_);
      }

    foreach($data as $e)
      $ret[]=$this->computation_->computeStructure($e);

    return $ret; //ok to be null
  }

  public function selectionChanged($sender, $param)
  {
    $this->DataGrid->PageSize=$sender->SelectedValue;
    $this->DataGrid->CurrentPageIndex=0;
    $this->DataGrid->DataSource=$this->getDataRows($this->DataGrid->PageSize,$this->DataGrid->CurrentPageIndex);
    $this->DataGrid->dataBind();
  }

  public function changePage($sender, $param)
  {
    $this->DataGrid->CurrentPageIndex=$param->NewPageIndex;
    $this->DataGrid->DataSource=$this->getDataRows($this->DataGrid->PageSize,$this->DataGrid->CurrentPageIndex);
    $this->DataGrid->dataBind();
  }

  public function pagerCreated($sender, $param)
  {
    $param->Pager->Controls->insertAt(0, 'Page: ');
  }

  public function getDataGrid()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('DataGrid');
  }

  public function getTopCaption()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('TopCaption');
  }

  public function setcomputation_($param)
  {
    $this->computation_=$param;
  }

  public function setquery_($param)
  {
    $this->query_=$param;
  }

  public function settext_($param)
  {
    $this->TopCaption->text=$param;
  }

  // TODO: check if these can be private
  public $computation_;
  public $query_;
  public $params_;
}

?>
