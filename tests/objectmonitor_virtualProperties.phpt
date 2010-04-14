--TEST--
Checks that when using virtual properties with __set, only the change on the real property is recorded. ( TODO! )
--SKIPIF--
<?php print "skip";
 if (!extension_loaded("objectmonitor")) print "skip"; ?>
--FILE--
<?php
class A {
	protected $firstProperty;
	public function __set($name, $val) {
		$this->firstProperty = $name . ':' . $val;
	}
}

$instance = new A();
objectmonitor_register_object($instance);

$instance->someProperty = "Record";

var_dump(objectmonitor_get_changes());
?>
--EXPECT--
array(1) {
  [1]=>
  array(2) {
    ["__object"]=>
    object(A)#1 (1) {
      ["firstProperty":protected]=>
      string(19) "someProperty:Record"
    }
    ["firstProperty"]=>
    string(19) "someProperty:Record"
  }
}