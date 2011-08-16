<?php

class AlertTypes extends TTemplateControl
{
  function __construct()
  {
    parent::__construct();
    $this->log_=true;
  }

  public function onLoad($param)
  {
    parent::onLoad($param);

    if ($this->DataGrid === null)
     return;

    $this->DataGrid->DataSource=$this->getDataRows();

    if (count($this->DataGrid->DataSource)==0)
      {
        $this->Labelka->setVisible(true);
        return;
      }

    $this->Labelka->setVisible(false);

    $data=$this->DataGrid->DataSource[0]; //[0] is ok, count is >=1

    assert(count($data)!=0);

    foreach($data as $column_name=>$rows)
      {
        if ($column_name=="id")
          continue; //skip columns entitled "id"

        $header=new TTableHeaderCell();
        $column=new TBoundColumn();
        $column->DataField=$column_name;
        if ($column_name=="Count")
          $column->getItemStyle()->setHorizontalAlign('Right');
        $column->HeaderText= str_replace("_"," ",$column_name);
        $column->initializeCell($header,0,"Header");
        $this->DataGrid->Columns->add($column);
        }
    $this->DataGrid->dataBind();
  }

  private function getDataRows()
  {
    if ($this->params_===null)
      $data=($this->log_)?SQLWrapper::queryForList($this->query_):CSQLMap::get()->queryForList($this->query_);
    else
      $data=($this->log_)?SQLWrapper::queryForList($this->query_.'Range',$this->params_):CSQLMap::get()->queryForList($this->query_.'Range',$this->params_);

    $ret=array();

    foreach($data as $e)
      $ret[]=$this->computation_->computeStructure($e);

    if (count($ret)==0)
      return null;

    return $ret; //ok to be null
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
}

?>
