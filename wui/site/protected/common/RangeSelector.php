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
    $range->date_from=$this->From->Date;
    $range->date_to=$this->To->Date;;
    $range->severities=implode('.',$this->CB->SelectedValues);
    $range->src=$this->srcip->Text;
    $range->dst=$this->dstip->Text;

    if ($range->dst=='any')
      {
        $range->dst='0.0.0.0';
        $range->ignoredst=1;
      } else
      $range->ignoredst=0;

    if ($range->src=='any')
      {
        $range->src='0.0.0.0';
        $range->ignoresrc=1;
      } else
      $range->ignoresrc=0;

    return $range;
  }

  public function onInit($param)
  {
    parent::onInit($param);
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


}

?>
