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
#include "messages.h"

#include <cmath>

#ifdef NNFW_USE_MKL
#include <mkl_vml.h>
#include <mkl_cblas.h>
#endif

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

RealMat::RealMat( u_int rows, u_int cols )
    : Matrix<Real>( rows, cols ) {
    zeroing();
}

RealMat::~RealMat() {
    // --- Nothing to do
}

    // ***********************************
    // *** VECTOR-MATRIX OPERATORS *******
    // ***********************************

RealVec& RealMat::mul( RealVec& y, const RealVec& x, const RealMat& m ) {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    cblas_sgemv(CblasColMajor, CblasTrans, m.rows(), m.cols(), 1.0, m.rawdata(), m.rows(), x.rawdata(), 1, 1.0f, y.rawdata(), 1);
#else
    cblas_dgemv(CblasColMajor, CblasTrans, m.rows(), m.cols(), 1.0, m.rawdata(), m.rows(), x.rawdata(), 1, 1.0, y.rawdata(), 1);
#endif
#else
    for ( u_int i = 0; i<m.cols(); i++ ) {
        for ( u_int j = 0; j<m.rows(); j++ ) {
            y[i] += x[j] * m.at( j, i );
        }
    }
#endif
    return y;
}

RealVec& RealMat::mul( RealVec& y, const RealMat& m, const RealVec& x ) {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    cblas_sgemv(CblasColMajor, CblasNoTrans, m.rows(), m.cols(), 1.0, m.rawdata(), m.rows(), x.rawdata(), 1, 1.0f, y.rawdata(), 1);
#else
    cblas_dgemv(CblasColMajor, CblasNoTrans, m.rows(), m.cols(), 1.0, m.rawdata(), m.rows(), x.rawdata(), 1, 1.0, y.rawdata(), 1);
#endif
#else
    for ( u_int i = 0; i<m.cols(); i++ ) {
        for ( u_int j = 0; j<m.rows(); j++ ) {
            y[j] += m.at( j, i ) * x[i];
        }
    }
#endif
    return y;
}


    // ****************************
    // *** MATH FUNCTION **********
    // ****************************

RealMat& RealMat::exp() {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    vsExp( size(), rawdata(), rawdata() );
#else
    vdExp( size(), rawdata(), rawdata() );
#endif
#else
    for( u_int i=0; i<size(); i++ ) {
        rawdata()[i] = std::exp( rawdata()[i] );
    };
#endif
    return (*this);
}

RealMat& RealMat::inv() {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    vsInv( size(), rawdata(), rawdata() );
#else
    vdInv( size(), rawdata(), rawdata() );
#endif
#else
    for( u_int i=0; i<size(); i++ ) {
        rawdata()[i] = 1.0/rawdata()[i];
    };
#endif
    return (*this);
}

RealMat::RealMat( const RealMat& orig )
    : Matrix<Real>( orig.rows(), orig.cols() ) {
    // --- Never Called
    this->assign( orig );
}

RealMat& RealMat::operator=( const RealMat& src ) {
    // --- Never Called
    this->assign( src );
    return (*this);
}

}

