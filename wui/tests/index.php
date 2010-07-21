<?php
$root     = dirname(__FILE__);
require_once($root.'/../test_framework/test_tools/unit_tests.php');

// ensure tests will pass
set_time_limit(0);
$put      = new PradoUnitTester($root.'/protected/tests', $root.'/protected');
$reporter = new HtmlReporter();
$put->run($reporter)
?>
