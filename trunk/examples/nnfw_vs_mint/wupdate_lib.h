/* $Id: wupdate_lib.h,v 1.3 2005/02/19 09:47:17 stefano Exp $ */

#ifndef MINT_WUPDATE_LIB_H
#define MINT_WUPDATE_LIB_H

#include "weights.h"
#include "nodes.h"

void mint_weight_correlation( mint_weights w, mint_nodes pre, 
			      mint_nodes post, float *p );
#endif
