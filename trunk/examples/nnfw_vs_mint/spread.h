/* $Id: spread.h,v 1.2 2005/02/20 10:28:20 stefano Exp $ */

#ifndef MINT_SPREAD_H
#define MINT_SPREAD_H

#include <stdio.h>

struct mint_spread;
struct mint_network;

/* create a dummy spread objec that does not contain any spreading
   scheme (as revealed by mint_spread_len() returning 0)*/
struct mint_spread *mint_spread_new( void );

/* destroy a spread object */
void mint_spread_del( struct mint_spread * );

/* duplicate a spread object */
struct mint_spread *mint_spread_dup( const struct mint_spread * );

/* copy a spread object */
void mint_spread_cpy( struct mint_spread *, const struct mint_spread * );

/* load a spread object from file. The format is, for all kinds of
   spreading but the asynchronous one:

   spread <len>
   w1 ... w<len>
   n1 ... n<len>

   where <len> is an integer giving the length of the spread sequence.
   w1 ... w<len> is a sequence of integers stating what weight matrix
   is involved in each step of the spreading sequence. if wi is
   negative, no matrix-vector multiplication is performed; if wi is
   positive, the matrix-vector multiplication relative to weight
   matrix wi is performed (which node groups are involved is stored in
   the weight matrix itself, see weights.h). The node group sequence
   specifies similarly which node group is involved: if ni is
   negative, no node update operation is performed; if ni is positive,
   the node group with index ni is updated. 

   Example: spreading for a two-layer feed-forward network is given
   by:
 
   spread 2
   -1 0
   0 1

   meaning: update node group 0 (input nodes), perform the
   matrix-vector multiplication involving the (only) weight matrix
   (with index 0), update node group 1 (output nodes). Note that the
   sequence can be compressed in just two steps because matrix-vector
   multiplications are performed before node update. An equivalent
   sequence, one step longer, is:

   spread 3
   -1 0 -1
   0 -1 1

   In general, if ni is -1 and wi+1 is also -1, the sequence can be
   shortened by one step replacing the pairs (ni,wi) (ni+1,wi+1) with
   (wi,ni+1).
*/
struct mint_spread *mint_spread_load( FILE * );

/* save a spread objec to file, according to the above format */
void mint_spread_save( struct mint_spread *, FILE * );

/* retrieve length of spread sequence */
int mint_spread_len( struct mint_spread * );

/* retrieve the node group index for step i of the sequence */
int mint_spread_nodes( struct mint_spread *, int i );

/* retrieve the weight matrix index for step i of the sequence */
int mint_spread_weights( struct mint_spread *, int );

/* set feedforward network dynamics: a single call to
   mint_network_update fully propagates the input to the output
   group(s) */
void mint_spread_feedforward( struct mint_network * );

/* set synchronous network dynamics: the input to all nodes is
   calculated, then the node update functions are called  */
void mint_spread_synchronous( struct mint_network * );

/* set asynchronous network dynamics: seqlen randomly picked nodes are
   updated at each call of mint_network_nupdate */
void mint_spread_asynchronous( struct mint_network *, int len );

/* set up a custom update sequence for network nodes; len is the
   sequence length and the variable arguments are a list of 2*len
   integers that specify the weight matrix and node groups sequences,
   according to the meaning explained in mint_spread_load above.
*/
void mint_spread_custom( struct mint_network *, int len, ... );

#endif
