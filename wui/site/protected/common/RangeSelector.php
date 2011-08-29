<?php

class RangeSelector extends TTemplateControl
{
  function __construct()
  {
    parent::__construct();
    $this->From->Timestamp=strtotime("-1 month");
    $this->To->Timestamp=strtotime("today");
  }

  public function getRangeData()
  {
    $range=new CParamRange();
    $range->date_from=date("Y-m-d 00:00:00",strtotime($this->From->Date));
    $range->date_to=date("Y-m-d 23:59:59",strtotime($this->To->Date));
    $range->severities=implode('.',$this->CB->SelectedValues);
    $range->srct=$this->srcip->Text;
    $range->dstt=$this->dstip->Text;
    $range->type=$this->Type->Text;

    if ($range->dstt=='any')
    {
      $range->dst='0.0.0.0';
      $range->ignoredst=1;
    }
    else
    {
      $range->dst=$range->dstt;
      $range->ignoredst=0;
    }

    if ($range->srct=='any')
    {
      $range->src='0.0.0.0';
      $range->ignoresrc=1;
    }
    else
    {
      $range->src=$range->srct;
      $range->ignoresrc=0;
    }

    if ($range->type=='any')
      $range->ignoretype=1;
    else
      $range->ignoretype=0;

    return $range;
  }

  public function onInit($param)
  {
    parent::onInit($param);
    if($this->view!=null)
      $this->MultiView->ActiveViewIndex=$this->view;
  }

  public function getFrom()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('From');
  }

  public function getTo()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('To');
  }

  public function setView($p)
  {
    $this->view=$p;
  }

  public $view;
}

?>
