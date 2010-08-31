<?php

class MetaAlertReport extends TPage
{
  public function __construct()
  {
    parent::__construct();
    // NOTE: ID here is sys-ID, not data-base ID!
    $id=$this->Request->itemAt('id');
    assert( $id!==null );
    // maps sys-ID to database-ID:
    $this->dbid_=CSQLMap::get()->queryForObject('MapSysIDtoDatabaseID', $id);
    assert( $this->dbid_!==null );
  }

  public function onLoad()
  {
    // redirect to mapped page...
    $url=$this->Service->constructUrl( "MetaAlert", array('id' => $this->dbid_) );
    $this->Response->redirect($url);
  }

  private $dbid_;
}

?>
