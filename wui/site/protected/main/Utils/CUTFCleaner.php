<?php

//
// this class is rather a hack than a solution, but it helps in defined number
// of cases. in general it replaces all known (i.e. hardcoded) UTF8
// characters to other, known ASCII characters.
//
class CUTFCleaner
{
  // remove all known chars from string
  static public function removeUTFs($in)
  {
    // null transforms to null :)
    if($in===null)
      return null;

    // switch to nocale sutable for translation
    $out       =null;
    $prevLocale=setlocale(LC_CTYPE, null);
    assert($prevLocale!==null);
    setlocale(LC_CTYPE, "en_US");
    try
    {
      // convert in secure invironment
      $out=iconv("UTF-8", "ASCII//TRANSLIT", $in);
    }
    catch(Exception $e)
    {
      // whe should never be here...
      setlocale(LC_CTYPE, $prevLocale);
      assert("oops - iconv() thrown an error... :/"===null);
    }
    // return proper locale's value and return converted string.
    setlocale(LC_CTYPE, $prevLocale);
    assert( setlocale(LC_CTYPE, null)===$prevLocale );
    assert($out!==null);
    return $out;
  }
}

?>
