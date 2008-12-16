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

// --- You can't include it directly
#ifndef TYPES_INCLUDES
#error "You can't include realvec.h directly; Instead, You have to include types.h"
// --- follow define avoid to get a lot of understandable error !
#define REALVEC_H
#endif

#ifndef REALVEC_H
#define REALVEC_H

/*! \file
 *  \brief This file contains the RealVec Class; Don't include this file directly, instead include types.h
 */

namespace nnfw {

/*! \brief RealVec Class
 *  \par Motivation
 *  \par Description
 *  \par Warnings
 */
class NNFW_API RealVec : public VectorData<double> {
public:
    /*! \name Constructors */
    //@{

    /*! Construct an empty vector of dimension size
     */
    RealVec( unsigned int size );
    
    /*! Construct a vector of dimension size with all values set to value
     */
    RealVec( unsigned int size, double value );

    /*! Construct an empty vector with dimesion zero
     */
    RealVec();

    /*! Construct a RealVec view
     */
    RealVec( RealVec& src, unsigned int idStart, unsigned int idEnd );

    /*! Construct by copying data from const double* vector
     */
    RealVec( const double* r, unsigned int dim );

    /*! Copy-Constructor
     */
    RealVec( const RealVec& orig );

    /*! Assignement-operator
     */
    RealVec& operator=( const RealVec& src ) {
		RealVec& self = *this;
		self.resize( src.size() );
		self.assign( src );
		return self;
	};

    //@}
    /*! \name Operations on RealVec */
    //@{

    /*! Append Operator
     */
    RealVec& operator<<( const double& value ) {
        append( value );
        return (*this);
    };

    //@}
    /*! \name Unary Operators */
    //@{

    /*! Operator - */
    RealVec& operator-() {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] = -data[i];
        }
        return (*this);
    };

    //@}
    /*! \name Binary Operators */
    //@{

    /*! Operator += with RealVec */
    RealVec& operator+=(const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            qCritical() << "Different numbers of element" ;
            return (*this);
        }
#endif
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] += r.data[i];
        }
        return (*this);
    };
    /*! Operator += with double */
    RealVec& operator+=(const double& r ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] += r;
        }
        return (*this);
    };
    /*! Operator -= with RealVec */
    RealVec& operator-=(const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            qCritical() << "Different numbers of element" ;
            return *this;
        }
#endif
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] -= r.data[i];
        }
        return (*this);
    };
    /*! Operator -= with double */
    RealVec& operator-=(const double& r ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] -= r;
        }
        return (*this);
    };
    /*! Operator *= with RealVec */
    RealVec& operator*=(const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            qCritical() << "Different numbers of element" ;
            return (*this);
        }
#endif
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] *= r.data[i];
        }
        return (*this);
    };
    /*! Operator *= with double */
    RealVec& operator*=(const double& r ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] *= r;
        }
        return (*this);
    };
    /*! Operator /= with RealVec */
    RealVec& operator/=(const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            qCritical() << "Different numbers of element" ;
            return (*this);
        }
#endif
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] /= r.data[i];
        }
        return (*this);
    };
    /*! Operator /= with double */
    RealVec& operator/=(const double& r ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] /= r;
        }
        return (*this);
    };

    /*! Dot product */
    double dot( const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            qCritical() << "Different numbers of element" ;
            return 0.0;
        }
