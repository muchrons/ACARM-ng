<?php

// class is derived from prado's TInvalidDataValueException to make
// default output preaty-printed.
class CBaseException extends TInvalidDataValueException
{
  public function __construct($message)
  {
    assert($message!==null);
    parent::__construct($message);
  }

  protected function translate($message)
  {
    // TODO: message translation should go here
    return $message;    // TODO: temporary solution
  }
}

?>
