/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
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

#include "libmatrixmodifiers.h"

#include <iostream>
namespace nnfw {

void BasicMatrixModifier::modify() {
	//To check (rows and cols) and, most importantly, to be implemented matricially!!!
	u_int rows = m.rows();
	u_int cols = m.cols();
	for ( u_int r = 0; r < rows; r++ ) {
		for ( u_int c = 0; c < cols; c++ ) {
			m[r][c] += lr * inp[r] * out[c];
		}
	}
}

void SparseMatrixModifier::modify() {
	//To check (rows and cols) and, most importantly, to be implemented matricially!!!
	u_int rows = m.rows();
	u_int cols = m.cols();
	for ( u_int r = 0; r < rows; r++ ) {
		for ( u_int c = 0; c < cols; c++ ) {
			if ( mask[r][c] ) {
				m[r][c] += lr * inp[r] * out[c];
			}
		}
	}
}

void SparseCovarianceMatrixModifier::modify() {
	//To check (rows and cols) and, most importantly, to be implemented matricially!!!
	u_int rows = m.rows();
	u_int cols = m.cols();
	for ( u_int r = 0; r < rows; r++ ) {
		for ( u_int c = 0; c < cols; c++ ) {
			if ( mask[r][c] ) {
				m[r][c] += lr * ( inp[r] - mInp[r] ) * ( out[c] - mOut[c] );
			}
		}
	}
}


}
