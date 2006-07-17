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
    : data(rows*cols), rowView( rows ) {
    data.zeroing();
    nrows = rows;
    ncols = cols;
    tsize = nrows*ncols;
    // --- Constructing the view of rows
    //rowView = new ( RealVec ( *[nrows] ) );
    //rowView = new RealVec[nrows];
    for( u_int i=0; i<nrows; i++ ) {
        //rowViews[i] = new RealVec( data, i*ncols, (i+1)*ncols );
        rowView[i].convertToView( data, i*ncols, (i+1)*ncols );
    }
}

RealMat::~RealMat() {
    // --- Nothing to do
}

void RealMat::resize( u_int rows, u_int cols ) {
    nrows = rows;
    ncols = cols;
    tsize = nrows*ncols;
    data.resize( tsize );
    rowView.resize( nrows );
    // --- Adjust the view of rows
    for( u_int i=0; i<nrows; i++ ) {
        if ( rowView[i].isView() ) {
            rowView[i].setView( i*ncols, (i+1)*ncols );
        } else {
            rowView[i].convertToView( data, i*ncols, (i+1)*ncols );
        }
    }
}


    // ***********************************
    // *** VECTOR-MATRIX OPERATORS *******
    // ***********************************

RealVec& RealMat::mul( RealVec& y, const RealVec& x, const RealMat& m ) {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    cblas_sgemv(CblasRowMajor, CblasTrans, m.rows(), m.cols(), 1.0, m.rawdata(), m.cols(), x.rawdata(), 1, 1.0f, y.rawdata(), 1);
#else
    cblas_dgemv(CblasRowMajor, CblasTrans, m.rows(), m.cols(), 1.0, m.rawdata(), m.cols(), x.rawdata(), 1, 1.0, y.rawdata(), 1);
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
#ifndef NNFW_DOUBLE_PRECISION
    cblas_sgemv(CblasRowMajor, CblasNoTrans, m.rows(), m.cols(), 1.0, m.rawdata(), m.cols(), x.rawdata(), 1, 1.0f, y.rawdata(), 1);
#else
    cblas_dgemv(CblasRowMajor, CblasNoTrans, m.rows(), m.cols(), 1.0, m.rawdata(), m.cols(), x.rawdata(), 1, 1.0, y.rawdata(), 1);
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

RealMat::RealMat( const RealMat& ) {
    // --- Never Called
    //this->assign( orig );
}

RealMat& RealMat::operator=( const RealMat& ) {
    // --- Never Called
    //this->assign( src );
    return (*this);
}

}

