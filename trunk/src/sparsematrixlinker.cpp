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
    : MatrixLinker( from, to, name ), mask(nrows, ncols) {
    // --- Init data
    for( u_int i=0; i<nrows; i++ ) {
        for( u_int j=0; j<ncols; j++ ) {
            mask[i][j] = true;
        }
    }
}

SparseMatrixLinker::SparseMatrixLinker( Real prob, Cluster* from, Cluster* to, const char* name )
    : MatrixLinker( from, to, name ), mask(nrows, ncols) {
    // --- Init data
    for( u_int i=0; i<nrows; i++ ) {
        for( u_int j=0; j<ncols; j++ ) {
            mask[i][j] = Random::boolean( prob );
        }
    }
}

SparseMatrixLinker::SparseMatrixLinker( Cluster* from, Cluster* to, Real prob, bool zeroDiagonal, bool symmetricMask, const char* name )
    : MatrixLinker( from, to, name ), mask(nrows, ncols) {
#ifdef NNFW_DEBUG
        if( nrows != ncols ) {
            nnfwMessage( NNFW_ERROR, "SparseMatrixLinker constructor which assumes square matrix used with a non square matrix!" );
            return;
        }
#endif
	if ( zeroDiagonal ) {
		if ( symmetricMask ) {
			for( u_int i=0; i<nrows; i++ ) {
				for( u_int j=i+1; j<ncols; j++ ) {
				    mask[i][j] = Random::boolean( prob );
				    mask[j][i] = mask[i][j];
				}
			}
		}
		else {
			for( u_int i=0; i<nrows; i++ ) {
				for( u_int j=i+1; j<ncols; j++ ) {
				    mask[i][j] = Random::boolean( prob );
				    mask[j][i] = Random::boolean( prob );
				}
			}
		}
	}
	else {
		if ( symmetricMask ) {
			for( u_int i=0; i<nrows; i++ ) {
				for( u_int j=i; j<ncols; j++ ) {
				    mask[i][j] = Random::boolean( prob );
				    mask[j][i] = mask[i][j];
				}
			}
		} 
		else {	
			for( u_int i=0; i<nrows; i++ ) {
				for( u_int j=i; j<ncols; j++ ) {
				    mask[i][j] = Random::boolean( prob );
				    mask[j][i] = Random::boolean( prob );
				}
			}
		}
	}
}

SparseMatrixLinker::SparseMatrixLinker( PropertySettings& prop )
    : MatrixLinker( prop ), mask(nrows, ncols) {
}


SparseMatrixLinker::~SparseMatrixLinker() {
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
    if ( mask[from][to] ) {
        w[from][to] = weight;
    } else {
        w[from][to] = 0.0;
    }
}

void SparseMatrixLinker::randomize( Real min, Real max ) {
    for ( u_int i = 0; i<nrows; i++ ) {
        for ( u_int j = 0; j<ncols; j++ ) {
            if ( mask[i][j] ) {
                w[i][j] = Random::flatReal( min, max );
            } else {
                w[i][j] = 0.0;
            }
        }
    }
}

void SparseMatrixLinker::connect( u_int from, u_int to ) {
    if ( from >= nrows ) {
        // Messaggio di errore !!!
        return;
    }
    if ( to >= ncols ) {
        // Messaggio di errore !!!
        return;
    }
    mask[from][to] = true;
}

void SparseMatrixLinker::connectAll() {
    for ( u_int r = 0; r < nrows; r++ ) 
		for ( u_int c = 0; c < ncols; c++ )
		    mask[r][c] = true;	
}

void SparseMatrixLinker::disconnect( u_int from, u_int to ) {
    if ( from >= nrows ) {
        // Messaggio di errore !!!
        return;
    }
    if ( to >= ncols ) {
        // Messaggio di errore !!!
        return;
    }
    mask[from][to] = false;
    w[from][to] = 0.0;
}

void SparseMatrixLinker::disconnect( Real prob ) {
    for ( u_int r = 0; r < nrows; r++ ) 
		for ( u_int c = 0; c < ncols; c++ )
			if ( Random::flatReal() < prob )
				mask[r][c] = false;
}


}

