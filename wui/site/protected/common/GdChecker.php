<?php

class GdChecker
{
  function __construct()
  {
    if (!extension_loaded('gd') || !function_exists('gd_info'))
      {
        echo "<html><head></head><body><h3 style=\"color:red;\">ERROR:</h3>";
        echo "<p>It looks like GD is not installed. GD library is ";
        echo "necessary to draw charts. On some distributions you will find GD in package ";
        echo "php5-gd, otherwise you need to downaload it from <a href=\"http://www.libgd.";
        echo "org\">http://www.libgd.org</a> and install manually.</p></body></html>";
        die();
      }
  }
}
?>