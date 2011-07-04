<?php

class MetaAlert extends TPage
{
  private $per_page=100; //number of alerts displayed per page

  public function __construct()
  {
    parent::__construct();
    $id=$this->Request->itemAt('id');

    //in case there is no id try to get sys_id and convert to id
    if ($id===null)
      {
        $sysid=$this->Request->itemAt('sys_id');
        $id=SQLWrapper::queryForObject('MetaAlert_SysID_ID', $sysid);
      }

    //neither id nor sys_id specified
    if ($id===null)
      throw new TConfigurationException("No valis SysID or ID specified");

    $this->metaAlert_=SQLWrapper::queryForObject('SelectMetaAlertID', $id);
  }

  public function onLoad()
  {
    if( $this->metaAlert_===null )
      die("invalid meta-alert / meta-alert not set");
    $this->RelatedAlerts->PageSize=$this->per_page;
    // initialization of GridData
    if(!$this->IsPostBack)
      {
        $this->MetaAlertCreateTime->Text=$this->metaAlert_->create_time;
        $this->MetaAlertUpdateTime->Text=$this->metaAlert_->last_update_time;
        $idAlert=$this->metaAlert_->id;

        //get out alert in case we are a leaf
        $alerts=SQLWrapper::queryForList('SelectAlertForMetaAlert', $idAlert);

        if (count($alerts) == 0)
          {//we are a node not a leaf
            $params=new CDMQuad();
            $params->value1=$idAlert;
            $params->value2=$this->per_page; //limit
            $params->value3=0; //offset
            $alerts=SQLWrapper::queryForList('SelectMetaAlertsChildren', $params);
          }

        //get number of child alerts to show
        $children_count=SQLWrapper::queryForObject('SelectMetaAlertsChildrenCount', $idAlert);

        $data=array();

        foreach ($alerts as $a)
          if ($a->alertid === null)
            $data[]=array('name'=>$this->makeLinkTo('MetaAlert', $a->metaalertid, $a->name));
          else
            $data[]=array('name'=>$this->makeLinkTo('Alert', $a->alertid, $a->name));

        if (count($data)==0)
          $data[]=array('name'=>'This shouldn\'t be null. Your database seems to be corrupted.');

        $this->RelatedAlerts->DataSource=$data;
        $this->RelatedAlerts->VirtualItemCount=$children_count;
        $this->RelatedAlerts->CurrentPageIndex=0;
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

        $inUse=SQLWrapper::queryForObject('CheckInUse', $idAlert);
        if ($inUse!=0)
          $this->MetaAlertTags->Text.="<font color=\"yellow\"><b>In use</b></font></br>";

        $isRoot=SQLWrapper::queryForObject('CheckRoot', $idAlert);
        if ($isRoot!=0)
          $this->MetaAlertTags->Text.="<font color=\"red\"><b>Root</b></font> ";

        $triggers_tab=SQLWrapper::queryForList('SelectTriggered', $idAlert);

        $content="";

        foreach ($triggers_tab as $t)
          {
            $temp=explode("_",$t->trigger."_");
            $content.="<table border=\"0\" cellpadding=\"2\"><td align=\"center\"><img height=\"32\" width=\"32\" src=\"pics/triggers/$temp[0].png\" alt=\"$temp[0]\"></td>";
            $content.="<td>$temp[1]</td></table>";
          }

        $this->Triggered->Text=$content;

      } // if(!post_back)
  }

  public function changePage($sender,$param)
  {
    $this->RelatedAlerts->CurrentPageIndex=$param->NewPageIndex;

    $idAlert=$this->metaAlert_->id;

    $params=new CDMQuad();
    $params->value1=$idAlert;
    $params->value2=$this->per_page; //limit
    $params->value3=$this->RelatedAlerts->CurrentPageIndex*$params->value2; //offset
    $alerts=SQLWrapper::queryForList('SelectMetaAlertsChildren', $params);

    $data=array();

    foreach ($alerts as $a)
      if ($a->alertid === null)
        $data[]=array('name'=>$this->makeLinkTo('MetaAlert', $a->metaalertid, $a->name));
      else
        $data[]=array('name'=>$this->makeLinkTo('Alert', $a->alertid, $a->name));

    if (count($data)==0)
      $data[]=array('name'=>'This shouldn\'t be null. Your database seems to be corrupted.');

    $this->RelatedAlerts->DataSource=$data;
    $this->RelatedAlerts->dataBind();
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
