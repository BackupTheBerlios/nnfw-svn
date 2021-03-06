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

SparseMatrixLinker::SparseMatrixLinker( Cluster* from, Cluster* to, const char* name )
    : MatrixLinker( from, to, name ), maskm(rows(), cols()) {
    // --- Init data
    for( u_int i=0; i<rows(); i++ ) {
        for( u_int j=0; j<cols(); j++ ) {
            maskm[i][j] = true;
        }
    }
    addProperty( "mask", Variant::t_realmat, this, &SparseMatrixLinker::maskP, &SparseMatrixLinker::setMask );
    setTypename( "SparseMatrixLinker" );
}

SparseMatrixLinker::SparseMatrixLinker( Real prob, Cluster* from, Cluster* to, const char* name )
    : MatrixLinker( from, to, name ), maskm(rows(), cols()) {
    // --- Init data
    for( u_int i=0; i<rows(); i++ ) {
        for( u_int j=0; j<cols(); j++ ) {
            maskm[i][j] = Random::boolean( prob );
        }
    }
    addProperty( "mask", Variant::t_realmat, this, &SparseMatrixLinker::maskP, &SparseMatrixLinker::setMask );
    setTypename( "SparseMatrixLinker" );
}

SparseMatrixLinker::SparseMatrixLinker( Cluster* from, Cluster* to, Real prob, bool zeroDiagonal,
                                        bool symmetricMask, const char* name )
    : MatrixLinker( from, to, name ), maskm(rows(), cols()) {
#ifdef NNFW_DEBUG
    if( rows() != cols() ) {
        nError() << "SparseMatrixLinker constructor which assumes square matrix used with a non square matrix!";
        return;
    }
#endif
    u_int zeroD = 0;
    if ( zeroDiagonal ) {
        zeroD = 1;
    }
    if ( symmetricMask ) {
        for( u_int i=0; i<rows(); i++ ) {
            for( u_int j=i+zeroD; j<cols(); j++ ) {
                maskm[i][j] = Random::boolean( prob );
                maskm[j][i] = maskm[i][j];
            }
        }
    } else {
        for( u_int i=0; i<rows(); i++ ) {
            for( u_int j=i+zeroD; j<cols(); j++ ) {
                maskm[i][j] = Random::boolean( prob );
                maskm[j][i] = Random::boolean( prob );
            }
        }
    }
    addProperty( "mask", Variant::t_realmat, this, &SparseMatrixLinker::maskP, &SparseMatrixLinker::setMask );
    setTypename( "SparseMatrixLinker" );
}

SparseMatrixLinker::SparseMatrixLinker( PropertySettings& prop )
    : MatrixLinker( prop ), maskm(rows(), cols()) {
    Variant& v = prop["mask"];
    if ( ! v.isNull() ) {
        setMask( v );
    }
    addProperty( "mask", Variant::t_realmat, this, &SparseMatrixLinker::maskP, &SparseMatrixLinker::setMask );
    setTypename( "SparseMatrixLinker" );
}


SparseMatrixLinker::~SparseMatrixLinker() {
}

void SparseMatrixLinker::setWeight( u_int from, u_int to, Real weight ) {
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

void SparseMatrixLinker::randomize( Real min, Real max ) {
    for ( u_int i = 0; i<rows(); i++ ) {
        for ( u_int j = 0; j<cols(); j++ ) {
            if ( maskm[i][j] ) {
                matrix()[i][j] = Random::flatReal( min, max );
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

void SparseMatrixLinker::connect( u_int from, u_int to ) {
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

void SparseMatrixLinker::connectRandom( Real prob ) {
    for ( u_int r = 0; r < rows(); r++ ) {
        for ( u_int c = 0; c < cols(); c++ ) {
            maskm[r][c] = ! ( Random::flatReal() < prob );
        }
    }
}

void SparseMatrixLinker::connectAll() {
    for ( u_int r = 0; r < rows(); r++ ) {
        for ( u_int c = 0; c < cols(); c++ ) {
            maskm[r][c] = true;
        }
    }
}

void SparseMatrixLinker::disconnect( u_int from, u_int to ) {
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

void SparseMatrixLinker::disconnectRandom( Real prob ) {
    for ( u_int r = 0; r < rows(); r++ ) {
        for ( u_int c = 0; c < cols(); c++ ) {
            maskm[r][c] = ! ( Random::flatReal() < prob );
        }
    }
}

void SparseMatrixLinker::setMask( const MatrixData<bool>& m ) {
	maskm.assign( m );
	matrix().cover( maskm );
}

bool SparseMatrixLinker::setMask( const Variant& v ) {
	maskm.assign( *( v.getDataPtr< MatrixData<bool> >() ) );
	matrix().cover( maskm );
	return true;
}

SparseMatrixLinker* SparseMatrixLinker::clone() const {
	SparseMatrixLinker* newclone = new SparseMatrixLinker( this->from(), this->to(), name() );
	newclone->setMatrix( this->matrix() );
	newclone->maskm.assign( this->maskm );
	return newclone;
}


}

