<?php
// Getting the default timezone
date_default_timezone_set('UTC');

$basePath=dirname(__FILE__);
$frameworkPath=$basePath.'/prado/prado.php';

// The following directory checks may be removed if performance is required
$assetsPath=$basePath.'/assets';
$runtimePath=$basePath.'/protected/runtime';

if(!is_file($frameworkPath))
  die("Unable to find prado framework path $frameworkPath.");
if(!is_writable($assetsPath))
  die("Please make sure that the directory $assetsPath is writable by Web server process.");
if(!is_writable($runtimePath))
  die("Please make sure that the directory $runtimePath is writable by Web server process.");


require_once($frameworkPath);

$application=new TApplication;
$application->run();

?>
