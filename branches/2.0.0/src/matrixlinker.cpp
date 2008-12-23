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

#include "matrixlinker.h"
#include "random.h"

#ifdef NNFW_USE_MKL
#include <mkl_cblas.h>
#endif

namespace nnfw {

MatrixLinker::MatrixLinker( Cluster* from, Cluster* to, QString name )
    : Linker(from, to, name), nrows(from->numNeurons()), ncols(to->numNeurons()), w(nrows, ncols) {
}

MatrixLinker::~MatrixLinker() {
}

unsigned int MatrixLinker::size() const {
    return nrows*ncols;
}

void MatrixLinker::randomize( double min, double max ) {
    for ( unsigned int i = 0; i<nrows; i++ ) {
        for ( unsigned int j = 0; j<ncols; j++ ) {
            w[i][j] = Random::flatdouble( min, max );
        }
    }
}

void MatrixLinker::setWeight( unsigned int from, unsigned int to, double weight ) {
#ifdef NNFW_DEBUG
    if ( from >= nrows ) {
        qWarning() << "Accessing out of Rows Boundary" ;
        return;
    }
    if ( to >= ncols ) {
		qWarning() << "Accessing out of Columns Boundary" ;
        return;
    }
#endif
    w[from][to] = weight;
}

double MatrixLinker::getWeight( unsigned int from, unsigned int to ) {
#ifdef NNFW_DEBUG
    if ( from >= nrows ) {
        qWarning() << "Accessing out of Rows Boundary" ;
        return 0.0;
    }
    if ( to >= ncols ) {
        qWarning() << "Accessing out of Columns Boundary" ;
        return 0.0;
    }
#endif
    return w[from][to];
}

void MatrixLinker::setMatrix( const RealMat& mat ) {
    w.assign( mat );
}

bool MatrixLinker::setMatrix( const Variant& v ) {
    w.assign( *( v.getRealMat() ) );
    return true;
}

}
