/* $Id: nupdate_lib.h,v 1.2 2005/02/13 16:16:01 stefano Exp $ */

#ifndef N_UPDLIB_H
#define N_UPDLIB_H

#include "update.h"
#include "nodes.h"
#include <stddef.h>

/* linear activation function */
void mint_node_linear( mint_nodes n, int min, int max, float *p );

/* linear bounded between param[0] and param[1] */
void mint_node_bounded_linear( mint_nodes n, int min, int max, float *p );

/* sigmoidal non-linearity; the output to zero input and the maximum
   slope can be set */
void mint_node_sigmoid( mint_nodes n, int min, int max, float *p );

/* stochastic activation: 1 with Pr = n_in(n), 0 with Pr = 1 - n_in(n)
 (n_in(n)<0 and n_in(n)>1 are considered as 0 as 1, respectively */
void mint_node_binary( mint_nodes n, int min, int max, float *p );

/* leaky integrator with time constant param[0] */
void mint_node_integrator( mint_nodes n, int min, int max, float *p );

#endif
