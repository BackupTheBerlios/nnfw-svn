/* $Id: weights.h,v 1.5 2005/02/20 10:49:55 stefano Exp $ */

#ifndef MINT_WEIGHTS_H
#define MINT_WEIGHTS_H

#include "nodes.h"

#include <stddef.h>
#include <stdio.h>

/* a weights object should be treated as a 3-dim array of floats, with
   the following semantics:

   w[0][i][j]   --> value of weight (i,j)
   w[1+k][i][j] --> k-th state variable of weight (i,j)

   info such as the number of rows, columns and states is provided by
   the functions below
*/

typedef float ***mint_weights;

/* create a weight matrix with given rows, columns and states */
mint_weights mint_weights_new( size_t rows, size_t cols, size_t states );

/* destroy a weight matrix, freeing up used memory */
void mint_weights_del( mint_weights  );

/* duplicate src */
mint_weights mint_weights_dup( const mint_weights src );

/* make dst equal to src */ 
void mint_weights_cpy( mint_weights dst, mint_weights src );

/* load a weight matrix from an open file. The file format is:

   weights r c s
   <update>
   <values and states>

   where: r and c are the number of rows and columns in the weight
   matrix, and s is the number of state variables for each weight (in
   addition to its value); <update> is an OPTIONAL update function
   specification (see update.h, a null update function is used if not
   found); <values and states> is an OPTIONAL specification of weight
   values and state variables for each weight. This usually exists
   when the file has been created via a _save function, but not when
   it is a user written file. The layout of weight variables follows
   the description at the top of this file, i.e. the variables for
   weight (i,j) are arranged as:

   w[0][i][j] ... w[1+s][i][j]

   The column index is incremented faster, i.e. the weight are read
   (and stored by _save) in the sequence:

   (1,1), (1,2) ... (1,c), (2,1) ... (2,c), ... , (r,1) ... (r,c).

 */
mint_weights mint_weights_load( FILE * );

/* save a weight matrix to file */
void mint_weights_save( mint_weights , FILE * );

/* get the number of rows, columns and states */
size_t mint_weights_rows( mint_weights  );
size_t mint_weights_cols( mint_weights  );
size_t mint_weights_states( mint_weights  );

/* index of 'pre-synaptic' node group */
int mint_weights_get_from( const mint_weights w );
void mint_weights_set_from( mint_weights w, int );

/* index of 'post-synaptic' node group */
int mint_weights_get_to( const mint_weights w );
void mint_weights_set_to( mint_weights w, int );


/* get and set the update function */
const struct mint_update *mint_weights_get_update( const mint_weights );
void mint_weights_set_update( mint_weights , const struct mint_update * );

/* filter the output of nfrom nodes through the weight matrix w, and
   add the result to the input of nodes nto */
void mint_weights_mult( const mint_weights w, 
			const mint_nodes nfrom, const mint_nodes nto );

/* as above, but calculate inly the input to node i of nto */
void mint_weights_multrow( const mint_weights w, 
			   mint_nodes nfrom, 
			   mint_nodes nto, int i );

#endif

