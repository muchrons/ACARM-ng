<?php

class Layout extends TTemplateControl
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    SQLWrapper::check_dbversion();
    $response=$this->getResponse()->Request;
    $text="$response->UserHostAddress GET $response->RequestUri";
    Prado::log(substr($text,0,255), TLogger::INFO, 'WUI.Sites');
  }
}

?>
