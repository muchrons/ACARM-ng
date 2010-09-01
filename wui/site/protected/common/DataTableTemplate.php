<?php

class DataTableTemplate extends TTemplateControl
{
  public function onLoad()
  {
    //TODO: usunalem ispostback bo nie dzialalo
    $this->DataGrid->DataSource=$this->getDataRows($this->DataGrid->PageSize,$this->DataGrid->CurrentPageIndex);
    $this->DataGrid->VirtualItemCount=$this->getRowCount();
    $this->DataGrid->dataBind();
  }

  private function getRowCount()
  {
    return CSQLMap::get()->queryForObject('SelectAlertsSummaryCount');
  }

  private function getDataRows($perPage, $pageNumber)
  {
    $sqlmap_param=new CParamLimitOffset;
    $sqlmap_param->limit=$perPage;
    $sqlmap_param->offset=$perPage*$pageNumber;
    $data=CSQLMap::get()->queryForList('SelectAlertsSummaryRange',$sqlmap_param);

    foreach($data as $e)
    {
      $url    =$this->Service->constructUrl( 'Alert', array('id' => $e->id) );
      $href   ="<a href=\"$url\">see</a>";
      $e->link=$href;
    }
    return $data;
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
}

?>
