/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2015 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Dmitry Stogov <dmitry@zend.com>                              |
   +----------------------------------------------------------------------+
 */

#include <zend.h>
#include "zend_smart_str_public.h"

#define SMART_STR_OVERHEAD (ZEND_MM_OVERHEAD + _STR_HEADER_SIZE)

#ifndef SMART_STR_PAGE
# define SMART_STR_PAGE 4096
#endif

#ifndef SMART_STR_START_SIZE
# define SMART_STR_START_SIZE (256 - SMART_STR_OVERHEAD - 1)
#endif

#define SMART_STR_NEW_SIZE(len) \
	(((len + SMART_STR_OVERHEAD + SMART_STR_PAGE) & ~(SMART_STR_PAGE - 1)) - SMART_STR_OVERHEAD - 1)

ZEND_API void ZEND_FASTCALL smart_str_erealloc(smart_str *str, size_t len)
{
	if (UNEXPECTED(!str->s)) {
		str->a = len < SMART_STR_START_SIZE
				? SMART_STR_START_SIZE
				: SMART_STR_NEW_SIZE(len);
		str->s = zend_string_alloc(str->a, 0);
		str->s->len = 0;
	} else {
		str->a = SMART_STR_NEW_SIZE(len);
		str->s = (zend_string *) erealloc2(str->s, _STR_HEADER_SIZE + str->a + 1, _STR_HEADER_SIZE + str->s->len + 1);
	}
}

ZEND_API void ZEND_FASTCALL smart_str_realloc(smart_str *str, size_t len)
{
	if (UNEXPECTED(!str->s)) {
		str->a = len < SMART_STR_START_SIZE
				? SMART_STR_START_SIZE
				: SMART_STR_NEW_SIZE(len);
		str->s = zend_string_alloc(str->a, 1);
		str->s->len = 0;
	} else {
		str->a = SMART_STR_NEW_SIZE(len);
		str->s = (zend_string *) realloc(str->s, _STR_HEADER_SIZE + str->a + 1);
	}
}
