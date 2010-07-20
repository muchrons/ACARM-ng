<?php

//
// this is helper class that allows doing regular expressions that
// ommit national characters, since UTF-8 regexps does NOT work in
// PHP as for now (@&$^*#@$^&@$#%@^&$^&#!!!!).
//
// this is rather a hack than a solution, but it helps in defined number
// of cases. in general it replaces all known (i.e. hardcoded) UTF8
// characters to other, known ASCII characters that regular preg_match()
// can handle.
//
class CRegExpMatcher
{
  // returns true/false
  static public function pregMatch($pattern, $string)
  {
    $tmp=CUTFCleaner::removeUTFs($string);
    return preg_match($pattern, $tmp)===1;
  }
}

?>
