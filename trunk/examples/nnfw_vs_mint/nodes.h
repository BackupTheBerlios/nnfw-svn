/* $Id: nodes.h,v 1.8 2005/02/20 10:45:54 stefano Exp $ */

#ifndef MINT_NODES_H
#define MINT_NODES_H

#include <stdio.h>

struct mint_image;

/* a mint_nodes object should be treated as a 2-dim array of floats,
   with the following semantics: 
   
   n[0][i]   --> input to node i
   n[1][i]   --> output of node i
   n[2+k][i] --> k-th state variable of node i

   info such as the number of nodes or states is provided by the
   functions below
*/

typedef float **mint_nodes;

/* create a node group of n nodes, each with s state variables */
mint_nodes mint_nodes_new( int n, int s );

/* destroy a node group, freeing up the memory used */
void mint_nodes_del( mint_nodes );

/* create a duplicate of src */
mint_nodes mint_nodes_dup( const mint_nodes src );

/* make dst equal to src */
void mint_nodes_cpy( mint_nodes dst, const mint_nodes src );

/* load a node group from an open file. The file format is:

   nodes n s
   <update>
   <values and states>

   where: n and s are the number of nodes in the group and the number
   of state variables for each node (in addition to input and output
   variables); <update> is an OPTIONAL update function specification
   (see update.h, a null update function is used if not found);
   <values and states> is an OPTIONAL specification of input, output
   and state values for each node. This usually exists when the file
   has been created via a _save function, but not when it is a user
   written file. The layout of node variables follows the description
   at the top of this file, i.e. the variables of node i are arranged
   as:

   n[0][i] n[1][i] ... n[2+s][i]

 */
mint_nodes mint_nodes_load( FILE * );

/* save a node group to a file */
void mint_nodes_save( const mint_nodes , FILE * );

/* get number of nodes */
int mint_nodes_size( const mint_nodes  );

/* get number of states */ 
int mint_nodes_states( const mint_nodes  );

/* retrieve the update function used for these nodes */
struct mint_update *mint_nodes_get_update( mint_nodes  );

/* set the update function, returns a pointer to it so that, e.g.,
   rule parameter can be set */
struct mint_update *mint_nodes_set_update( mint_nodes , int id );

/* set variable i of all nodes to x */
void mint_nodes_set( mint_nodes n, int i, float x );

#endif

