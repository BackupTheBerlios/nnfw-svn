/* $Id: utils.h,v 1.1 2005/03/08 08:21:48 stefano Exp $ */

#ifndef MINT_UTILS_H
#define MINT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* assert-like marco */
#ifdef NDEBUG
#  define mint_check( condition, message )
#else
#  define mint_check( condition, message ) \
if(!(condition)){\
fprintf(stderr, "%s: %s (%s:%d)\n", __FUNCTION__, message, \
__FILE__, __LINE__ ); abort();}
#endif

/* advance until a non space character, the latter remains available
   for reading and is also returned */
int mint_skip_space( FILE *file );

/* returns 1 if the first non-space charachter is a digit, a dot or a
   minus sign, otherwise returns 0; the character remains available
   for reading */
int mint_values_waiting( FILE *file );

#endif
