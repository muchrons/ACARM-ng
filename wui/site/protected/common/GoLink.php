<?php

class GoLink extends TTemplateControl
{
  function __construct()
  {
    parent::__construct();
  }

  public function setToPage($value,$variables)
  {
    $url=$this->getApplication()->getRequest()->constructUrl("page", $value, $variables);
    $this->Link->setNavigateUrl($url);
  }

  public function setCssClass($class)
  {
    $this->Link->setCssClass($class);
    $this->Label->setCssClass($class);
  }

  public function setDeployID($id)
  {
    $this->Label->Attributes->OnClick="toggleSub('".$id."')";
    $this->Label->setStyle("cursor:pointer;");
  }

  public function setText($text)
  {
    $this->Label->setText($text);
  }

  public function getLabel()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('Label');
  }

  public function getLink()
  {
    $this->ensureChildControls();
    return $this->getRegisteredObject('Link');
  }

  public function setIcon($icon)
  {
    // TODO: consider checking if $icon is null, rather than some arbitarry magic value
    if($icon!="none")
      {
        $this->Image->ImageUrl="pics/".$icon."D.png";
        $this->Image->Attributes["onMouseOver"]="$(this).src='pics/".$icon."U.png'";
        $this->Image->Attributes["onMouseOut"]="$(this).src='pics/".$icon."D.png'";
        $this->Image->setVisible(true);
      }
    else
      {
        $this->Image->setVisible(false);
      }
  }

}
?>