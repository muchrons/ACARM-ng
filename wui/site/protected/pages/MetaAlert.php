<?php

class MetaAlert extends TPage
{
  private $per_page=100; //number of alerts displayed per page

  public function __construct()
  {
    parent::__construct();
    $mid=$this->Request->itemAt('mid');
    $aid=$this->Request->itemAt('aid');

    $msysid=$this->Request->itemAt('sys_id');

    if ($msysid!=null)
      $mid=CSQLMap::get()->queryForObject('MetaAlert_SysID_ID', $msysid);

    $this->group_count_=10;
    $this->max_children_=50;

    if ($mid===null && $aid===null)
      throw new TConfigurationException("No valid Meta Alert ID or Alert ID specified");

    if ($mid!=null)
      {//we have a MetaAlertID
        $this->metaAlert_=SQLWrapper::queryForObject('SelectMetaAlertID', $mid);

        if($this->metaAlert_->id_alert != null)
          $this->alert_=SQLWrapper::queryForObject('SelectAlert', $this->metaAlert_->id_alert);
      }
    else
      {//we have an AlertId
        $this->alert_=SQLWrapper::queryForObject('SelectAlert', $aid);
        $this->metaAlert_=SQLWrapper::queryForObject('SelectMetaAlertID', $this->alert_->id_meta_alert);
      }
  }

  public function onLoad($param)
  {
    if($this->metaAlert_===null && $this->alert_===null)
      die("invalid (meta-)alert ID");

    if(!$this->IsPostBack)
      {
        if($this->metaAlert_!=null)
          {
            $this->MetaAlert->setVisible(true);
            $this->fillMetaAlertForm();
          }

        //show either Alarm form or RelatedAlerts list
        if($this->alert_!=null)
          {
            $this->Alert->setVisible(true);
            $this->fillAlertForm();
          }
        else
          {
            $this->AlertList->setVisible(true);
            $this->fillRelatedAlerts();
          }
      }
  }

