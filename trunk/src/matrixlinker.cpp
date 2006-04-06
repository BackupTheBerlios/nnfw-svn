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
#include <cstdio>

#ifdef NNFW_USE_MKL
#include <mkl_cblas.h>
#endif

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

MatrixLinker::MatrixLinker( Cluster* from, Cluster* to, const char* name )
    : Linker(name), nrows(from->size()), ncols(to->size()), w(nrows, ncols) {
    this->from = from;
    this->to = to;
}

MatrixLinker::~MatrixLinker() {
}

u_int MatrixLinker::getRows() {
    return nrows;
}

u_int MatrixLinker::getCols() {
    return ncols;
}

u_int MatrixLinker::size() {
    return nrows*ncols;
}

void MatrixLinker::randomize( Real min, Real max ) {
    for ( u_int i = 0; i<nrows; i++ ) {
        for ( u_int j = 0; j<ncols; j++ ) {
            w.at( i, j ) = Random::flatReal( min, max );
        }
    }
}

void MatrixLinker::setWeight( u_int from, u_int to, Real weight ) {
    if ( from >= nrows ) {
        // Messaggio di errore !!!
        return;
    }
    if ( to >= ncols ) {
        // Messaggio di errore !!!
        return;
    }
    w.at( from, to ) = weight;
};

Real MatrixLinker::getWeight( u_int from, u_int to ) {
    if ( from >= nrows ) {
        // Messaggio di errore !!!
        return 0.0;
    }
    if ( to >= ncols ) {
        // Messaggio di errore !!!
        return 0.0;
    }
    return w.at( from, to );
};

Cluster* MatrixLinker::getFrom() const {
    return from;
};

Cluster* MatrixLinker::getTo() const {
    return to;
};

void MatrixLinker::update() {
    // incoming cluster output
    Real* outputsFrom = from->outputs().rawdata();
    // outgoing cluster inputs
    Real* inputsTo = to->inputs().rawdata();
#ifdef NNFW_USE_MKL
    cblas_dgemv(CblasColMajor, CblasTrans, nrows, ncols, 1.0, w.rawdata(), nrows, outputsFrom, 1, 1.0, inputsTo, 1);
#else
    for ( u_int i = 0; i<ncols; i++ ) {
        for ( u_int j = 0; j<nrows; j++ ) {
            inputsTo[i] += outputsFrom[j] * w.at( j, i );
        }
    }
#endif
    return;
}

}
