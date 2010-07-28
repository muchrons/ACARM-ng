<?php

class MetaAlert extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $id=$this->Request->itemAt('id');
    assert( $id!==null );
    $this->metaAlert_=CSQLMap::get()->queryForObject('SelectMetaAlert', $id);
  }

  public function onLoad()
  {
    if( $this->metaAlert_===null )
      die("invalid meta-alert / meta-alert not set");
    // initialization of GridData
    if(!$this->IsPostBack)
    {
      // fill fileds with avlues
      $this->MetaAlertName->Text          =$this->metaAlert_->name;
      $this->MetaAlertCreateTime->Text    =$this->metaAlert_->create_time;
      $this->MetaAlertLastUpdateTime->Text=$this->metaAlert_->last_update_time;
      $this->MetaAlertSeverityDelta->Text =$this->metaAlert_->severity_delta;
      $this->MetaAlertCertaintyDelta->Text=$this->metaAlert_->certainty_delta;
      // node or leaf?
      if($this->metaAlert_->id_alert===null)
      {
        // node
        $this->MetaAlertLinksName->Text="Children";
        $this->MetaAlertLinks->Text    =$this->makeChildrenLinkList($this->metaAlert_->id);
      }
      else
      {
        // leaf
        $this->MetaAlertLinksName->Text="Associated alert";
        $this->MetaAlertLinks->Text    =$this->makeLinkTo('Alert', $this->metaAlert_->id_alert, 'details');
      }
    } // if(!post_back)
  }


  private function makeChildrenLinkList($idAlert)
  {
    $pairs=CSQLMap::get()->queryForList('SelectMetaAlertsChildren', $idAlert);
    $out  ="";
    foreach($pairs as $e)
    {
      $out.=$this->makeLinkTo('MetaAlert', $e->id, $e->name) . '<br/>';
    }
    return $out;
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
