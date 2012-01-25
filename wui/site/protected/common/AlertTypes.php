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

    $this->data_=$this->getDataRows();
    $this->DataGrid->DataSource=$this->data_;

    if (count($this->DataGrid->DataSource)==0)
      {
        $this->Labelka->setVisible(true);
        $this->DataGrid->setVisible(false);
        return;
      }

    $this->Labelka->setVisible(false);
    $this->DataGrid->setVisible(true);

    $this->DataGrid->dataBind();
  }

  private function getDataRows()
  {
    if ($this->params_===null)
      $data=($this->log_)?SQLWrapper::queryForList($this->query_):
        CSQLMap::get()->queryForList($this->query_);
    else
      $data=($this->log_)?SQLWrapper::queryForList($this->query_.'Range',$this->params_):
        CSQLMap::get()->queryForList($this->query_.'Range',$this->params_);

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

  public function getSelection()
  {
    if ($this->CheckedRows->Value=="")
      return array();

    $selectedRows=explode(',',$this->CheckedRows->Value);
    return $selectedRows;
  }

  public function getSelectedTypes()
  {
    $selectedRows=$this->getSelection();
    $names="(^";
    $first=false;
    foreach($selectedRows as $row)
      {
        if ($first==false)
          $first=true;
        else
          $names.="|";
        assert($this->data_[$row]!=null);
        $names.=$this->data_[$row]->Name;
      }
    $names.="$)";
    return $names;
  }

  // TODO: check if these can be private
  public $computation_;
  public $query_;
  public $params_;
  private $log_;
  private $data_;
}

?>
