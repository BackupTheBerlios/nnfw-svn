
#ifndef MINT_NET_ARCH
#define MINT_NET_ARCH

#include <stdio.h>

/* defines a structure to describe network architectures; such
   structures are used to build networks; it is advised to build archs
   from text description in files - see descprition of file format at
   end of file.

   NOTE: modifying the arch structure of an already built network
   results in undefined behavior (i.e.: only troubles).
*/

struct mint_arch {
  int groups;     /* number of node groups */
  int matrices;   /* number of weight matrices */
  int *size;      /* group sizes */
  int *nstates;   /* nstates[i] = number of states of nodes in group i */
  int *wfrom;     /* wfrom[i] = which group matrix i comes from */
  int *wto;       /* wto[i] = which group matrix i goes to */
  int *wstates;   /* number of states for weights in matrix[i] */
};

/* functions for arch management */
struct mint_arch *mint_arch_new( int groups, int matrices ); 
void mint_arch_del( struct mint_arch * );
void mint_arch_cpy( struct mint_arch *a1, const struct mint_arch *a2 );       

int mint_arch_same( struct mint_arch *a1, const struct mint_arch *a2 );

struct mint_arch *mint_arch_load( FILE * );
void mint_arch_save( struct mint_arch *, FILE * );

/* file format for arch structures (examples follow):

   # arch N M name
   n_1 s_1
   ...
   n_N s_M
   wf_1 wt_1 ws_1
   ...
   wf_M wt_M ws_M

   where: N is the number of node groups; M the number of weight
   matrices; name is a string shorter than 1024 characters; then
   follow node description lines, one for each node group, in the
   format "<number of nodes> <number of state variables per node>";
   then follow weight description lines, one for each weight matrix,
   in the format "<index of pre-synaptic node group> <index of
   post-synaptic node group> <numer of state variabels for each
   weight>".

   EXAMPLE 1: a feed forward architecture with 10 input, 5 hidden and
   1 output nodes, nodes and weights are stateless

   # arch 3 2 feed_forward
   10 0
   5 0 
   1 0
   0 1 0
   1 2 0

   EXAMPLE 2: a stateless input group connected to a processing group
   which has recurrent connections and one state variable for each
   node; weights are stateless

   # arch 2 2 recurrent
   10 0
   5 1
   0 1 0
   1 1 0

 */

#endif