  private function fillMetaAlertForm()
  {
    $this->MetaAlertCreateTime->Text=$this->metaAlert_->create_time;
    $this->MetaAlertUpdateTime->Text=$this->metaAlert_->last_update_time;
    $this->SeverityDelta->Text=$this->metaAlert_->severity_delta;
    $this->CertaintyDelta->Text=$this->metaAlert_->certainty_delta;

    $inUse=SQLWrapper::queryForObject('CheckInUse', $this->metaAlert_->id);
    if ($inUse!=0)
      $this->MetaAlertTags->Text.="<font color=\"yellow\"><b>In use</b></font></br>";

    $isRoot=SQLWrapper::queryForObject('CheckRoot', $this->metaAlert_->id);
    if ($isRoot!=0)
      $this->MetaAlertTags->Text.="<font color=\"red\"><b>Root</b></font> ";

    $triggers_tab=SQLWrapper::queryForList('SelectTriggered', $this->metaAlert_->id);

    $content="";

    foreach ($triggers_tab as $t)
      {
        $temp=explode("_",$t->trigger."_",2);
        $content.="<table border=\"0\" cellpadding=\"2\">
                     <td align=\"center\">
                       <img height=\"48\" width=\"48\" src=\"pics/triggers/$temp[0].png\" alt=\"$temp[0]\">
                     </td>";
        $content.="<td>".substr($temp[1],0,-1)."</td></table>";
      }

    $this->Triggered->Text=$content;

    $filters=preg_split('/\s+/', $this->metaAlert_->name);

    $this->MetaAlertName->Text="";

    foreach($filters as $f)
      if ($f=='[many2one]')
        $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/mto.png\" border=0> ";
    elseif ($f=='[one2many]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/otm.png\" border=0> ";
    elseif ($f=='[one2one]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/oto.png\" border=0> ";
    elseif ($f=='[samename]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/name.png\" border=0> ";
    elseif ($f=='[many2many]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/mtm.png\" border=0> ";
    elseif ($f=='[similarity]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/similarity.png\" border=0> ";
    elseif ($f=='[usersmonitor]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/sameuser.png\" border=0> ";
    elseif ($f=='[eventchain]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/eventchain.png\" border=0> ";
    elseif ($f=='[ipblacklist]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/ipblacklist.png\" border=0> ";
    elseif ($f=='[newevent]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/newevent.png\" border=0> ";
    elseif ($f=='[python]')
      $this->Correlated->Text.="<img height=\"48\" width=\"48\" src=\"pics/filters/python.png\" border=0> ";
    else
      $this->MetaAlertName->Text.=$f." ";
  }

  private function fillRelatedAlerts()
  {
    $tree=$this->RelatedAlertsTree;
    $tree->setTitle($this->MetaAlertName->Text);
    $tree->setNodeType(MyTreeList::NODE_TYPE_PLAIN);

    $children=SQLWrapper::queryForList('SelectMetaAlertsChildrenFull', $this->metaAlert_->id);
    $children_count=count($children);

    $children_groupped_tmp=CSQLMap::get()->queryForList('SelectMetaAlertsChildrenFullGroup', $this->metaAlert_->id);

    $children_groupped=array();

    foreach($children_groupped_tmp as $child)
      $children_groupped[$child->name]=$child->id;

    if($children_count==0)
      {
        $tree->setCanDeploy(false);
        return $tree;
      }

    if(count($children_groupped)>=$this->max_children_)
      {
        $tree->addSubElement($this->addGrouppedMetaAlert("A great number of different meta-alerts are here.",count($children_groupped),$this->metaAlert_->id,false));
        return $tree;
      }

    $added=array();
    foreach($children as $child)
      if ($children_groupped[$child->name]<$this->group_count_)
        $tree->addSubElement($this->addSubtreeForMetaAlert($child));
    elseif(!isset($added[$child->name]))
      {
        $tree->addSubElement($this->addGrouppedMetaAlert($child->name,$children_groupped[$child->name],$this->metaAlert_->id,true));
        $added[$child->name]=true;
      }
  }

  private function addSubtreeForMetaAlert($ma)
  {
    $children=CSQLMap::get()->queryForList('SelectMetaAlertsChildrenFull', $ma->metaalertid);
    $children_count=count($children);

    $children_groupped_tmp=CSQLMap::get()->queryForList('SelectMetaAlertsChildrenFullGroup', $ma->metaalertid);

    $children_groupped=array();
    foreach($children_groupped_tmp as $child)
      $children_groupped[$child->name]=$child->id;

    $tree=new MyTreeList();
    $tree->setTitle($ma->name);
    $tree->setIcon("show");
    $tree->setNodeType(MyTreeList::NODE_TYPE_LINK);
    $tree->setToPage("MetaAlert");
    $tree->setGetVariables(array("mid"=>"$ma->metaalertid"));

    if($children_count==0)
      {
        $tree->setCanDeploy(false);
        return $tree;
      }

    if(count($children_groupped)>=$this->max_children_)
      {
        $tree->addSubElement($this->addGrouppedMetaAlert("A great number of different meta-alerts are here.",$children_groupped[$child->name],$ma->metaalertid,false));
        return $tree;
      }

    $added=array();
    foreach($children as $child)
      if ($children_groupped[$child->name]<$this->group_count_)
        {
          if ($child->alertid!=null)
            {//If this is an alert terminate recurrent call.
              $node=new MyTreeList();
              $node->setTitle($child->name);
              $node->setNodeType(MyTreeList::NODE_TYPE_LINK);
              $node->setToPage("MetaAlert");
              $node->setGetVariables(array("aid"=>"$child->alertid"));
              $node->setIcon("show");
              $node->setCanDeploy(false);
              $tree->addSubElement($node);
            }
          else
            $tree->addSubElement($this->addSubtreeForMetaAlert($child));
        }
      else
        {
          if(!isset($added[$child->name]))
            {
              $tree->addSubElement($this->addGrouppedMetaAlert($child->name,$children_groupped[$child->name],$ma->metaalertid,true));
              $added[$child->name]=true;
            }
        }
    return $tree;
  }

  private function addGrouppedMetaAlert($name, $count, $mid, $includetype)
  {
    $tree=new MyTreeList();
    $tree->setTitle($name." x ".$count." ");
    $tree->setNodeType(MyTreeList::NODE_TYPE_LINK);
    $tree->setCanDeploy(false);
    $tree->setToPage("MetaAlerts");
    if ($includetype)
      $tree->setGetVariables(array("parent"=>"$mid","type"=>"$name"));
    else
      $tree->setGetVariables(array("parent"=>"$mid"));
    $tree->setIcon("showall");
    return $tree;
  }

  private function fillAlertForm()
  {
    if ($this->alert_->detect !=null)
      $this->AlertDetectTime->Text =$this->alert_->detect;

    if ($this->alert_->create !=null)
      $this->AlertCreateTime->Text =$this->alert_->create;

    $this->AlertCertainty->Text=($this->alert_->certainty === null) ? "N/A" : $this->alert_->certainty;

    if($this->alert_->description!= null)
      $this->AlertDescription->Text=$this->alert_->description;

    if($this->alert_->severity!= null)
      {
        $text="<font";
        $value=trim($this->alert_->severity);

        if ($value=="4")
          $text.=" color=\"red\"";
        elseif ($value=="3")
          $text.=" color=\"#CC3300\"";
        elseif ($value=="2")
          $text.=" color=\"black\"";
        elseif ($value=="1")
          $text.=" color=\"green\"";
        elseif ($value=="0")
          $text.=" color=\"blue\"";

        $text.=">".$this->severityToName($value)."</font>";
        $this->AlertSeverity->Text=$text;
      }

    //Get all analyzers for the alert
    $analyzers=SQLWrapper::queryForList('SelectAnalyzersForAlert', $this->alert_->id);
    $data=array();
    foreach ($analyzers as $a)
      $data[]=array('link'=>$this->makeAnalyzerLink($a->id),'name'=>$a->name,'IP'=>$a->ip,'ver'=>$a->version, 'OS'=>$a->os);
    //data can be null
    $this->AlertAnalyzers->DataSource=$data;
    $this->AlertAnalyzers->dataBind();

    //Get all hosts for the alert
    $hosts=SQLWrapper::queryForList('SelectHostsForAlert', $this->alert_->id);
    $sources=array();
    $destinations=array();
    foreach ($hosts as $h)
      if ($h->role == "src")
        $sources[]=array('link'=>$this->makeHostLink($h->id),'name'=>$h->name,'IP'=>$h->ip);
      else
        $destinations[]=array('link'=>$this->makeHostLink($h->id),'name'=>$h->name,'IP'=>$h->ip);

    $this->AlertSources->DataSource=$sources; //can be null
    $this->AlertSources->dataBind();

    $this->AlertDestinations->DataSource=$destinations; //can be null
    $this->AlertDestinations->dataBind();
  }

  private function severityToName($n)
  {
    if ($n=="0")
      return "debug";
    if ($n=="1")
      return "info";
    if ($n=="2")
      return "low";
    if ($n=="3")
      return "medium";
    if ($n=="4")
      return "high";
    return "unknown";
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
        $data[]=array('name'=>$this->makeLinkTo('MetaAlert', "mid", $a->metaalertid, $a->name));
      else
        $data[]=array('name'=>$this->makeLinkTo('MetaAlert', "aid", $a->alertid, $a->name));

    if (count($data)==0)
      $data[]=array('name'=>'This shouldn\'t be null. Your database seems to be corrupted.');

    $this->RelatedAlerts->DataSource=$data;
    $this->RelatedAlerts->dataBind();
  }

  private function makeLinkTo($page, $id_name, $id, $name)
  {
    $url =$this->Service->constructUrl( $page, array($id_name => $id) );
    $href="<a href=\"$url\">$name</a>";
    return $href;
  }

  private function makeHostLink($id)
  {
    $out=$this->makeLinkTo('Host', "id", $id, '<img src="pics/dot.png" border="0">');
    return $out;
  }

  private function makeAnalyzerLink($id)
  {
    $out=$this->makeLinkTo('Analyzer', "id", $id, '<img src="pics/dot.png" border="0">');
    return $out;
  }

  private $max_children_;
  private $group_count_;
  private $alert_;
  private $metaAlert_;
}

?>
