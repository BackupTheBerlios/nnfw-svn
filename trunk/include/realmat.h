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
#error "You can't include realmat.h directly; Instead, You have to include types.h"
// --- follow define avoid to get a lot of understandable error !
#define REALMAT_H
#endif

#ifndef REALMAT_H
#define REALMAT_H

/*! \file
 *  \brief This file contains the RealMat Class; Don't include this file directly, instead include types.h
 */

namespace nnfw {

/*! \brief RealMat Class
 *  \par Motivation
 *  \par Description
 *  \par Warnings
 */
class NNFW_API RealMat : public MatrixData<Real, RealVec> {
public:
    /*! \name Constructors */
    //@{

    /*! Construct matrix of dimension size initialized to zero
     */
    RealMat( u_int rows, u_int cols );

    /*! Construct a Matrix view from VectorData passed
     *  \param src is the RealVec from which this method constructs a RealMat view.
     *  <b>See Documentation about MatrixData view constructor</b>
     */
    RealMat( RealVec& src, u_int rstart, u_int rend, u_int rows, u_int cols );

    /*! Destructor
     */
    ~RealMat();

    //@}
    /*! \name Unary Operators */
    //@{

    /*! Operator - */
    RealMat& operator-() {
        -rawdata();
        return (*this);
    };

    /*! Transpose this RealMat */
    RealMat& transpose() {
        Real tmp;
        RealMat& self = *this;
        for( u_int i=0; i<rows(); i++ ) {
            for( u_int j=i+1; j<cols(); j++ ) {
                tmp = self[i][j];
                self[i][j] = self[j][i];
                self[j][i] = tmp;
            }
        }
        return (*this);
    };

    //@}
    
	/*! \name Binary Operators */
    //@{

    /*! Operator += with RealMat */
    RealMat& operator+=(const RealMat& r ) {
#ifdef NNFW_DEBUG
        if( rows() != r.rows() || cols() != r.cols() ) {
            nError() << "Different dimension";
            return (*this);
        }
#endif
        rawdata() += r.rawdata();
        return (*this);
    };
    
	/*! Operator += with Real */
    RealMat& operator+=(const Real& r ) {
        rawdata() += r;
        return (*this);
    };
    
	/*! Operator -= with RealMat */
    RealMat& operator-=(const RealMat& r ) {
#ifdef NNFW_DEBUG
        if( rows() != r.rows() || cols() != r.cols() ) {
            nError() << "Different dimension";
            return (*this);
        }
#endif
        rawdata() -= r.rawdata();
        return (*this);
    };
    
	/*! Operator -= with Real */
    RealMat& operator-=(const Real& r ) {
        rawdata() -= r;
        return (*this);
    };
    
	/*! Operator *= with RealMat */
    RealMat& operator*=(const RealMat& r ) {
#ifdef NNFW_DEBUG
        if( rows() != r.rows() || cols() != r.cols() ) {
            nError() << "Different dimension";
            return (*this);
        }
#endif
        rawdata() *= r.rawdata();
        return (*this);
    };
    
	/*! Operator *= with Real */
    RealMat& operator*=(const Real& r ) {
        rawdata() *= r;
        return (*this);
    };
    
	/*! Operator /= with RealMat */
    RealMat& operator/=(const RealMat& r ) {
#ifdef NNFW_DEBUG
        if( rows() != r.rows() || cols() != r.cols() ) {
            nError() << "Different dimension";
            return (*this);
        }
#endif
        rawdata() /= r.rawdata();
        return (*this);
    };
    
	/*! Operator /= with Real */
    RealMat& operator/=(const Real& r ) {
        rawdata() /= r;
        return (*this);
    };

    //@}
    
	/*! \name Vector-Matrix Operators */
    //@{

    /*! Right Multiplication: y += x*m
     *  \param y the result of multiplication
     *  \param x the vector
     *  \param m the matrix
     *  \return the vector y
     */
    static RealVec& mul( RealVec& y, const RealVec& x, const RealMat& m );

    /*! Left Multiplication: y += m*x
     *  \param y the result of multiplication
     *  \param m the matrix
     *  \param x the vector
     *  \return the vector y
     */
    static RealVec& mul( RealVec& y, const RealMat& m, const RealVec& x );

	/*! Delta-Rule: m += rate * x * y<br>
	 *  It return itself
	 *  \param rate is the factor of multiplicaton
	 *  \param x is the first vector, its dimension have to be the same of matrix's rows
	 *  \param y is the second vector, its dimension have to be the same of matrix's columns
	 */
	RealMat& deltarule( Real rate, const RealVec& x, const RealVec& y );

    //@}
    /*! \name Matrix-Matrix Operators */
    //@{

	/*! Put to zero all elements at positions where mask elements are false */
	RealMat& cover( const MatrixData<bool>& mask ) {
		RealMat& self = *this;
		for( u_int r=0; r<rows(); r++ ) {
			for( u_int c=0; c<cols(); c++ ) {
				if ( !mask[r][c] ) {
					self[r][c] = 0.0;
				}
			}
		}
		return (*this);
	};

    //@}
    /*! \name Math Functions */
    //@{

    /*! Exponential */
    RealMat& exp();

    /*! Scaling (scalar-vector multiply) */
    RealMat& scale( const Real v ) {
        for( u_int i=0; i<size(); i++ ) {
            rawdata()[i] *= v;
        }
        return (*this);
    };

    /*! Element Inversion */
    RealMat& inv();

	/*! "Entrywise" Norm of order 2 or Frobenius norm */
	Real norm() {
		return rawdata().norm();
	};
	/*! normalize the matrix, so the "entrywise" norm is one */
	void normalize() {
		rawdata().normalize();
	};

    //@}

	//--- for accessing from C interface implementation
	friend Real* getRawData( RealMat& );

};

}

#endif

