--TEST--
Checks for basic functionality to work, especially if property changes are also recorded
when they occur *inside* an object (via $this)
--SKIPIF--
<?php if (!extension_loaded("objectmonitor")) print "skip"; ?>
--FILE--
<?php
class A {
	protected $firstProperty;
	private $secondProperty;

	public function changeFirst($changeTo) {
		$this->firstProperty = $changeTo;
	}

	public function changeSecond($changeTo) {
		$this->secondProperty = $changeTo;
	}
}

$instance = new A();
$anotherInstance = new A();

$instance->changeFirst("This is not recorded");

objectmonitor_register_object($instance);
objectmonitor_register_object($anotherInstance);

$instance->changeSecond("Change to second property");
$anotherInstance->changeFirst("Change 1");
$anotherInstance->changeFirst("Change 2");

var_dump(objectmonitor_get_changes());
?>
--EXPECT--
array(2) {
  [1]=>
  array(2) {
    ["__object"]=>
    object(A)#1 (2) {
      ["firstProperty":protected]=>
      string(20) "This is not recorded"
      ["secondProperty":"A":private]=>
      string(25) "Change to second property"
    }
    ["secondProperty"]=>
    string(25) "Change to second property"
  }
  [2]=>
  array(2) {
    ["__object"]=>
    object(A)#2 (2) {
      ["firstProperty":protected]=>
      string(8) "Change 2"
      ["secondProperty":"A":private]=>
      NULL
    }
    ["firstProperty"]=>
    string(8) "Change 2"
  }
}