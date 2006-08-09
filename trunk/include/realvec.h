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

#ifndef REALVEC_H
#define REALVEC_H

/*! \file
 *  \brief This file contains the RealVec Class; Don't include this file directly, instead include types.h
 *  Details...
 */

#include <iostream>
#ifdef NNFW_DEBUG
#include "messages.h"
#endif


//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief RealVec Class
 *  \par Motivation
 *  \par Description
 *  \par Warnings
 */
class RealVec : public VectorData<Real> {
public:
    /*! \name Constructors */
    //@{

    /*! \brief Construct an empty vector of dimension size
     */
    RealVec( u_int size );
    
    /*! \brief Construct a vector of dimension size with all values set to value
     */
    RealVec( u_int size, Real value );

    /*! \brief Construct an empty vector with dimesion zero
     */
    RealVec();

    /*! \brief Construct a RealVec view
     */
    RealVec( RealVec& src, u_int idStart, u_int idEnd );

    /*! \brief Construct by copying data from const Real* vector
     */
    RealVec( const Real* r, u_int dim );

    /*! \brief Copy-Constructor
     */
    RealVec( const RealVec& orig );

    //@}
    /*! \name Unary Operators */
    //@{

    //! Operator -
    RealVec& operator-() {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = -data[i];
        }
        return (*this);
    };

    //@}
    /*! \name Binary Operators */
    //@{

    //! Operator += with RealVec
    RealVec& operator+=(const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return (*this);
        }
#endif
        for( u_int i=0; i<vsize; i++ ) {
            data[i] += r.data[i];
        }
        return (*this);
    };
    //! Operator += with Real
    RealVec& operator+=(const Real& r ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] += r;
        }
        return (*this);
    };
    //! Operator -= with RealVec
    RealVec& operator-=(const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return *this;
        }
#endif
        for( u_int i=0; i<vsize; i++ ) {
            data[i] -= r.data[i];
        }
        return (*this);
    };
    //! Operator -= with Real
    RealVec& operator-=(const Real& r ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] -= r;
        }
        return (*this);
    };
    //! Operator *= with RealVec
    RealVec& operator*=(const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return (*this);
        }
#endif
        for( u_int i=0; i<vsize; i++ ) {
            data[i] *= r.data[i];
        }
        return (*this);
    };
    //! Operator *= with Real
    RealVec& operator*=(const Real& r ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] *= r;
        }
        return (*this);
    };
    //! Operator /= with RealVec
    RealVec& operator/=(const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return (*this);
        }
#endif
        for( u_int i=0; i<vsize; i++ ) {
            data[i] /= r.data[i];
        }
        return (*this);
    };
    //! Operator /= with Real
    RealVec& operator/=(const Real& r ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] /= r;
        }
        return (*this);
    };

    //! Dot product
    Real dot( const RealVec& r ) {
#ifdef NNFW_DEBUG
        if( vsize != r.vsize ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return 0.0;
        }
#endif
        Real ret = 0.0;
        for( u_int i=0; i<vsize; i++ ) {
            ret += data[i] * r.data[i];
        }
        return ret;
    };

    //@}
    /*! \name Math Functions */
    //@{

    /*! \brief Exponential
     */
    RealVec& exp();

    /*! \brief Scaling (scalar-vector multiply)
     */
    void scale( const Real v ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] *= v;
        }
    };

    /*! \brief Element Inversion
     */
    RealVec& inv();

    /*! \brief Equation: x-y
     */
    void assign_xminusy( const RealVec& x, const RealVec& y ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = x[i]-y[i];
        }
    };

    /*! \brief Equation: -x
     */
    void assign_minusx( const RealVec& x ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = -x[i];
        }
    };

    /*! \brief Equation: a-x
     */
    void assign_aminusx( Real a, const RealVec& x ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = a-x[i];
        }
    };

    /*! \brief Equation: a*x
     */
    void assign_amulx( const Real a, const RealVec& x ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = a*x[i];
        }
    };

    /*! \brief Equation: a/x
     */
    void assign_adivx( const Real a, const RealVec& x ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = a/x[i];
        }
    };
	
    /*! \brief Calculate the square of each element
     */
	void square() {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] *= data[i];
        }
    };
	
    /*! \brief Return the sum of the vector's elements
     */
	Real sum() {
		Real s = 0.0;
        for( u_int i=0; i<vsize; i++ ) {
            s += data[i];
        }
		return s;
    };
	
    /*! \brief Return the mean value of the vector's elements
     */
	Real mean() {
		return this->sum() / vsize;
    };

	/*! \brief For each element i, data[i] = 0 if data[i] <= threshold; data[i] = 1 otherwise
	 */
	void step( Real threshold ) {
		for ( u_int i = 0; i<vsize; i++ ) {
		    ( data[i] > threshold ) ? data[i] = 1.0f : data[i] = 0.0f;
		}
	}

    //@}
	
    /*! \brief Create all the binary vectors of a given dimension
     */
	static void createAllBinaries( RealVec* vector, unsigned long int pats, u_int dims );

    /*! \brief Return the mean square error of the vector's elements
     */
	static Real mse( const RealVec& target, const RealVec& actual );

protected:

    friend class RealMat;

    /*! \brief return the rawdata
     */
    Real* rawdata() const {
        return VectorData<Real>::rawdata();
    };

};

//! Operator << with RealVec
std::ostream& operator<<(std::ostream& stream, const RealVec& v);

//! Operator >> with RealVec
std::istream& operator>>(std::istream& stream, RealVec& v);

}


#endif

