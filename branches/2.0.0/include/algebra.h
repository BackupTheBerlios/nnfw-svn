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

#ifndef ALGEBRA_H
#define ALGEBRA_H

/*! \file
 *  \brief This file contains algebra stuff
 *
 *  Details...
 *
 *
 */

#include "vectors.h"
#include "matrices.h"

namespace nnfw {

	/*! \name Vector Operation */
	//@{
	/*! Calculate the square of each element */
	DoubleVector& square( DoubleVector& data ) {
		for( unsigned int i=0; i<data.size(); i++ ) {
			data[i] *= data[i];
		}
		return data;
	};
	/*! Return the sum of the vector's elements */
	double sum( const DoubleVector& data ) {
		double s = 0.0;
		for( unsigned int i=0; i<data.size(); i++ ) {
			s += data[i];
		}
		return s;
	};
	/*! Return the mean value of the vector's elements */
	double mean( const DoubleVector& src ) {
		return sum( src ) / ( src.size() );
	};
	/*! Return the mean square error of the vector's elements */
	double mse( const DoubleVector& target, const DoubleVector& actual ) {
#ifdef NNFW_DEBUG
		if ( target.size() != actual.size() ) {
			qCritical() << "Error in mse: target and actual vectors have different size" ;
			return 0;
		}
#endif
		return mean( square( target - actual ) );
	};
	/*! It compute t = x-y
	 *  \param t the result of subtraction (it must already allocated by user)
	 *  \param x the vector
	 *  \param y the vector
	 *  \return the vector t
	 */
	DoubleVector& minus( DoubleVector& t, const DoubleVector& x, const DoubleVector& y ) {
#ifdef NNFW_DEBUG
		if ( t.size() != x.size() || x.size() != y.size() ) {
			qCritical() << "Error in minus: t, x and y vectors have different size" ;
			return 0;
		}
#endif
		for( unsigned int i=0; i<t.size(); i++ ) {
			t[i] = x[i]-y[i];
		}
	};

	//@}
	/*! \name Vector-Matrix Operation */
	//@{

	/*! Right Multiplication: y += x*m
	 *  \param y the result of multiplication
	 *  \param x the vector
	 *  \param m the matrix
	 *  \return the vector y
	 */
	DoubleVector& mul( DoubleVector& y, const DoubleVector& x, const DoubleMatrix& m ) {
#ifdef NNFW_DEBUG
		if ( y.size() != m.cols() || x.size() != m.rows() ) {
			qCritical() << "Incompatible DoubleVector and DoubleMatrix in mul operation";
			return y;
		}
#endif
		for ( unsigned int j = 0; j<m.rows(); j++ ) {
			for ( unsigned int i = 0; i<m.cols(); i++ ) {
				y[i] += x[j] * m[j][i];
			}
		}
		return y;
	};

	/*! Left Multiplication: y += m*x
	 *  \param y the result of multiplication
	 *  \param m the matrix
	 *  \param x the vector
	 *  \return the vector y
	 */
	DoubleVector& mul( DoubleVector& y, const DoubleMatrix& m, const DoubleVector& x ) {
#ifdef NNFW_DEBUG
		if ( y.size() != m.rows() || x.size() != m.cols() ) {
			qCritical() << "Incompatible DoubleVector and DoubleMatrix in mul operation";
			return y;
		}
#endif
		for ( unsigned int j = 0; j<m.rows(); j++ ) {
			for ( unsigned int i = 0; i<m.cols(); i++ ) {
				y[j] += m[j][i] * x[i];
			}
		}
		return y;
	};

	//@}

}

#endif
