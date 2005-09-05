/* $Id: random.h,v 1.2 2005/02/19 09:47:17 stefano Exp $ */

#ifndef MINT_RANDOM_H
#define MINT_RANDOM_H

/* initalize generator */
void mint_random_seed( unsigned long );

/* uniformly distributed in [0,1[ */
float mint_random( void );

/* uniformly distrinuted in [min,max[ */
float mint_random_uniform( float min, float max );

float mint_random_normal( float mean, float stdev );

/* random integer between min and max */
int mint_random_int( int min, int max );

/* stores in perm random permutation of integers between min and max;
   perm must have already been allocated */
void mint_random_permutation( int *perm, int min, int max );

/* returns the number of successes in n Bernoulli trials (coin tosses)
   each with a probability of success of p */
int mint_random_binomial( float p, int n );

#endif

