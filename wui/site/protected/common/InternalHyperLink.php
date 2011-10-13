<?php

class InternalHyperLink extends THyperLink
{
  public function setToPage($value)
  {
    $this->setViewState("ToPage", $value, '');
  }

  public function getToPage()
  {
    return $this->getViewState("ToPage", '');
  }

  /**
   * Set the variables as an array
   * typically: array('key'=>'val')
   *
   * @param array() $value
   */
  public function setGetVariables($value)
  {
    $variables = "";
    if(is_array($value))
      {
        foreach($value as $k=>$v)
          {
            if($variables != "") $variables .="&";
            $variables .= "$k=$v";
          }
      }
    else
      $variables = $value;

    $this->setViewState("GetVariables", $variables, '');
  }

  public function getGetVariables()
  {
    return $this->getViewState("GetVariables", '');
  }

  public function setAnchor($value)
  {
    $this->setViewState("Anchor", $value, null);
  }

  public function getAnchor()
  {
    return $this->getViewState("Anchor", null);
  }

  public function getNavigateUrl()
  {
    $page = $this->getToPage();
    $variables = $this->getGetVariables();
    parse_str($variables, $variablesAsArray);
    $anchor = ($this->getAnchor() !== null ? "#" . $this->getAnchor() : "");
    return $this->getApplication()->getRequest()->constructUrl("page", $page, $variablesAsArray) . $anchor;
  }
}
?>