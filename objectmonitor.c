/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2008 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 252479 2008-02-07 19:39:50Z iliaa $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_objectmonitor.h"

ZEND_DECLARE_MODULE_GLOBALS(objectmonitor)

/* True global resources - no need for thread safety here */
static int le_objectmonitor;

/* {{{ objectmonitor_functions[]
 *
 * Every user visible function must have an entry in objectmonitor_functions[].
 */
const zend_function_entry objectmonitor_functions[] = {
	PHP_FE(objectmonitor_register_object,	NULL)
	PHP_FE(objectmonitor_get_changes, php_objectmonitor_getchanges_arginfo)
	{NULL, NULL, NULL}	/* Must be the last line in objectmonitor_functions[] */
};
/* }}} */

/* {{{ objectmonitor_module_entry
 */
zend_module_entry objectmonitor_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"objectmonitor",
	objectmonitor_functions,
	PHP_MINIT(objectmonitor),
	PHP_MSHUTDOWN(objectmonitor),
	PHP_RINIT(objectmonitor),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(objectmonitor),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(objectmonitor),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_OBJECTMONITOR
ZEND_GET_MODULE(objectmonitor)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("objectmonitor.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_objectmonitor_globals, objectmonitor_globals)
    STD_PHP_INI_ENTRY("objectmonitor.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_objectmonitor_globals, objectmonitor_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_objectmonitor_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_objectmonitor_init_globals(zend_objectmonitor_globals *objectmonitor_globals)
{
	objectmonitor_globals->global_value = 0;
	objectmonitor_globals->global_string = NULL;
}
*/
/* }}} */


void php_objectmonitor_write_property_handler(zval *obj, zval *property, zval *value TSRMLS_DC) {
	zend_object_handle handleOfCurrentObject;
	
	zval* propertyArray;
	zval** propertyArray_p;

	handleOfCurrentObject = Z_OBJ_HANDLE(*obj);

	if (zend_hash_index_find(Z_ARRVAL_P(OBJECTMONITOR_G(list_of_changed_objects)), handleOfCurrentObject, (void **)&propertyArray_p) == FAILURE) {
		// Array entry for current object not found -> create it!
		MAKE_STD_ZVAL(propertyArray);
		array_init(propertyArray);

		// Add the current object to the array, as we might need it lateron.
		add_assoc_zval(propertyArray, "__object", obj);
		zval_addref_p(obj);

		add_index_zval(OBJECTMONITOR_G(list_of_changed_objects), handleOfCurrentObject, propertyArray);
	} else {
		propertyArray = *propertyArray_p;
	}

	// Add element to inner entry
	// as the value is is stored here, we need to increase the ref count of it properly; else, strange things will happen
	zval_addref_p(value);

	add_assoc_zval(propertyArray, Z_STRVAL_P(property), value);

	// call original property handler to actually change the property.
	OBJECTMONITOR_G(original_write_property_handler)(obj, property, value TSRMLS_DC);
	zval_ptr_dtor(&property);
	//zval_ptr_dtor(&value);
}

static void php_objectmonitor_globals_ctor(zend_objectmonitor_globals *objectmonitor_globals TSRMLS_DC) {
	objectmonitor_globals->object_handlers = *zend_get_std_object_handlers();
	objectmonitor_globals->original_write_property_handler = objectmonitor_globals->object_handlers.write_property;
	objectmonitor_globals->object_handlers.write_property = php_objectmonitor_write_property_handler;
}

static void php_objectmonitor_globals_dtor(zend_objectmonitor_globals *objectmonitor_globals TSRMLS_DC) {
	// Destructor.
}
/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(objectmonitor)
{
	#ifdef ZTS
	ts_allocate_id(&objectmonitor_globals_id,
			sizeof(zend_objectmonitor_globals),
			(ts_allocate_ctor)php_objectmonitor_globals_ctor,
			(ts_allocate_dtor)php_objectmonitor_globals_dtor);
	#else
		php_objectmonitor_globals_ctor(&objectmonitor_globals);
	#endif

	

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(objectmonitor)
{
	#ifndef ZTS
		php_objectmonitor_globals_dtor(&objectmonitor_globals);
	#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(objectmonitor)
{
	MAKE_STD_ZVAL(OBJECTMONITOR_G(list_of_changed_objects));
	array_init(OBJECTMONITOR_G(list_of_changed_objects));

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(objectmonitor)
{
	zval_ptr_dtor(&OBJECTMONITOR_G(list_of_changed_objects));
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(objectmonitor)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "objectmonitor support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* {{{ proto string confirm_objectmonitor_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(objectmonitor_register_object)
{
	zval* object_to_register;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &object_to_register) == FAILURE) {
		return;
	}
	php_printf("Refcount on init: %d\n", zval_refcount_p(object_to_register));

	Z_OBJ_HT_P(object_to_register) = &OBJECTMONITOR_G(object_handlers);
	// TODO: do we need to increase the refcount to prevent garbage collection?
}
/* }}} */

/* {{{ proto string confirm_objectmonitor_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(objectmonitor_get_changes)
{
	zval_ptr_dtor(return_value_ptr);
	*return_value_ptr = OBJECTMONITOR_G(list_of_changed_objects);
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
