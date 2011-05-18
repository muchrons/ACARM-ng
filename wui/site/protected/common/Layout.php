<?php

class Layout extends TTemplateControl
{
  public function onLoad($param)
  {
    parent::onLoad($param);
    $response=$this->getResponse()->Request;
    Prado::log("$response->UserHostAddress GET $response->RequestUri", TLogger::INFO, 'WUI.Sites');
  }
}

?>
