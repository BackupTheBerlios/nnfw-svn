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

#include "types.h"

#include <cmath>

#ifdef NNFW_USE_MKL
#include <mkl_vml.h>
#include <mkl_cblas.h>
#endif


namespace nnfw {

RealMat::RealMat( u_int rows, u_int cols )
    : MatrixData<Real, RealVec>( rows, cols ) {
}

RealMat::RealMat( RealVec& src, u_int rstart, u_int rend, u_int rows, u_int cols )
    : MatrixData<Real, RealVec>(src, rstart, rend, rows, cols) {
}

RealMat::~RealMat() {
    // --- Nothing to do
}

    // ***********************************
    // *** VECTOR-MATRIX OPERATORS *******
    // ***********************************

RealVec& RealMat::mul( RealVec& y, const RealVec& x, const RealMat& m ) {
#ifdef NNFW_USE_MKL
    Real* mRaw = m.rawdata().rawdata();
    Real* xRaw = x.rawdata();
    Real* yRaw = y.rawdata();
#ifndef NNFW_DOUBLE_PRECISION
    cblas_sgemv(CblasRowMajor, CblasTrans, m.rows(), m.cols(), 1.0, mRaw, m.cols(), xRaw, 1, 1.0f, yRaw, 1);
#else
    cblas_dgemv(CblasRowMajor, CblasTrans, m.rows(), m.cols(), 1.0, mRaw, m.cols(), xRaw, 1, 1.0, yRaw, 1);
#endif
#else
    for ( u_int j = 0; j<m.rows(); j++ ) {
        for ( u_int i = 0; i<m.cols(); i++ ) {
            y[i] += x[j] * m[j][i];
        }
    }
#endif
    return y;
}

RealVec& RealMat::mul( RealVec& y, const RealMat& m, const RealVec& x ) {
#ifdef NNFW_USE_MKL
    Real* mRaw = m.rawdata().rawdata();
    Real* xRaw = x.rawdata();
    Real* yRaw = y.rawdata();
#ifndef NNFW_DOUBLE_PRECISION
    cblas_sgemv(CblasRowMajor, CblasNoTrans, m.rows(), m.cols(), 1.0, mRaw, m.cols(), xRaw, 1, 1.0f, yRaw, 1);
#else
    cblas_dgemv(CblasRowMajor, CblasNoTrans, m.rows(), m.cols(), 1.0, mRaw, m.cols(), xRaw, 1, 1.0, yRaw, 1);
#endif
#else
    for ( u_int j = 0; j<m.rows(); j++ ) {
        for ( u_int i = 0; i<m.cols(); i++ ) {
            y[j] += m[j][i] * x[i];
        }
    }
#endif
    return y;
}

RealMat& RealMat::deltarule( Real rate, const RealVec& x, const RealVec& y ) {
#ifdef NNFW_USE_MKL
    Real* mRaw = rawdata().rawdata();
    Real* xRaw = x.rawdata();
    Real* yRaw = y.rawdata();
#ifndef NNFW_DOUBLE_PRECISION
    cblas_sgemm( CblasRowMajor, CblasNoTrans, CblasNoTrans,
                rows(), cols(), 1, rate, xRaw, 1, yRaw, cols(), 1.0f, mRaw, cols() );
#else
    cblas_dgemm( CblasRowMajor, CblasNoTrans, CblasNoTrans,
                rows(), cols(), 1, rate, xRaw, 1, yRaw, cols(), 1.0f, mRaw, cols() );
#endif
	return (*this);
#else
	RealMat& self = *this;
	for ( u_int r=0; r<rows(); r++ ) {
		for ( u_int c=0; c<cols(); c++ ) {
			self[r][c] += rate * x[r] * y[c];
		}
	}
	return self;
#endif
}


    // ****************************
    // *** MATH FUNCTION **********
    // ****************************

RealMat& RealMat::exp() {
    rawdata().exp();
    return (*this);
}

RealMat& RealMat::inv() {
    rawdata().inv();
    return (*this);
}

}

