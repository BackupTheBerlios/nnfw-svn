/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
 *                                                                              *
 *  This program is free software; you can redistribute it and/or modify        *
 *  it under the terms of the GNU General Public License as published by        *
 *  the Free Software Foundation; either version 2 of the License, or           *
 *  (at your option) any later version.                                         *
 *                                                                              *
 *  This program is distributed in the hope that it will be useful,             *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 *  GNU General Public License for more details.                                *
 *                                                                              *
 *  You should have received a copy of the GNU General Public License           *
 *  along with this program; if not, write to the Free Software                 *
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA  *
 ********************************************************************************/

#include "random.h"
#include <cmath>

#ifdef NNFW_USE_GSL
#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#endif


namespace nnfw {

#ifdef NNFW_USE_GSL
gsl_rng* rnd = gsl_rng_alloc( gsl_rng_taus2 );
#endif

void Random::setSeed( long int seed ) {
#ifdef NNFW_USE_GSL
    gsl_rng_set( rnd, seed );
#else
    srand( seed );
#endif
    return;
}

Real Random::flatReal( ) {
#ifdef NNFW_USE_GSL
    return (Real)( gsl_rng_uniform( rnd ) );
#else
    return (Real) rand() / RAND_MAX;
#endif
}

Real Random::flatReal( Real min, Real max ) {
#ifdef NNFW_USE_GSL
    return (Real)( gsl_ran_flat( rnd, (double)min, (double)max ) );
#else
    return ( ( ( (Real) rand() ) / RAND_MAX ) * (max-min) ) + min;
#endif
}

bool Random::boolean( ) {
#ifdef NNFW_USE_GSL
    return ( gsl_rng_get( rnd )%2 );
#else
    return ( rand()%2 );
#endif
}

bool Random::boolean( Real trueProb ) {
#ifdef NNFW_USE_GSL
    return ( trueProb > gsl_rng_uniform( rnd ) );
#else
    return ( trueProb >  ( (Real)( rand() )/RAND_MAX ) );
#endif
}

u_int Random::flatInt( u_int x ) {
#ifdef NNFW_USE_GSL
    return (u_int)( gsl_rng_uniform_int( rnd, x ) );
#else
	return (int)(rand() % x);
#endif
}

int Random::flatInt( int min, int max ) {
#ifdef NNFW_USE_GSL
    return ( gsl_rng_get(rnd) % (max-min) ) + min;
#else
    return (int)(rand() % (max-min) + min-1);
#endif
}

Real Random::gauss( Real mean, Real stdev ) {
#ifdef NNFW_USE_GSL
    return mean + gsl_ran_gaussian( rnd, (double)stdev );
#else
    Real x1, x2, w;
    do {
        x1 = 2. * flatReal() - 1.;
        x2 = 2. * flatReal() - 1.;
        w = x1 * x1 + x2 * x2;
    } while ( w >= 1. );
    w = sqrt( (-2. * log( w ) ) / w );
    return (x1 * w) * sqrt(stdev) + mean;
#endif
}

};

