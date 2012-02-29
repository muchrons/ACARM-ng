<?php

class Home extends TPage
{
  private function format($number)
  {
    if($number===null)
      return "No data";
    TPropertyValue::ensureInteger($number);
    if ($number>1e9)
      return (round($number/1e7)/100)." G&nbsp";
    if ($number>1e6)
      return (round($number/1e4)/100)." M&nbsp";
    if ($number>1e3)
      return (round($number/10)/100)." k&nbsp";
    return $number;
  }

  public function onLoad($param)
  {
    $mainuse=SQLWrapper::queryForObject('SMetaAlertsInUse');
    $alerts=SQLWrapper::queryForObject('SAlertCount');
    $meta_alerts=SQLWrapper::queryForObject('SMetaAlertCount');
    $heartbeats=SQLWrapper::queryForList('SelectHeartbeats');


    $this->StatusGrid->DataSource=
      array(
            array(
                  'text'=>'Meta alerts in use',
                  'value'=>$this->format($mainuse),
                  ),
            array(
                  'text'=>'Number of alerts in database',
                  'value'=>$this->format($alerts),
                  ),
            array(
                  'text'=>'Number of meta alerts in database',
                  'value'=>$this->format($meta_alerts),
                  )
            );

    $this->StatusGrid->databind();


    $this->Heartbeats->DataSource=$heartbeats;
    $this->Heartbeats->databind();
  }


  public function deleteItem($sender,$param)
  {
    $idx=$param->Item->ItemIndex;
    $owner=$this->Heartbeats->DataSource[$idx]->owner;
    $module=$this->Heartbeats->DataSource[$idx]->module;

    $c=new CDMPair();
    $c->key=$owner;
    $c->value=$module;

    SQLWrapper::delete('DeleteHeartbeat',$c);

    $heartbeats=SQLWrapper::queryForList('SelectHeartbeats');
    $this->Heartbeats->DataSource=$heartbeats;
    $this->Heartbeats->databind();
  }
}

?>
