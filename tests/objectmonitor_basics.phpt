--TEST--
Checks for basic functionality to work
--SKIPIF--
<?php if (!extension_loaded("objectmonitor")) print "skip"; ?>
--FILE--
<?php
class A {
	public $firstProperty;
	public $secondProperty;
}

$instance = new A();
$anotherInstance = new A();

$instance->firstProperty = "This is not recorded";

objectmonitor_register_object($instance);
objectmonitor_register_object($anotherInstance);

$instance->secondProperty = "Change to second property";
$anotherInstance->firstProperty = "Change 1";
$anotherInstance->firstProperty = "Change 2";

var_dump(objectmonitor_get_changes());
?>
--EXPECT--
array(2) {
  [1]=>
  array(2) {
    ["__object"]=>
    object(A)#1 (2) {
      ["firstProperty"]=>
      string(20) "This is not recorded"
      ["secondProperty"]=>
      string(25) "Change to second property"
    }
    ["secondProperty"]=>
    string(25) "Change to second property"
  }
  [2]=>
  array(2) {
    ["__object"]=>
    object(A)#2 (2) {
      ["firstProperty"]=>
      string(8) "Change 2"
      ["secondProperty"]=>
      NULL
    }
    ["firstProperty"]=>
    string(8) "Change 2"
  }
}