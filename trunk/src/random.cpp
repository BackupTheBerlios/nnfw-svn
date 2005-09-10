/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005 Gianluca Massera <emmegian@yahoo.it>                     *
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
#include "gsl/gsl_randist.h"


//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

gsl_rng* Random::rnd = gsl_rng_alloc( gsl_rng_taus2 );

void Random::setSeed( long int seed ) {
    gsl_rng_set( rnd, seed );
    return;
}

nnfwReal Random::flatReal( nnfwReal min, nnfwReal max ) {
    return (nnfwReal)( gsl_ran_flat( rnd, (double)min, (double)max ) );
}

bool Random::boolean( ) {
    return ( gsl_rng_get( rnd )%2 );
}

bool Random::boolean( nnfwReal trueProb ) {
    return ( trueProb > gsl_rng_uniform( rnd ) );
}

}
