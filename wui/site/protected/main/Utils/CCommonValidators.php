<?php

class CCommonValidators
{
  static public function isIP($ip)
  {
    // NOTE: based on http://komunitasweb.com/2009/03/10-practical-php-regular-expression-recipes/
    if($ip===null)
      return false;

    // function to validate ip address format in php by Roshan Bhattarai(http://roshanbh.com.np)
    // first of all the format of the ip address is matched
    if( preg_match("/^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/", $ip)!==1 )
      return false;

    // now all the intger values are separated
    $parts=explode(".",$ip);
    // now we need to check each part can range from 0-255
    foreach($parts as $ip_part)
    {
      if(intval($ip_part)>255 || intval($ip_part)<0)
        return false; // if number is not within range of 0-255
    }
    // looks fine
    return true;
  }

  static public function isHost($host)
  {
    if($host===null)
      return false;

    if( preg_match('/^([a-z][a-z0-9_-]*)+(\.[a-z][a-z0-9_-]*)*$/', $host)!==1 )
      return false;

    return true;
  }

  static public function isEmail($email)
  {
    if($email===null)
      return false;

    // breakte e-mail into user name and host name parts
    $parts=explode('@',$email);
    // invalid number of blocks?
    if( count($parts)!=2 )
      return false;
    $name=$parts[0];
    $host=$parts[1];

    // validate username
    if( preg_match('/^[a-z][a-z0-9_\-\.]+[^\.\-_]$/', $name)!==1 )
      return false;

    // validate host part
    if( !CCommonValidators::isHost($host) )
      return false;

    return true;
  }
}

?>
