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

#include <cstdio>
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
class RealVec {
public:
    /*! \brief Construct an empty vector of dimension size
     */
    RealVec( u_int size );

    /*! \brief Construct an empty vector with dimesion zero
     */
    RealVec();

    /*! \brief Construct by copying data from const Real* vector
     */
    RealVec( const Real* r, u_int dim );

    /*! \brief Destructor
     */
    ~RealVec();

    /*! \brief Raw Data
     * ======== QUESTO METODO NON DOVREBBE ESISTERE ?!?!? =========
     */
    Real* rawdata() {
        return data;
    };

    /*! \brief Return the size of RealVec
     */
    u_int size() const {
        return vsize;
    };

    /*! \brief Set all values to zero
     */
    void zeroing() {
        memset( data, 0, sizeof(Real)*vsize );
    };

    /*! \brief Assign to first num element the value passed
     */
    void assign( u_int num, Real value ) {
#ifdef NNFW_DEBUG
        if ( num > vsize ) {
            nnfwMessage( NNFW_ERROR, "Wrong number of elements passed to assign method" );
            num = vsize;
        }
#endif
        for( u_int i=0; i<num; i++ ) {
            data[i] = value;
        }
    };

    /*! \brief Assignment method. The sizes of RealVec must be the same.
     */
    void assign( const RealVec& src ) {
#ifdef NNFW_DEBUG
        if ( vsize != src.vsize ) {
            nnfwMessage( NNFW_ERROR, "Wrong number of elements between to assign method" );
            return;
        }
#endif
        memcpy( data, src.data, sizeof(Real)*vsize );
    };

    /*! \brief Indexing operator
     *  Boundary check activated only when DEBUG if defined
     */
    Real& operator[]( u_int index ) {
#ifdef NNFW_DEBUG
        if( index >= vsize ) {
            nnfwMessage( NNFW_ERROR, "Accessing elements outside boundary" );
            return data[0];
        }
#endif
        return data[index];
    };

    /*! \brief Indexing operator (Const Version)
     *  Boundary check activated only when DEBUG if defined
     */
    const Real& operator[]( u_int index ) const {
#ifdef NNFW_DEBUG
        if( index >= vsize ) {
            nnfwMessage( NNFW_ERROR, "Accessing elements outside boundary" );
            return data[0];
        }
#endif
        return data[index];
    };

    /*! \brief Resize the RealVec
     */
    void resize( u_int newsize ) {
        if ( allocated < newsize ) {
            allocated = newsize+20;
            Real* tmp = new Real[allocated];
            memcpy( tmp, data, sizeof(Real)*(vsize) );
            delete []data;
            data = tmp;
        }
        for( u_int i=vsize; i<newsize; i++ ) {
            data[i] = 0.0f;
        }
        vsize = newsize;
    };

    /*! \brief Append an element; the dimesion increase by one
     */
    void append( const Real value ) {
        if ( allocated < vsize+1 ) {
            allocated += 21;
            Real* tmp = new Real[allocated];
            memcpy( tmp, data, sizeof(Real)*(vsize) );
            delete []data;
            data = tmp;
        }
        data[vsize] = value;
        vsize += 1;
    };

    /*! \brief Append Operator
     */
    RealVec& operator<<( const Real value ) {
        append( value );
        return (*this);
    };

    // ****************************
    // *** UNARY OPERATORS ********
    // ****************************
    //! Operator -
    RealVec& operator-() {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = -data[i];
        }
        return (*this);
    };

    // ****************************
    // *** BINARY OPERATORS *******
    // ****************************
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

    // ****************************
    // *** MATH FUNCTION **********
    // ****************************
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
    void inv();

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

private:
    //! The actual size of RealVec
    u_int vsize;
    //! The allocated space
    u_int allocated;
    //! Data
    Real* data;

    /*! \brief Copy-Constructor
     */
    RealVec( const RealVec& orig );

    /*! \brief Assignment Operator
     */
    RealVec& operator=( const RealVec& src );
};

}

#endif

