/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                *
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
NNFW_INTERNAL gsl_rng* rnd = gsl_rng_alloc( gsl_rng_taus2 );
#endif

void Random::setSeed( long int seed ) {
#ifdef NNFW_USE_GSL
    gsl_rng_set( rnd, seed );
#else
    srand( seed );
#endif
    return;
}

double Random::flatdouble( ) {
#ifdef NNFW_USE_GSL
    return (double)( gsl_rng_uniform( rnd ) );
#else
    return (double) rand() / RAND_MAX;
#endif
}

double Random::flatdouble( double min, double max ) {
#ifdef NNFW_USE_GSL
    return (double)( gsl_ran_flat( rnd, (double)min, (double)max ) );
#else
    return ( ( ( (double) rand() ) / RAND_MAX ) * (max-min) ) + min;
#endif
}

RealVec& Random::flatRealVec( RealVec& vec, double min, double max ) {
	for( unsigned int i=0; i<vec.size(); i++ ) {
		vec[i] = flatdouble( min, max );
	}
	return vec;
}

RealMat& Random::flatRealMat( RealMat& mat, double min, double max ) {
	for( unsigned int row_nr=0; row_nr<mat.rows(); row_nr++ ) {
		for( unsigned int col_nr=0; col_nr<mat.cols(); col_nr++ ) {
			mat[row_nr][col_nr] = flatdouble( min, max );
		}
	}
	return mat;
}

bool Random::boolean( ) {
#ifdef NNFW_USE_GSL
    return ( gsl_rng_get( rnd )%2 );
#else
    return ( rand()%2 );
#endif
}

bool Random::boolean( double trueProb ) {
#ifdef NNFW_USE_GSL
    return ( trueProb > gsl_rng_uniform( rnd ) );
#else
    return ( trueProb >  ( (double)( rand() )/RAND_MAX ) );
#endif
}

unsigned int Random::flatInt( unsigned int x ) {
#ifdef NNFW_USE_GSL
    return (unsigned int)( gsl_rng_uniform_int( rnd, x ) );
#else
	return (int)(rand() % x);
#endif
}

int Random::flatInt( int min, int max ) {
#ifdef NNFW_DEBUG
	if ( min == max ) {
		nFatal() << "You cannot specify identical min and max values in Random::flatInt" ;
		return min;
	}
#endif
#ifdef NNFW_USE_GSL
    return ( gsl_rng_get(rnd) % (max-min) ) + min;
#else
    return (int)(rand() % (max-min) + min);
#endif
}

double Random::gauss( double mean, double stdev ) {
#ifdef NNFW_USE_GSL
    return mean + gsl_ran_gaussian( rnd, (double)stdev );
#else
    double x1, x2, w;
    do {
        x1 = 2. * flatdouble() - 1.;
        x2 = 2. * flatdouble() - 1.;
        w = x1 * x1 + x2 * x2;
    } while ( w >= 1. );
    w = sqrt( (-2. * log( w ) ) / w );
    return (x1 * w) * sqrt(stdev) + mean;
#endif
}

};

