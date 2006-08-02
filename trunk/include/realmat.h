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
class RealMat {
public:
    /*! \brief Construct an empty matrix of dimension size
     */
    RealMat( u_int rows, u_int cols );

    /*! \brief Destructor
     */
    ~RealMat();

    //! Returns the numbers of Row
    u_int rows() const {
        return nrows;
    };
    //! Returns the numbers of Columns
    u_int cols() const {
        return ncols;
    };
    //! Returns the total numbers of elements (Rows*Columns)
    u_int size() const {
        return tsize;
    };

    //! Resize the matrix
    void resize( u_int rows, u_int cols );

    //! Return a reference to element at position (row, col)
    Real& at( u_int row, u_int col ) {
#ifdef NNFW_DEBUG
        if ( row >= nrows ) {
            nnfwMessage( NNFW_ERROR, "Accessing an element beyond Row boundary of matrix" );
            return data[0];
        }
        if ( col >= ncols ) {
            nnfwMessage( NNFW_ERROR, "Accessing an element beyond Column boundary of matrix" );
            return data[0];
        }
#endif
        return rowView[row][col];
    };

    //! Return a Const reference to element at position (row, col)
    const Real& at( u_int row, u_int col ) const {
#ifdef NNFW_DEBUG
        if ( row >= nrows ) {
            nnfwMessage( NNFW_ERROR, "Accessing an element beyond Row boundary of matrix" );
            return data[0];
        }
        if ( col >= ncols ) {
            nnfwMessage( NNFW_ERROR, "Accessing an element beyond Column boundary of matrix" );
            return data[0];
        }
#endif
        return rowView[row][col];
    };

    /*! \brief Indexing operator
     *  Boundary check activated only when DEBUG if defined
     */
    RealVec& operator[]( u_int row ) {
#ifdef NNFW_DEBUG
        if( row >= nrows ) {
            nnfwMessage( NNFW_ERROR, "Accessing elements outside boundary" );
            return rowView[0];
        }
#endif
        return rowView[row];
    };

    /*! \brief Indexing operator (Const Version)
     *  Boundary check activated only when DEBUG if defined
     */
    const RealVec& operator[]( u_int row ) const {
#ifdef NNFW_DEBUG
        if( row >= nrows ) {
            nnfwMessage( NNFW_ERROR, "Accessing elements outside boundary" );
            return rowView[0];
        }
#endif
        return rowView[row];
    };

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
    };

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

protected:
    /*! \brief Row Data allocated
     */
    Real* rawdata() const {
        return data.rawdata();
    };

private:
    //! Numbers of Rows
    u_int nrows;
    //! Numbers of Columns
    u_int ncols;
    //! Size
    u_int tsize;
    //! RealVec of data
    RealVec data;
    //! Vector of RealVec views of data
    VectorData<RealVec> rowView;

    /*! \brief Copy-Constructor
     */
    RealMat( const RealMat& orig );

    /*! \brief Assignment Operator
     */
    RealMat& operator=( const RealMat& src );
};

}

#endif

