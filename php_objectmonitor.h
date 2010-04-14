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

#ifndef PHP_OBJECTMONITOR_H
#define PHP_OBJECTMONITOR_H

extern zend_module_entry objectmonitor_module_entry;
#define phpext_objectmonitor_ptr &objectmonitor_module_entry

#ifdef PHP_WIN32
#	define PHP_OBJECTMONITOR_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_OBJECTMONITOR_API __attribute__ ((visibility("default")))
#else
#	define PHP_OBJECTMONITOR_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(objectmonitor);
PHP_MSHUTDOWN_FUNCTION(objectmonitor);
PHP_RINIT_FUNCTION(objectmonitor);
PHP_RSHUTDOWN_FUNCTION(objectmonitor);
PHP_MINFO_FUNCTION(objectmonitor);

PHP_FUNCTION(objectmonitor_register_object);
PHP_FUNCTION(objectmonitor_get_changes);

ZEND_BEGIN_ARG_INFO_EX(php_objectmonitor_getchanges_arginfo, 0, 1, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_MODULE_GLOBALS(objectmonitor)
    /*
     * The modified object handler table; with write_property wrapped, so to remember what has been written
     */
    zend_object_handlers object_handlers;

    /**
     * The original write property handler to call
     */
    zend_object_write_property_t original_write_property_handler;

    zval* list_of_changed_objects;
ZEND_END_MODULE_GLOBALS(objectmonitor)


/* In every utility function you add that needs to use variables 
   in php_objectmonitor_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as OBJECTMONITOR_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define OBJECTMONITOR_G(v) TSRMG(objectmonitor_globals_id, zend_objectmonitor_globals *, v)
#else
#define OBJECTMONITOR_G(v) (objectmonitor_globals.v)
#endif

#endif	/* PHP_OBJECTMONITOR_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
