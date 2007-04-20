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

#include "matrixlinker.h"
#include "random.h"

#ifdef NNFW_USE_MKL
#include <mkl_cblas.h>
#endif


namespace nnfw {

MatrixLinker::MatrixLinker( Cluster* from, Cluster* to, const char* name )
    : Linker(from, to, name), nrows(from->numNeurons()), ncols(to->numNeurons()), w(nrows, ncols) {
    addProperty( "weights", Variant::t_realmat, this, &MatrixLinker::matrixP, &MatrixLinker::setMatrix );
    setTypename( "MatrixLinker" );
}

MatrixLinker::MatrixLinker( PropertySettings& prop )
    : Linker( prop ), nrows( from()->numNeurons() ), ncols( to()->numNeurons() ), w(nrows, ncols) {
    Variant& v = prop["weights"];
    if ( ! v.isNull() ) {
        setMatrix( v );
    }
    addProperty( "weights", Variant::t_realmat, this, &MatrixLinker::matrixP, &MatrixLinker::setMatrix );
    setTypename( "MatrixLinker" );
}

MatrixLinker::~MatrixLinker() {
}

u_int MatrixLinker::getRows() {
    return nrows;
}

u_int MatrixLinker::getCols() {
    return ncols;
}

u_int MatrixLinker::size() const {
    return nrows*ncols;
}

void MatrixLinker::randomize( Real min, Real max ) {
    for ( u_int i = 0; i<nrows; i++ ) {
        for ( u_int j = 0; j<ncols; j++ ) {
            w[i][j] = Random::flatReal( min, max );
        }
    }
}

void MatrixLinker::setWeight( u_int from, u_int to, Real weight ) {
    if ( from >= nrows ) {
        nnfwMessage( NNFW_ERROR, "Accessing out of Rows Boundary" );
        return;
    }
    if ( to >= ncols ) {
        nnfwMessage( NNFW_ERROR, "Accessing out of Columns Boundary" );
        return;
    }
    w[from][to] = weight;
}

Real MatrixLinker::getWeight( u_int from, u_int to ) {
    if ( from >= nrows ) {
        nnfwMessage( NNFW_ERROR, "Accessing out of Rows Boundary" );
        return 0.0;
    }
    if ( to >= ncols ) {
        nnfwMessage( NNFW_ERROR, "Accessing out of Columns Boundary" );
        return 0.0;
    }
    return w[from][to];
}

void MatrixLinker::setMatrix( const RealMat& mat ) {
    w.assign( mat );
}

bool MatrixLinker::setMatrix( const Variant& v ) {
    w.assign( *( v.getRealMat() ) );
    return true;
}

void MatrixLinker::update() {
    // check if cluster 'To' needs a reset
    if ( to()->needReset() ) {
        to()->resetInputs();
    }
    RealMat::mul( to()->inputs(), from()->outputs(), w );
    return;
}

}
