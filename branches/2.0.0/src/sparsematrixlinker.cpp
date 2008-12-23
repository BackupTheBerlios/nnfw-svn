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

#include "sparsematrixlinker.h"
#include "random.h"

namespace nnfw {

SparseMatrixLinker::SparseMatrixLinker( Cluster* from, Cluster* to, QString name )
    : MatrixLinker( from, to, name ), maskm(rows(), cols()) {
    // --- Init data
    for( unsigned int i=0; i<rows(); i++ ) {
        for( unsigned int j=0; j<cols(); j++ ) {
            maskm[i][j] = true;
        }
    }
}

SparseMatrixLinker::SparseMatrixLinker( double prob, Cluster* from, Cluster* to, QString name )
    : MatrixLinker( from, to, name ), maskm(rows(), cols()) {
    // --- Init data
    for( unsigned int i=0; i<rows(); i++ ) {
        for( unsigned int j=0; j<cols(); j++ ) {
            maskm[i][j] = Random::boolean( prob );
        }
    }
}

SparseMatrixLinker::SparseMatrixLinker( Cluster* from, Cluster* to, double prob, bool zeroDiagonal,
                                        bool symmetricMask, QString name )
    : MatrixLinker( from, to, name ), maskm(rows(), cols()) {
#ifdef NNFW_DEBUG
    if( rows() != cols() ) {
        qWarning() << "SparseMatrixLinker constructor which assumes square matrix used with a non square matrix!";
        return;
    }
#endif
    unsigned int zeroD = 0;
    if ( zeroDiagonal ) {
        zeroD = 1;
    }
    if ( symmetricMask ) {
        for( unsigned int i=0; i<rows(); i++ ) {
            for( unsigned int j=i+zeroD; j<cols(); j++ ) {
                maskm[i][j] = Random::boolean( prob );
                maskm[j][i] = maskm[i][j];
            }
        }
    } else {
        for( unsigned int i=0; i<rows(); i++ ) {
            for( unsigned int j=i+zeroD; j<cols(); j++ ) {
                maskm[i][j] = Random::boolean( prob );
                maskm[j][i] = Random::boolean( prob );
            }
        }
    }
}

SparseMatrixLinker::~SparseMatrixLinker() {
}

void SparseMatrixLinker::setWeight( unsigned int from, unsigned int to, double weight ) {
    if ( from >= rows() ) {
        // Messaggio di errore !!!
        return;
    }
    if ( to >= cols() ) {
        // Messaggio di errore !!!
        return;
    }
    if ( maskm[from][to] ) {
        matrix()[from][to] = weight;
    } else {
        matrix()[from][to] = 0.0;
    }
}

void SparseMatrixLinker::randomize( double min, double max ) {
    for ( unsigned int i = 0; i<rows(); i++ ) {
        for ( unsigned int j = 0; j<cols(); j++ ) {
            if ( maskm[i][j] ) {
                matrix()[i][j] = Random::flatdouble( min, max );
            } else {
                matrix()[i][j] = 0.0;
            }
        }
    }
}

void SparseMatrixLinker::update() {
    // check if cluster 'To' needs a reset
    if ( to()->needReset() ) {
        to()->resetInputs();
    }
    RealMat::mul( to()->inputs(), from()->outputs(), matrix() );
    return;
}

void SparseMatrixLinker::connect( unsigned int from, unsigned int to ) {
    if ( from >= rows() ) {
        // Messaggio di errore !!!
        return;
    }
    if ( to >= cols() ) {
        // Messaggio di errore !!!
        return;
    }
    maskm[from][to] = true;
}

void SparseMatrixLinker::connectRandom( double prob ) {
    for ( unsigned int r = 0; r < rows(); r++ ) {
        for ( unsigned int c = 0; c < cols(); c++ ) {
            maskm[r][c] = ! ( Random::flatdouble() < prob );
        }
    }
}

void SparseMatrixLinker::connectAll() {
    for ( unsigned int r = 0; r < rows(); r++ ) {
        for ( unsigned int c = 0; c < cols(); c++ ) {
            maskm[r][c] = true;
        }
    }
}

void SparseMatrixLinker::disconnect( unsigned int from, unsigned int to ) {
    if ( from >= rows() ) {
        // Messaggio di errore !!!
        return;
    }
    if ( to >= cols() ) {
        // Messaggio di errore !!!
        return;
    }
    maskm[from][to] = false;
    matrix()[from][to] = 0.0;
}

void SparseMatrixLinker::disconnectAll() {
    maskm.zeroing();
}

void SparseMatrixLinker::disconnectRandom( double prob ) {
    for ( unsigned int r = 0; r < rows(); r++ ) {
        for ( unsigned int c = 0; c < cols(); c++ ) {
            maskm[r][c] = ! ( Random::flatdouble() < prob );
        }
    }
}

void SparseMatrixLinker::setMask( const MatrixData<bool>& m ) {
	maskm.assign( m );
	matrix().cover( maskm );
}

SparseMatrixLinker* SparseMatrixLinker::clone() const {
	SparseMatrixLinker* newclone = new SparseMatrixLinker( this->from(), this->to(), name() );
	newclone->setMatrix( this->matrix() );
	newclone->maskm.assign( this->maskm );
	return newclone;
}


}

