<?php
ini_set("memory_limit","256M"); //override default memory settings

class ComputeRecords
{
  public function computeStructure($data_row)
  {
    $data_row->logtime=date('Y-m-d H:i:s T',$data_row->logtime);
    switch($data_row->level)
      {
        case TLogger::ALERT:
          $data_row->level="Alert";
          break;
        case TLogger::DEBUG:
          $data_row->level="Debug";
          break;
        case TLogger::ERROR:
          $data_row->level="Error";
          break;
        case TLogger::FATAL:
          $data_row->level="Fatal";
          break;
        case TLogger::INFO:
          $data_row->level="Info";
          break;
        case TLogger::NOTICE:
          $data_row->level="Notice";
          break;
        case TLogger::WARNING:
          $data_row->level="Warning";
          break;
      default:
        $data_row->level="Unknown";
        break;
      }
    return $data_row;
  }
};


class WUILogs extends TPage
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    if (!$this->isPostBack)
      {
        $date_from=$this->Request->itemAt('from');
        if( $date_from!==null )
          $this->Range->From->Text=$date_from;

        $date_to=$this->Request->itemAt('to');
        if( $date_to!==null )
          $this->Range->To->Text=$date_to;

        $severities=$this->Request->itemAt('severities');
        if( $severities!==null )
          $this->Range->CB->setSelectedValues(explode('.',$severities));
      }
    $this->WUILogs->computation_=new ComputeRecords();
    $this->WUILogs->params_=$this->Range->getRangeData();
  }
};


?>
