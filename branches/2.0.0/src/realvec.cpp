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

#include "types.h"

#include <cmath>

#ifdef NNFW_USE_MKL
#include <mkl_vml.h>
#include <mkl_cblas.h>
#endif

namespace nnfw {

#ifdef NNFW_USE_MKL
NNFW_INTERNAL int inutile = vmlSetMode( VML_LA );
#endif

RealVec::RealVec( unsigned int size )
    : VectorData<double>( size ) {
}

RealVec::RealVec( unsigned int size, double value )
    : VectorData<double>(size, value) {
}


RealVec::RealVec()
    : VectorData<double>() {
}

RealVec::RealVec( const double* r, unsigned int dim )
    : VectorData<double>(r, dim) {
}

RealVec::RealVec( RealVec& src, unsigned int idStart, unsigned int idEnd )
    : VectorData<double>(src, idStart, idEnd) {
}

RealVec::RealVec( const RealVec& src )
    : VectorData<double>(src) {
}

RealVec& RealVec::exp() {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    vsExp( vsize, data, data );
#else
    vdExp( vsize, data, data );
#endif
#else
    for( unsigned int i=0; i<vsize; i++ ) {
        data[i] = std::exp( data[i] );
    };
#endif
    return (*this);
}

RealVec& RealVec::inv() {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    vsInv( vsize, data, data );
#else
    vdInv( vsize, data, data );
#endif
#else
    for( unsigned int i=0; i<vsize; i++ ) {
        data[i] = 1.0/data[i];
    };
#endif
    return (*this);
}

double RealVec::norm() {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    return cblas_snrm2( vsize, data, 1 );
#else
    return cblas_dnrm2( vsize, data, 1 );
#endif
#else
    double res = 0.0;
    for( unsigned int i=0; i<vsize; i++ ) {
        res += data[i]*data[i];
    }
    res = std::sqrt( res );
    return res;
#endif
}

RealMat& RealVec::outprod( RealMat& m, const RealVec& x, const RealVec& y ) {
#ifdef NNFW_USE_MKL
    double* mRaw = m.rawdata().rawdata();
    double* xRaw = x.rawdata();
    double* yRaw = y.rawdata();
#ifndef NNFW_DOUBLE_PRECISION
    cblas_sgemm( CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m.rows(), m.cols(), 1, 1.0f, xRaw, 1, yRaw, m.cols(), 1.0f, mRaw, m.cols() );
#else
    cblas_dgemm( CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m.rows(), m.cols(), 1, 1.0f, xRaw, 1, yRaw, m.cols(), 1.0f, mRaw, m.cols() );
#endif
#else
	for ( unsigned int r=0; r<m.rows(); r++ ) {
		for ( unsigned int c=0; c<m.cols(); c++ ) {
			m[r][c] += x[r] * y[c];
		}
	}
#endif
    return m;
}

void RealVec::createAllBinaries( RealVec* v, unsigned long int pats, unsigned int dims ) {
	unsigned long int totPat = 1;
	for (unsigned int x = 0; x < dims; x++)
		totPat *= 2;
#ifdef NNFW_DEBUG
	if ( totPat != pats ) {
		nError() << "Error in createAllBinaries: total patterns are not how many you think they are" ;
		return;
	}
#endif	
	//Create the first string of all 0s
	unsigned int i;
	for (i = 0; i < dims; i++)
		v[0][i] = 0.;
	for (unsigned long int number = 1; number < pats; number++) {
		//String n = string n - 1
		for (i = 0; i < dims; i++)
			v[ number ][i] = v[ number - 1 ][i];
		//Add one to string n - 1
		i = 0;
		while ( v[ number ][i] == 1. ) {
			v[ number ][i] = 0.;
			i++;
		}
		v[ number ][i] = 1.;
	}	
};


double RealVec::mse( const RealVec& target, const RealVec& actual ) {
#ifdef NNFW_DEBUG
	if ( target.size() != actual.size() ) {
		nError() << "Error in mse: target and actual vectors have different size" ;
		return 0.0;
	}
#endif
	RealVec error( target.size() );
	error.assign(target);
	error -= actual;
	error.square();
	return error.mean();
};


}

