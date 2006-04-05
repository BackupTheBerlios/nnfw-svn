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

#include "sparsematrixlinker.h"
#include "random.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

SparseMatrixLinker::SparseMatrixLinker( Cluster* from, Cluster* to, const char* name )
    : MatrixLinker( from, to, name ) {
    // Mask Matrix Allocation procedure
    //  Matrix[column][row]
    memMask = new bool[nrows*ncols];
    mask = new ( bool ( *[ncols] ) );
    for ( u_int i = 0; i<ncols; i++ ) {
        mask[i] = memMask + i*nrows;
    }
    // Set Full Connection
    for( u_int i=0; i<ncols; i++ ) {
        for( u_int j=0; j<nrows; j++ ) {
            mask[i][j] = true;
        }
    }
}

SparseMatrixLinker::SparseMatrixLinker( Real prob, Cluster* from, Cluster* to, const char* name )
    : MatrixLinker( from, to, name ) {
    // Mask Matrix Allocation procedure
    //  Matrix[column][row]
    memMask = new bool[nrows*ncols];
    mask = new ( bool ( *[ncols] ) );
    for ( u_int i = 0; i<ncols; i++ ) {
        mask[i] = memMask + i*nrows;
    }
    // Set a Connection with probability specified
    for( u_int i=0; i<ncols; i++ ) {
        for( u_int j=0; j<nrows; j++ ) {
            mask[i][j] = Random::boolean( prob );
        }
    }
}

SparseMatrixLinker::~SparseMatrixLinker() {
    delete []memMask;
}

void SparseMatrixLinker::setWeight( u_int from, u_int to, Real weight ) {
    if ( from >= nrows ) {
        // Messaggio di errore !!!
        return;
    }
    if ( to >= ncols ) {
        // Messaggio di errore !!!
        return;
    }
    if ( mask[to][from] ) {
        w[to][from] = weight;
    } else {
        w[to][from] = 0.0;
    }
}

void SparseMatrixLinker::randomize( Real min, Real max ) {
    for ( u_int i = 0; i<nrows; i++ ) {
        for ( u_int j = 0; j<ncols; j++ ) {
            if ( mask[j][i] ) {
                w[j][i] = Random::flatReal( min, max );
            } else {
                w[j][i] = 0.0;
            }
        }
    }
}

void SparseMatrixLinker::connection( u_int from, u_int to ) {
    if ( from >= nrows ) {
        // Messaggio di errore !!!
        return;
    }
    if ( to >= ncols ) {
        // Messaggio di errore !!!
        return;
    }
    mask[to][from] = true;
}

void SparseMatrixLinker::disconnection( u_int from, u_int to ) {
    if ( from >= nrows ) {
        // Messaggio di errore !!!
        return;
    }
    if ( to >= ncols ) {
        // Messaggio di errore !!!
        return;
    }
    mask[to][from] = false;
    w[to][from] = 0.0;
}

}

