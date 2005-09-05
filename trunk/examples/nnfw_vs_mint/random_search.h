/* $Id: random_search.h,v 1.2 2005/02/19 09:47:17 stefano Exp $ */

struct mint_network;

float 
random_search( struct mint_network *net1,  /* initial network */
	       float (*perf)( struct mint_network * ), /* perf function */
	       void (*mut)( struct mint_network * ),   /* mutation rule */
	       float target,             /* target performance */
	       long max                  /* max iterations */
	       );

