<?php

class Host extends TPage
{
  public function __construct()
  {
    parent::__construct();
    $this->hostID_=$this->Request->itemAt('id');
    assert( $this->hostID_!==null );
    $this->host_=SQLWrapper::queryForObject('SelectHost', $this->hostID_);
  }

  public function onLoad($param)
  {
    if( $this->host_===null )
      die("invalid host / host not set");
    // initialization of GridData
    if(!$this->IsPostBack)
    {
      // fill fileds with values
      $this->HostName->Text=($this->host_->name === null)?"N/A":$this->host_->name;
      $this->HostIP->Text=$this->host_->ip;

      $this->HostRole->Text=($this->host_->role=="src")?"Source":"Destination";
      $this->HostMask->Text=($this->host_->mask === null)?"N/A":$this->host_->mask;

      $this->HostRefName->Text=($this->host_->ref_name === null)?"N/A":$this->host_->ref_name;
      $this->HostRefURL->Text=($this->host_->ref_url === null)?"N/A":$this->host_->ref_url;

      //Get all services for the host
      $services=SQLWrapper::queryForList('SelectServicesForHost', $this->hostID_);
      $data=array();
      foreach ($services as $s)
        $data[]=array('id'=>$s->id,'name'=>$s->name,'port'=>$s->port, 'protocol'=>$s->protocol,
                      'reference name'=>$s->ref_name, 'reference url'=>$s->ref_url);

      $this->AlertServices->DataSource=$data;
      $this->AlertServices->dataBind();


      //Get all processes for the host
      $processes=SQLWrapper::queryForList('SelectProcessesForHost', $this->hostID_);
      $data=array();
      foreach ($processes as $p)
        $data[]=array('id'=>$p->id,'name'=>$p->name,'port'=>$p->path, 'MD5'=>$p->md5, 'PID'=>$p->pid,
                      'UID'=>$p->uid, 'username'=>$p->username, 'arguments'=>$p->arguments,
                      'reference name'=>$p->ref_name, 'reference url'=>$p->ref_url);

      $this->AlertProcesses->DataSource=$data;
      $this->AlertProcesses->dataBind();
      }
  }
  private $host_;
  private $hostID_;
}

?>
