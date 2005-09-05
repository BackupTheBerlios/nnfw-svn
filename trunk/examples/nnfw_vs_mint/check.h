/* $Id: check.h,v 1.4 2005/02/20 21:59:49 stefano Exp $ */

#ifndef MINT_CHECK_H
#define MINT_CHECK_H

#include <stdio.h>
#include <stdlib.h>

/* assert-like marco */
#ifdef NDEBUG
#  define mint_check( condition, message )
#else
#  define mint_check( condition, message ) \
if(!(condition)){\
fprintf(stderr, "%s: %s (%s:%d)\n", __FUNCTION__, message, \
__FILE__, __LINE__ ); abort();}
#endif

#endif
