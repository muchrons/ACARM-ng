<?php

class Home extends TPage
{
  private function format($number)
  {
    if($number===null)
      return "No data";
    TPropertyValue::ensureInteger($number);
    if ($number>1e9)
      return round($number/1e9)." G";
    if ($number>1e6)
      return round($number/1e6)." M";
    if ($number>1e3)
      return round($number/1e3)." k";
    return $number;
  }

  public function onLoad()
  {
    //$mainuse=SQLWrapper::queryForObject('SMetaAlertsInUse');
    $mainuse=SQLWrapper::queryForObject('SMetaAlertsInUse');
    $alerts=SQLWrapper::queryForObject('SAlertCount');
    $meta_alerts=SQLWrapper::queryForObject('SMetaAlertCount');

    $this->MAinUse->Text=$this->format($mainuse);
    $this->Alerts->Text=$this->format($alerts);
    $this->MetaAlerts->Text=$this->format($meta_alerts);
  }
}

?>
