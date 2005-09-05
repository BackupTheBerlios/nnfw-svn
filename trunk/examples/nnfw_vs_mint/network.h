/* $Id: network.h,v 1.8 2005/03/03 13:44:43 stefano Exp $ */

#ifndef MINT_NETWORK_H
#define MINT_NETWORK_H

#include "nodes.h"
#include "weights.h"
#include <stdio.h>

struct mint_network;

/* there is no mint_network_new function, because networks are created
   either reading from file (either a short description resulting in a
   network with null values for weights and nodes, or a previously
   saved network) or duplicating other networks via
   mint_network_dup */

/* destroy a network, freeing up used memory */
void mint_network_del( struct mint_network *net );

/* duplicate a network */
struct mint_network *mint_network_dup( const struct mint_network * );

/* make net1 equal to net2 */
void mint_network_cpy( struct mint_network *net1, 
		       const struct mint_network *net2 );

/* save a network to an open file */
void mint_network_save( const struct mint_network *net, FILE *dest );

/* load a network from an open file. The file format is:

   network g m
   <spread scheme>
   <node groups>
   <weight matrices>

   where: g and m are the number of node groups and weight matrices;
   <spread scheme> is an OPTIONAL spreading scheme (synchronous
   spreading is set if not specified, may be changed later with
   spread.h functions); <node groups> consists of g node group
   specifications; <weight matrices> consists of m weight matrix
   specifications. Both may be either in short form, giving only
   architectural details such as number of nodes and state variables,
   or in long form giving values for all nodes/weights. See nodes.h
   and weights.h for details.
*/
struct mint_network *mint_network_load( FILE * );

/* number of node groups in this network */
int mint_network_groups( struct mint_network *n );

/* number of weight matrices in this network */
int mint_network_matrices( struct mint_network *n );

/* access node group i of the network (the order is as given by the
   arch object) */
mint_nodes mint_network_nodes( struct mint_network *, int i );

/* access weight matrix i of the network (the order is as given by the
   arch object) */
mint_weights mint_network_weights( struct mint_network *, int i );

/* update network nodes, according to the current update scheme, see
   below */
void mint_network_nupdate( struct mint_network * );

/* update network weights, invoking the wupdate functions on all
   weights */
void mint_network_wupdate( struct mint_network *net );

/* retrieve the spread object associated with this network */
struct mint_spread *mint_network_get_spread( struct mint_network *net );

/* set the spread object (it is advisable to use the functions in
   spread.h, though) */
void mint_network_set_spread( struct mint_network *net,
			      struct mint_spread *s );

/* create a new network by joining two existing ones the n1 node group
   of the first network is set to send its output to the n2 group of
   the second network, by creating an appropriate weight matrix, whose
   weigths are initially set to 1/s1, where s1 is the size of n1. Each
   weight will have s state variables. No additional weight matrix is
   created if both n1 and n2 are -1 (it is an error to use -1 only for
   one parameter). The two component networks remain valid after the
   call, and must be destroyed if they are no longer needed */
struct mint_network *mint_network_join( const struct mint_network *net1,
					int n1,
					const struct mint_network *net2,
					int n2, int s );

#endif
