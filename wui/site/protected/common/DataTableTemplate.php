<?php

class DataTableTemplate extends TTemplateControl
{
  function __construct()
  {
    parent::__construct();
    $this->log_=true;
    $this->sortup="sort-up.png";
    $this->sortdown="sort-down.png";
  }

  public function onLoad($param)
  {
    parent::onLoad($param);

    if ($this->DataGrid === null)
     return;

    $this->DataGrid->DataSource=$this->getDataRows($this->DataGrid->PageSize,$this->DataGrid->CurrentPageIndex);
    $this->DataGrid->VirtualItemCount=$this->getRowCount();

    if (count($this->DataGrid->DataSource)==0)
      {
        $this->Labelka->setVisible(true);
        $this->Pager->setVisible(false);
        return;
      }

    $this->Labelka->setVisible(false);
    $this->Pager->setVisible(true);

    $data=$this->DataGrid->DataSource[0]; //[0] is ok, count is >=1

    assert(count($data)!=0);

    $last_column_name;
    foreach($data as $column_name=>$rows)
      {
        if ($column_name=="id")
          continue; //skip columns entitled "id"
        $column=new TBoundColumn();
        $column->DataField=$column_name;
        $last_column_name=$column_name;
        $column->setSortExpression($column_name);
        if ($column_name=="Created" || $column_name=="Severity")
          $column->getItemStyle()->setHorizontalAlign('Center');
        $column->HeaderText="<font color=\"white\">".str_replace("_"," ",$column_name)."</font>";
        $this->DataGrid->Columns->add($column);
      }
    //$column->HeaderText.=$last_column_name;
    $this->DataGrid->dataBind();
  }

  private function getRowCount()
  {
    if ($this->params_===null)
      return ($this->log_)?SQLWrapper::queryForObject($this->query_.'Count'):CSQLMap::get()->queryForObject($this->query_.'Count');
    else
      return ($this->log_)?SQLWrapper::queryForObject($this->query_.'RangeCount',$this->params_):CSQLMap::get()->queryForObject($this->query_.'RangeCount',$this->params_);
  }

  private function getDataRows($perPage, $pageNumber)
  {
    if ($this->params_===null)
      {
        $sqlmap_param=new CParamLimitOffset;
        $sqlmap_param->limit=$perPage;
        $sqlmap_param->offset=$perPage*$pageNumber;
        $data=($this->log_)?SQLWrapper::queryForList($this->query_,$sqlmap_param):CSQLMap::get()->queryForList($this->query_,$sqlmap_para);
      }
    else
      {
        $this->params_->limit=$perPage;
        $this->params_->offset=$perPage*$pageNumber;
        $data=($this->log_)?SQLWrapper::queryForList($this->query_.'Range',$this->params_):CSQLMap::get()->queryForList($this->query_.'Range',$this->params_);
      }
    $ret=array();
    foreach($data as $e)
      $ret[]=$this->computation_->computeStructure($e);

    if (count($ret)==0)
      return null;

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

  public function setquery($param)
  {
    $this->query_=$param;
  }

  public function setlog($param)
  {
    $this->log_=$param;
  }

  public function settext($param)
  {
    $this->TopCaption->text=$param;
  }

  // TODO: check if these can be private
  public $computation_;
  public $query_;
  public $params_;
  public $log_;
  private $sortup;
  private $sortdown;
}

?>
