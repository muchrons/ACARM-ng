<?php

class CLocalUserManager extends TModule implements IUserManager
{
  /**
   * @return string name for a guest user.
   */
  public function getGuestName()
  {
    return 'guest';
  }

  /**
   * Returns a user instance given the user name.
   * @param string user name, null if it is a guest.
   * @return TUser the user instance
   */
  public function getUser($username=null)
  {
    $user=new TUser($this);
    if($username !== null)
    {
      $user->setIsGuest(false);
      $user->setName($username);
      $user->setRoles(array('normal'));
    }
    else
      $user->setIsGuest(true);
    return $user;
  }

  /**
   * Validates if the username exists.
   * @param string user name
   * @param string password
   * @return boolean true if validation is successful, false otherwise.
   */
  public function validateUser($username,$password)
  {
    $finder=CUserRecord::finder();
    // TODO: this query throws when non-ASCI chers (i.e. UTF-8) are spotted...
    $tab   =$finder->find('login=? AND pwdhash=?', $username, $password);
    if($tab===null)
      return false;
    return true;
  }

  /**
   * Saves user auth data into a cookie.
   * @param THttpCookie the cookie to receive the user auth data.
   * @since 3.1.1
   */
  public function saveUserToCookie($cookie)
  {
    // do nothing since we don't support cookie-based auth in this example
    throw new TNotSupportedException('saveUserToCookie(): action not supported');
  }

  /**
   * Returns a user instance according to auth data stored in a cookie.
   * @param THttpCookie the cookie storing user authentication information
   * @return TUser the user instance generated based on the cookie auth data, null if the cookie does not have valid auth data.
   * @since 3.1.1
   */
  public function getUserFromCookie($cookie)
  {
    // do nothing since we don't support cookie-based auth in this example
    throw new TNotSupportedException('getUserFromCookie(): action not supported');
    return null;
  }
}


?>
