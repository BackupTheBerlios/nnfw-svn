/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>                *
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
#include "randomgenerator.h"

namespace nnfw {

MatrixLinker::MatrixLinker( Cluster* from, Cluster* to, QString name )
	: Linker(from, to, name), nrows(from->numNeurons()), ncols(to->numNeurons()), w(nrows, ncols, true) {
}

MatrixLinker::~MatrixLinker() {
}

unsigned int MatrixLinker::size() const {
	return nrows*ncols;
}

void MatrixLinker::randomize( double min, double max ) {
	for ( unsigned int i = 0; i<nrows; i++ ) {
		for ( unsigned int j = 0; j<ncols; j++ ) {
			w[i][j] = globalRNG->getDouble( min, max );
		}
	}
}

void MatrixLinker::setWeight( unsigned int from, unsigned int to, double weight ) {
	#ifdef NNFW_DEBUG
	if ( from >= nrows ) {
		qWarning() << "Accessing out of Rows Boundary in MatrixLinker" ;
		return;
	}
	if ( to >= ncols ) {
		qWarning() << "Accessing out of Columns Boundary in MatrixLinker" ;
		return;
	}
	#endif
	w[from][to] = weight;
}

double MatrixLinker::weight( unsigned int from, unsigned int to ) {
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

void MatrixLinker::connect( unsigned int from, unsigned int to ) {
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
	w.unsteady( from, to );
}

void MatrixLinker::connectRandom( double prob, bool zeroDiagonal, bool symmetric ) {
	if ( symmetric ) {
		//--- SYMMETRIC TRUE
#ifdef NNFW_DEBUG
		if ( nrows != ncols ) {
			qWarning() << "connectRandom with symmetric to TRUE is allowed only with square matrix" ;
			return;
		}
#endif
		for( unsigned int r=0; r<rows(); r++ ) {
			for( unsigned int c=r; c<cols(); c++ ) {
				if ( r==c && zeroDiagonal ) {
					w[r][c] = 0.0;
					w.steady( r, c );
					continue;
				}
				if ( globalRNG->getBool( prob ) ) {
					w.unsteady( r, c );
					w.unsteady( c, r );
				} else {
					w[r][c] = 0.0;
					w.steady( r, c );
					w[c][r] = 0.0;
					w.steady( c, r );
				}
			}
		}
	} else {
		//--- SYMMETRIC FALSE
		for( unsigned int r=0; r<rows(); r++ ) {
			for( unsigned int c=0; c<cols(); c++ ) {
				if ( r==c && zeroDiagonal ) {
					w[r][c] = 0.0;
					w.steady( r, c );
					continue;
				}
				if ( globalRNG->getBool( prob ) ) {
					w.unsteady( r, c );
				} else {
					w[r][c] = 0.0;
					w.steady( r, c );
				}
			}
		}
	}
}

void MatrixLinker::connectAll() {
	for( unsigned int r=0; r<rows(); r++ ) {
		for( unsigned int c=0; c<cols(); c++ ) {
			w.unsteady( r, c );
		}
	}
}

void MatrixLinker::disconnect( unsigned int from, unsigned int to ) {
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
	w[from][to] = 0.0;
	w.steady( from, to );
}

void MatrixLinker::disconnectAll() {
	for( unsigned int r=0; r<rows(); r++ ) {
		for( unsigned int c=0; c<cols(); c++ ) {
			w[r][c] = 0.0;
			w.steady( r, c );
		}
	}
}

}
