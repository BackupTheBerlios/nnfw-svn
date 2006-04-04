/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2006 Gianluca Massera <emmegian@yahoo.it>                     *
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

#include "types.h"

#include <cmath>

#ifdef NNFW_USE_MKL
#include <mkl_vml.h>
#include <mkl_cblas.h>
#endif

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

#ifdef NNFW_USE_MKL
int inutile = vmlSetMode( VML_LA );
#endif

RealVec::RealVec( u_int size ) {
    vsize = size;
    allocated = size;
    data = new Real[vsize];
}

RealVec::RealVec() {
    data = 0;
    vsize = 0;
    allocated = 0;
}

RealVec::RealVec( const Real* r, u_int dim ) {
    data = new Real[dim];
    vsize = dim;
    allocated = dim;
    memcpy( data, r, sizeof(Real)*dim );
}

RealVec::~RealVec() {
    // ---- QUESTO DELETE DA PROBLEMI ... PERCHE' ????
    //delete []data;
}

RealVec& RealVec::exp() {
#ifdef NNFW_USE_MKL
    vdExp( vsize, data, data );
#else
    for( u_int i=0; i<vsize; i++ ) {
        data[i] = std::exp( data[i] );
    };
#endif
    return (*this);
}

void RealVec::inv() {
#ifdef NNFW_USE_MKL
    vdInv( vsize, data, data );
#else
    for( u_int i=0; i<vsize; i++ ) {
        data[i] = 1.0/data[i];
    };
#endif
}

RealVec::RealVec( const RealVec& ) {
/*    vsize = orig.vsize;
    allocated = orig.allocated;
    data = new Real[allocated];
    memcpy( data, orig.data, sizeof(Real)*vsize );*/
}

RealVec& RealVec::operator=( const RealVec& ) {
    // delete []data;  <-- DA PROBLEMI... Perche??? Non capisco !! :-(
/*    vsize = src.vsize;
    allocated = src.allocated;
    data = new Real[allocated];
    memcpy( data, src.data, sizeof(Real)*vsize );
*/
    return (*this);
}

}

