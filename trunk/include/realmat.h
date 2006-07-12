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

#ifndef REALMAT_H
#define REALMAT_H

/*! \file
 *  \brief This file contains the RealMat Class; Don't include this file directly, instead include types.h
 *  Details...
 */

#ifdef NNFW_DEBUG
#include "messages.h"
#endif

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief RealMat Class
 *  \par Motivation
 *  \par Description
 *  \par Warnings
 */
class RealMat : public Matrix<Real> {
public:
    /*! \brief Construct an empty matrix of dimension size
     */
    RealMat( u_int rows, u_int cols );

    /*! \brief Destructor
     */
    ~RealMat();

    /*! \brief Assign elements
     */
    RealMat& assign( const RealMat& src ) {
#ifdef NNFW_DEBUG
        if( rows() != src.rows() || cols() != src.cols() ) {
            nnfwMessage( NNFW_ERROR, "Different dimension" );
            return (*this);
        }
#endif
        memcpy( rawdata(), src.rawdata(), sizeof(Real)*size() );
        return (*this);
    }

    /*! \brief Set all values to zero
     */
    void zeroing() {
        memset( rawdata(), 0, sizeof(Real)*size() );
    };

    // ****************************
    // *** UNARY OPERATORS ********
    // ****************************
    //! Operator -
    RealMat& operator-() {
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] = -rawdata()[i];
        }
        return (*this);
    };

    // ****************************
    // *** BINARY OPERATORS *******
    // ****************************
    //! Operator += with RealMat
    RealMat& operator+=(const RealMat& r ) {
#ifdef NNFW_DEBUG
        if( rows() != r.rows() || cols() != r.cols() ) {
            nnfwMessage( NNFW_ERROR, "Different dimension" );
            return (*this);
        }
#endif
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] += r.rawdata()[i];
        }
        return (*this);
    };
    //! Operator += with Real
    RealMat& operator+=(const Real& r ) {
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] += r;
        }
        return (*this);
    };
    //! Operator -= with RealMat
    RealMat& operator-=(const RealMat& r ) {
#ifdef NNFW_DEBUG
        if( rows() != r.rows() || cols() != r.cols() ) {
            nnfwMessage( NNFW_ERROR, "Different dimension" );
            return (*this);
        }
#endif
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] -= r.rawdata()[i];
        }
        return (*this);
    };
    //! Operator -= with Real
    RealMat& operator-=(const Real& r ) {
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] -= r;
        }
        return (*this);
    };
    //! Operator *= with RealMat
    RealMat& operator*=(const RealMat& r ) {
#ifdef NNFW_DEBUG
        if( rows() != r.rows() || cols() != r.cols() ) {
            nnfwMessage( NNFW_ERROR, "Different dimension" );
            return (*this);
        }
#endif
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] *= r.rawdata()[i];
        }
        return (*this);
    };
    //! Operator *= with Real
    RealMat& operator*=(const Real& r ) {
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] *= r;
        }
        return (*this);
    };
    //! Operator /= with RealMat
    RealMat& operator/=(const RealMat& r ) {
#ifdef NNFW_DEBUG
        if( rows() != r.rows() || cols() != r.cols() ) {
            nnfwMessage( NNFW_ERROR, "Different dimension" );
            return (*this);
        }
#endif
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] /= r.rawdata()[i];
        }
        return (*this);
    };
    //! Operator /= with Real
    RealMat& operator/=(const Real& r ) {
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] /= r;
        }
        return (*this);
    };

    // ***********************************
    // *** VECTOR-MATRIX OPERATORS *******
    // ***********************************
    /*! \brief Right Multiplication: y += x*m
     *  \param y the result of multiplication
     *  \param x the vector
     *  \param m the matrix
     *  \return the vector y
     */
    static RealVec& mul( RealVec& y, const RealVec& x, const RealMat& m );

    /*! \brief Left Multiplication: y += m*x
     *  \param y the result of multiplication
     *  \param m the matrix
     *  \param x the vector
     *  \return the vector y
     */
    static RealVec& mul( RealVec& y, const RealMat& m, const RealVec& x );

    // ****************************
    // *** MATH FUNCTION **********
    // ****************************
    /*! \brief Exponential
     */
    RealMat& exp();

    /*! \brief Scaling (scalar-vector multiply)
     */
    RealMat& scale( const Real v ) {
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] *= v;
        }
        return (*this);
    };

    /*! \brief Element Inversion
     */
    RealMat& inv();

private:
    //! RealVec view of rawdata
    RealVec* memView;
    //! Vector of RealVec views of rawdata
    RealVec** rowView;

    /*! \brief Copy-Constructor
     */
    RealMat( const RealMat& orig );

    /*! \brief Assignment Operator
     */
    RealMat& operator=( const RealMat& src );
};

}

#endif

