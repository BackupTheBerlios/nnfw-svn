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

#define PI_GRECO 3.14159265358979323846
#include "vectors.h"
#include "matrices.h"
#include <cmath>

namespace nnfw {
	/*! \name Vector Operation */
	//@{
	/*! Return the index where the first occurrence of the maximum value appear in the vector */
	inline unsigned int maxIndex( const DoubleVector& src ) {
		unsigned int mi = 0;
		for( unsigned int i=0; i<src.size(); i++ ) {
			if ( src[mi] < src[i] ) {
				mi = i;
			}
		}
		return mi;
	};
	/*! Fill up the dest vector with all available elements from source until
	 *  the end of dest or source is reached
	 */
	inline DoubleVector& fillup( DoubleVector& dest, const DoubleVector& source ) {
		unsigned int size = qMin( dest.size(), source.size() );
		for( unsigned int i=0; i<size; i++ ) {
			dest[i] = source[i];
		}
		return dest;
	};
	/*! Calculate the square of each element */
	inline DoubleVector& square( DoubleVector& data ) {
		for( unsigned int i=0; i<data.size(); i++ ) {
			data[i] *= data[i];
		}
		return data;
	};
	/*! Calculate the exponential of each element */
	inline DoubleVector& exp( DoubleVector& data ) {
		for( unsigned int i=0; i<data.size(); i++ ) {
			data[i] = std::exp(data[i]);
		}
		return data;
	};
	/*! Calculate the inverse of each element */
	inline DoubleVector& inv( DoubleVector& data ) {
		for( unsigned int i=0; i<data.size(); i++ ) {
			data[i] = 1.0 / data[i];
		}
		return data;
	};
	/*! Return the sum of the vector's elements */
	inline double sum( const DoubleVector& data ) {
		double s = 0.0;
		for( unsigned int i=0; i<data.size(); i++ ) {
			s += data[i];
		}
		return s;
	};
	/*! Return the mean value of the vector's elements */
	inline double mean( const DoubleVector& src ) {
		return sum( src ) / ( src.size() );
	};
	/*! Return the mean square error of the vector's elements */
	inline double mse( const DoubleVector& target, const DoubleVector& actual ) {
#ifdef NNFW_DEBUG
		if ( target.size() != actual.size() ) {
			qCritical() << "Error in mse: target and actual vectors have different size" ;
			return 0;
		}
#endif
		DoubleVector min = target - actual;
		return mean( square( min ) );
	};
	/*! It compute t = x-y
	 *  \param t the result of subtraction (it must already allocated by user)
	 *  \param x the vector
	 *  \param y the vector
	 *  \return the vector t
	 */
	inline DoubleVector& minus( DoubleVector& t, const DoubleVector& x, const DoubleVector& y ) {
#ifdef NNFW_DEBUG
		if ( t.size() != x.size() || x.size() != y.size() ) {
			qCritical() << "Error in minus: t, x and y vectors have different size" ;
			return t;
		}
#endif
		for( unsigned int i=0; i<t.size(); i++ ) {
			t[i] = x[i]-y[i];
		}
		return t;
	};
	/*! It compute y = a-x
	 *  \param y the result of subtraction (it must already allocated by user)
	 *  \param a the scalar
	 *  \param x the vector
	 *  \return the vector y
	 */
	inline DoubleVector& minus( DoubleVector& y, double a, const DoubleVector& x ) {
#ifdef NNFW_DEBUG
		if ( y.size() != x.size() ) {
			qCritical() << "Error in minus: x and y vectors have different size" ;
			return y;
		}
#endif
		for( unsigned int i=0; i<y.size(); i++ ) {
			y[i] = a - x[i];
		}
		return y;
	};
	/*! It compute y = x + a
	 *  \param y the result of addition (it must already allocated by user)
	 *  \param a the scalar
	 *  \param x the vector
	 *  \return the vector y
	 *  \note if you can pass '-a' to obtain a subtraction y = x -a 
	 */
	inline DoubleVector& add( DoubleVector& y, const DoubleVector& x, double a ) {
#ifdef NNFW_DEBUG
		if ( y.size() != x.size() ) {
			qCritical() << "Error in minus: x and y vectors have different size" ;
			return y;
		}
#endif
		for( unsigned int i=0; i<y.size(); i++ ) {
			y[i] = x[i] + a;
		}
		return y;
	};
	/*! Multiplication with a scalar: y = a*x
	 *  \param y the result of multiplication
	 *  \param a the scalar value
	 *  \param x the vector
	 *  \return the vector y
	 */
	inline DoubleVector& mul( DoubleVector& y, double a, const DoubleVector& x ) {
#ifdef NNFW_DEBUG
		if ( y.size() != x.size() ) {
			qCritical() << "Incompatibles DoubleVectors in mul operation";
			return y;
		}
#endif
		for( unsigned int i=0; i<y.size(); i++ ) {
			y[i] = a*x[i];
		}
		return y;
	};
	/*! Accumulated Multiplication with a scalar: y += a*x
	 *  \param y the result of multiplication
	 *  \param a the scalar value
	 *  \param x the vector
	 *  \return the vector y
	 */
	inline DoubleVector& amul( DoubleVector& y, double a, const DoubleVector& x ) {
#ifdef NNFW_DEBUG
		if ( y.size() != x.size() ) {
			qCritical() << "Incompatibles DoubleVectors in mul operation";
			return y;
		}
#endif
		for( unsigned int i=0; i<y.size(); i++ ) {
			y[i] += a*x[i];
		}
		return y;
	};
	/*! Delta-Rule: r += rate * x * y<br>
	 *  Add to r the computation of the so-called delta-rule
	 *  \param r is the vector on which delta-rule results is added
	 *  \param rate is the scaling factor of multiplication
	 *  \param x is the first vector
	 *  \param y is the second vector
	 *  \return the vector r
	 */
	inline DoubleVector& deltarule( DoubleVector& r, double rate, const DoubleVector& x, const DoubleVector& y ) {
#ifdef NNFW_DEBUG
		if ( r.size() != y.size() || y.size() != x.size() ) {
			qCritical() << "Incompatibles DoubleVectors in deltarule operation";
			return r;
		}
#endif
		for( unsigned int i=0; i<r.size(); i++ ) {
			r[i] += rate*x[i]*y[i];
		}
		return r;
	};