#endif
        double ret = 0.0;
        for( unsigned int i=0; i<vsize; i++ ) {
            ret += data[i] * r.data[i];
        }
        return ret;
    };

    //@}
    /*! \name Math Functions */
    //@{

    /*! Exponential
     */
    RealVec& exp();

    /*! Scaling (scalar-vector multiply)
     */
    RealVec& scale( const double v ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] *= v;
        }
		return (*this);
    };

    /*! Element Inversion
     */
    RealVec& inv();

    /*! vector norm: ||x||
     */
    double norm();

    /*! Calculate the square of each element
     */
	RealVec& square() {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] *= data[i];
        }
		return (*this);
    };
	
    /*! Return the sum of the vector's elements
     */
	double sum() {
		double s = 0.0;
        for( unsigned int i=0; i<vsize; i++ ) {
            s += data[i];
        }
		return s;
    };

	/*! Normalize the vector, so the norm is equal to one
	 */
	RealVec& normalize() {
		double n = norm();
		if ( n==0.0 ) return (*this);
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] /= n;
        }
		return (*this);
	};

	/*! Not operation<br>
	 *  It apply the operator '!' to all elements of RealVec
     */
    RealVec& neg() {
        for( unsigned int i=0; i<vsize; i++ ) {
			data[i] = !data[i];
        }
		return (*this);
    };

    /*! Equation: x-y
     */
    void assign_xminusy( const RealVec& x, const RealVec& y ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] = x[i]-y[i];
        }
    };

    /*! Equation: -x
     */
    void assign_minusx( const RealVec& x ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] = -x[i];
        }
    };

    /*! Equation: a-x
     */
    void assign_aminusx( double a, const RealVec& x ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] = a-x[i];
        }
    };

    /*! Equation: a*x
     */
    void assign_amulx( const double a, const RealVec& x ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] = a*x[i];
        }
    };

    /*! Equation: a/x
     */
    void assign_adivx( const double a, const RealVec& x ) {
        for( unsigned int i=0; i<vsize; i++ ) {
            data[i] = a/x[i];
        }
    };
		
    /*! Return the mean value of the vector's elements
     */
	double mean() {
		return this->sum() / vsize;
    };

	/*! return the position of the maximum element in the vector */
	int maxIndex() {
		int mi = 0;
		for( unsigned int i=1; i<vsize; i++ ) {
			if ( data[i] > data[mi] ) mi=i;
		}
		return mi;
	}

	/*! return the value of the maximum element in the vector */
	double maxValue() {
		return data[maxIndex()];
	}

	/*! return the position of the minimum element in the vector */
	int minIndex() {
		int mi = 0;
		for( unsigned int i=1; i<vsize; i++ ) {
			if ( data[i] < data[mi] ) mi=i;
		}
		return mi;
	}

	/*! return the value of the minimum element in the vector */
	double minValue() {
		return data[minIndex()];
	}

	/*! For each element i, data[i] = 0 if data[i] <= threshold; data[i] = 1 otherwise
	 */
	RealVec& step( double threshold ) {
		for ( unsigned int i = 0; i<vsize; i++ ) {
		    ( data[i] > threshold ) ? data[i] = 1.0f : data[i] = 0.0f;
		}
		return (*this);
	}

    /*! Create all the binary vectors of a given dimension
     */
	static void createAllBinaries( RealVec* vector, unsigned long int pats, unsigned int dims );

    /*! Return the mean square error of the vector's elements
     */
	static double mse( const RealVec& target, const RealVec& actual );

	/*! Delta-Rule: r += rate * x * y<br>
	 *  Add to itselt the scaled by rate of the element-by-element multiplication of x with y
	 *  \param rate is the scaling factor of multiplication
	 *  \param x is the first vector
	 *  \param y is the second vector
	 */
	RealVec& deltarule( double rate, const RealVec& x, const RealVec& y ) {
		for( unsigned int i=0; i<vsize; i++ ) {
			data[i] += rate*x[i]*y[i];
		}
		return (*this);
	};

    //@}
    /*! \name Vector-Vector Operators */
    //@{

    /*! Outer Product: m += x'*y<br>
	 *  The x vector will be taken as a matrix of Rx1 dimension and y as a matrix of 1xC dimension,
	 *  where RxC is the dimension of m<br>
     *  \param m the matrix result of multiplication
     *  \param x the first vector; the dimesion must be the same of matrix's rows
     *  \param y the second vector; the dimension must be the same of matrix's columns
     *  \return the matrix m 
     */ 
    static RealMat& outprod( RealMat& m, const RealVec& x, const RealVec& y );

    //@}
	

protected:

    friend class RealMat;
    /*! return the rawdata */
    double* rawdata() const {
        return VectorData<double>::rawdata();
    };

	//--- for accessing from C interface implementation
	friend double* getRawData( RealVec& );

};

}

#endif

