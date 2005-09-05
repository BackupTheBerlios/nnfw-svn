/* $Id: load_utils.h,v 1.2 2005/02/20 14:42:26 stefano Exp $ */

#ifndef MINT_LOAD_UTILS_H
#define MINT_LOAD_UTILS_H

#include <stdio.h>
#include <ctype.h>

/* advance until a non space character, the latter remains available
   for reading and is also returned */
int mint_skip_space( FILE *file );

/* returns 1 if the first non-space charachter is a digit, a dot or a
   minus sign, otherwise returns 0; the character remains available
   for reading */
int mint_values_waiting( FILE *file );

#endif
