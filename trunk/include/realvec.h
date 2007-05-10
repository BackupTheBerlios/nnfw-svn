/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                     *
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
#error "You can't include realmat.h directly; Instead, You have to include types.h"
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
class NNFW_API RealVec : public VectorData<Real> {
public:
    /*! \name Constructors */
    //@{

    /*! Construct an empty vector of dimension size
     */
    RealVec( u_int size );
    
    /*! Construct a vector of dimension size with all values set to value
     */
    RealVec( u_int size, Real value );

    /*! Construct an empty vector with dimesion zero
     */
    RealVec();

    /*! Construct a RealVec view
     */
    RealVec( RealVec& src, u_int idStart, u_int idEnd );

    /*! Construct by copying data from const Real* vector
     */
    RealVec( const Real* r, u_int dim );

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
    RealVec& operator<<( const Real& value ) {
        append( value );
        return (*this);
    };

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

    /*! Exponential
     */
    RealVec& exp();

    /*! Scaling (scalar-vector multiply)
     */
    void scale( const Real v ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] *= v;
        }
    };

    /*! Element Inversion
     */
    RealVec& inv();

    /*! vector norm: ||x||
     */
    Real norm();

    /*! Calculate the square of each element
     */
	void square() {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] *= data[i];
        }
    };
	
    /*! Return the sum of the vector's elements
     */
	Real sum() {
		Real s = 0.0;
        for( u_int i=0; i<vsize; i++ ) {
            s += data[i];
        }
		return s;
    };

	/*! Not operation<br>
	 *  It apply the operator '!' to all elements of RealVec
     */
    void neg() {
        for( u_int i=0; i<vsize; i++ ) {
			data[i] = !data[i];
        }
    };

    /*! Equation: x-y
     */
    void assign_xminusy( const RealVec& x, const RealVec& y ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = x[i]-y[i];
        }
    };

    /*! Equation: -x
     */
    void assign_minusx( const RealVec& x ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = -x[i];
        }
    };

    /*! Equation: a-x
     */
    void assign_aminusx( Real a, const RealVec& x ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = a-x[i];
        }
    };

    /*! Equation: a*x
     */
    void assign_amulx( const Real a, const RealVec& x ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = a*x[i];
        }
    };

    /*! Equation: a/x
     */
    void assign_adivx( const Real a, const RealVec& x ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = a/x[i];
        }
    };
		
    /*! Return the mean value of the vector's elements
     */
	Real mean() {
		return this->sum() / vsize;
    };

	/*! For each element i, data[i] = 0 if data[i] <= threshold; data[i] = 1 otherwise
	 */
	void step( Real threshold ) {
		for ( u_int i = 0; i<vsize; i++ ) {
		    ( data[i] > threshold ) ? data[i] = 1.0f : data[i] = 0.0f;
		}
	}

    //@}
    /*! \name Vector-Vector Operators */
    //@{

    /*! Outer Product: m += x'*y
     *  \param m the matrix result of multiplication
     *  \param x the first vector
     *  \param y the second vector
     *  \return the matrix m
     */ 
    static RealMat& mul( RealMat& m, const RealVec& x, const RealVec& y );

    //@}
	
    /*! Create all the binary vectors of a given dimension
     */
	static void createAllBinaries( RealVec* vector, unsigned long int pats, u_int dims );

    /*! Return the mean square error of the vector's elements
     */
	static Real mse( const RealVec& target, const RealVec& actual );


protected:

    friend class RealMat;

    /*! return the rawdata
     */
    Real* rawdata() const {
        return VectorData<Real>::rawdata();
    };

};

}


#endif

