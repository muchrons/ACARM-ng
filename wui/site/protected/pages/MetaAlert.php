<?php

class MetaAlert extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $id=$this->Request->itemAt('id');
    $sysid=$this->Request->itemAt('sys_id');

    assert($id!=null || $sysid!=null);

    if ($id!=null)
      $this->metaAlert_=CSQLMap::get()->queryForObject('SelectMetaAlertID', $id);
    else
      if ($sysid!=null)
        $this->metaAlert_=CSQLMap::get()->queryForObject('SelectMetaAlertSysID', $sysid);
      else
        throw new TConfigurationException("SysID and ID are both null");
  }

  public function onLoad()
  {
    if( $this->metaAlert_===null )
      die("invalid meta-alert / meta-alert not set");
    // initialization of GridData
    if(!$this->IsPostBack)
      {
        $this->MetaAlertCreateTime->Text=$this->metaAlert_->create_time;
        $this->MetaAlertUpdateTime->Text=$this->metaAlert_->last_update_time;
        $idAlert=$this->metaAlert_->id;

        $alerts=CSQLMap::get()->queryForList('SelectMetaAlertsChildren', $idAlert);

        foreach ($alerts as $a)
          if ($a->alertid === null)
            $data[]=array('name'=>$this->makeLinkTo('MetaAlert', $a->metaalertid, $a->name));
          else
            $data[]=array('name'=>$this->makeLinkTo('Alert', $a->alertid, $a->name));

        $this->RelatedAlerts->DataSource=$data;
        $this->RelatedAlerts->dataBind();

        $filters=preg_split('/\s+/', $this->metaAlert_->name);

        $this->MetaAlertName->Text="";

        foreach($filters as $f)
          {
            if ($f=='[many2one]')
              $this->Correlated->Text.="<img src=\"pics/filters/MTO.png\" border=2>";
            else
            if ($f=='[one2many]')
              $this->Correlated->Text.="<img src=\"pics/filters/OTM.png\" border=2>";
            else
            if ($f=='[samename]')
              $this->Correlated->Text.="<img src=\"pics/filters/name.png\" border=2>";
            else
            if ($f=='[many2many]')
              $this->Correlated->Text.="<img src=\"pics/filters/MTM.png\" border=2>";
            else
              $this->MetaAlertName->Text.=$f." ";
          }

      } // if(!post_back)
  }


  private function makeLinkTo($page, $id, $name)
  {
    $url =$this->Service->constructUrl( $page, array('id' => $id) );
    $href="<a href=\"$url\">$name</a>";
    return $href;
  }

  private $metaAlert_;
}

?>