	//@}
	/*! \name Vector-Matrix Operation */
	//@{

	/*! Right Multiplication: y = x*m
	 *  \param y the result of multiplication
	 *  \param x the vector
	 *  \param m the matrix
	 *  \return the vector y
	 */
	inline DoubleVector& mul( DoubleVector& y, const DoubleVector& x, const DoubleMatrix& m ) {
#ifdef NNFW_DEBUG
		if ( y.size() != m.cols() || x.size() != m.rows() ) {
			qCritical() << "Incompatible DoubleVector and DoubleMatrix in mul operation";
			return y;
		}
#endif
		for ( unsigned int j = 0; j<m.rows(); j++ ) {
			for ( unsigned int i = 0; i<m.cols(); i++ ) {
				y[i] = x[j] * m[j][i];
			}
		}
		return y;
	};
	/*! Right Accumulated Multiplication: y += x*m
	 *  \param y the result of multiplication
	 *  \param x the vector
	 *  \param m the matrix
	 *  \return the vector y
	 */
	inline DoubleVector& amul( DoubleVector& y, const DoubleVector& x, const DoubleMatrix& m ) {
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

	/*! Left Multiplication: y = m*x
	 *  \param y the result of multiplication
	 *  \param m the matrix
	 *  \param x the vector
	 *  \return the vector y
	 */
	inline DoubleVector& mul( DoubleVector& y, const DoubleMatrix& m, const DoubleVector& x ) {
#ifdef NNFW_DEBUG
		if ( y.size() != m.rows() || x.size() != m.cols() ) {
			qCritical() << "Incompatible DoubleVector and DoubleMatrix in mul operation";
			return y;
		}
#endif
		for ( unsigned int j = 0; j<m.rows(); j++ ) {
			for ( unsigned int i = 0; i<m.cols(); i++ ) {
				y[j] = m[j][i] * x[i];
			}
		}
		return y;
	};
	/*! Left Accumulated Multiplication: y += m*x
	 *  \param y the result of multiplication
	 *  \param m the matrix
	 *  \param x the vector
	 *  \return the vector y
	 */
	inline DoubleVector& amul( DoubleVector& y, const DoubleMatrix& m, const DoubleVector& x ) {
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
	/*! Delta-Rule: m += rate * x * y<br>
	 *  \param m the matrix on which the result of Delta-rule is added up
	 *  \param rate is the factor of multiplicaton
	 *  \param x is the first vector, its dimension must be the same of matrix's rows
	 *  \param y is the second vector, its dimension must be the same of matrix's columns
	 *  \return the matrix m
	 */
	inline DoubleMatrix& deltarule( DoubleMatrix& m, double rate, const DoubleVector& x, const DoubleVector& y ) {
#ifdef NNFW_DEBUG
		if ( x.size() != m.rows() || y.size() != m.cols() ) {
			qCritical() << "Incompatible DoubleVector and DoubleMatrix in deltarule operation";
			return m;
		}
#endif
		for ( unsigned int r=0; r<m.rows(); r++ ) {
			for ( unsigned int c=0; c<m.cols(); c++ ) {
				m[r][c] += rate * x[r] * y[c];
			}
		}
		return m;
	}
	//@}

}

#endif
