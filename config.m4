dnl $Id$
dnl config.m4 for extension objectmonitor

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(objectmonitor, for objectmonitor support,
dnl Make sure that the comment is aligned:
dnl [  --with-objectmonitor             Include objectmonitor support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(objectmonitor, whether to enable objectmonitor support,
 Make sure that the comment is aligned:
 [  --enable-objectmonitor           Enable objectmonitor support])

if test "$PHP_OBJECTMONITOR" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-objectmonitor -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/objectmonitor.h"  # you most likely want to change this
  dnl if test -r $PHP_OBJECTMONITOR/$SEARCH_FOR; then # path given as parameter
  dnl   OBJECTMONITOR_DIR=$PHP_OBJECTMONITOR
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for objectmonitor files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       OBJECTMONITOR_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$OBJECTMONITOR_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the objectmonitor distribution])
  dnl fi

  dnl # --with-objectmonitor -> add include path
  dnl PHP_ADD_INCLUDE($OBJECTMONITOR_DIR/include)

  dnl # --with-objectmonitor -> check for lib and symbol presence
  dnl LIBNAME=objectmonitor # you may want to change this
  dnl LIBSYMBOL=objectmonitor # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $OBJECTMONITOR_DIR/lib, OBJECTMONITOR_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_OBJECTMONITORLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong objectmonitor lib version or lib not found])
  dnl ],[
  dnl   -L$OBJECTMONITOR_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(OBJECTMONITOR_SHARED_LIBADD)

  PHP_NEW_EXTENSION(objectmonitor, objectmonitor.c, $ext_shared)
fi
