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

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/**********************************************
 *  Implementation of MatrixLinker Class     *
 **********************************************/

MatrixLinker::MatrixLinker( Cluster* from, Cluster* to, const char* name )
    : Linker(name) {
    nrows = from->size();
    ncols = to->size();

    // Weight Matrix Allocation procedure
    //  Matrix[column][row]
    memM = new Real[nrows*ncols];
    w = new ( Real ( *[ncols] ) );
    for ( u_int i = 0; i<ncols; i++ ) {
        w[i] = memM + i*nrows;
    }

    this->from = from;
    this->to = to;
}

MatrixLinker::~MatrixLinker() {
    delete []memM;
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
            w[j][i] = Random::flatReal( min, max );
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
    w[to][from] = weight;
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
    return w[to][from];
};

Cluster* MatrixLinker::getFrom() const {
    return from;
};

Cluster* MatrixLinker::getTo() const {
    return to;
};

void MatrixLinker::update() {
    // incoming cluster output
    Real* outputsFrom = from->outputs();
    // outgoing cluster inputs
    Real* inputsTo = to->inputs();

//    Real* ptr;
    for ( u_int i = 0; i<ncols; i++ ) {
//        ptr = w[i];
        for ( u_int j = 0; j<nrows; j++ ) {
            inputsTo[i] += outputsFrom[j] * w[i][j];
//            inputsTo[i] += outputsFrom[j] * ptr[j];
        }
    }
    return;
}

}
