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

#include "types.h"

#include <cmath>

#ifdef NNFW_USE_MKL
#include <mkl_vml.h>
#include <mkl_cblas.h>
#endif

//! Namespace that contains all classes of Neural Network Framework
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

