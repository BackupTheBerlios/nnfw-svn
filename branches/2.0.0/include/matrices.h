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

#ifndef MATRICES_H
#define MATRICES_H

/*! \file
 *  \brief This file contains definitions of Matrices used for algebra
 */

#include "memutils.h"
#include "vectors.h"

namespace nnfw {

/*! \brief DoubleMatrix Class
 *  \par Motivation
 *  \par Description
 *  \par Warnings
 */
class NNFW_API DoubleMatrix {
public:
	/*! \name Constructors */
	//@{
	/*! Construct an empty matrix of dimension size */
	DoubleMatrix( unsigned int rows, unsigned int cols, bool isprotected = false ) {
		shData = new sharedData();
		shData->refcounts = 1;
		shData->nrows = rows;
		shData->ncols = cols;
		shData->tsize = rows*cols;
		shData->temporary = false;
		shData->alldata = new double[ shData->tsize ];
		shData->rowdata = new DoubleVector[ rows ];
		shData->coldata = new DoubleVector[ cols ];
		for( unsigned int i=0; i<rows; i++ ) {
			shData->rowdata[i].resizeNoData( cols );
		}
		for( unsigned int i=0; i<cols; i++ ) {
			shData->coldata[i].resizeNoData( rows );
		}
		for( unsigned int t=0; t<shData->tsize; t++ ) {
			unsigned int r = t/cols; //--- division may be expensive
			unsigned int c = t%cols; //--- module may be expensive
			shData->rowdata[r].shData->dataref[c].setRef( shData->alldata + t );
			shData->coldata[c].shData->dataref[r].setRef( shData->alldata + t );
		}
		this->isprotected = isprotected;
	};
	/*! Copy-Constructor */
	DoubleMatrix( const DoubleMatrix& src ) {
		shData = src.shData;
		shData->refcounts++;
		//--- is not a temporary anymore !
		shData->temporary = false;
		isprotected = false;
	};
	/*! Destructor */
	~DoubleMatrix() {
		shData->refcounts -= 1;
		if ( shData->refcounts == 0 ) {
			//--- the last destroy the data
			if ( shData->tsize > 0 ) {
				delete [](shData->rowdata);
				delete [](shData->coldata);
				delete [](shData->alldata);
			}
			delete shData;
		}
	};
	//@}
	/*! \name Informations about MatrixData */
	//@{
	/*! Returns the numbers of Row */
	unsigned int rows() const {
		return shData->nrows;
	};
	/*! Returns the numbers of Columns */
	unsigned int cols() const {
		return shData->ncols;
	};
	/*! Returns the total numbers of elements (Rows*Columns) */
	unsigned int size() const {
		return shData->tsize;
	};
	/*! Equal Operator */
	bool operator==( const DoubleMatrix& right ) {
		if ( shData == right.shData ) return true;
		if ( shData->tsize != right.shData->tsize ) return false;
		if ( shData->nrows != right.shData->nrows ) return false;
		if ( shData->ncols != right.shData->ncols ) return false;
		for( unsigned int i=0; i<shData->tsize; i++ ) {
			if ( shData->alldata[i] != right.shData->alldata[i] ) return false;
		}
		return true;
	};
	/*! Not-Equal Operator */
	bool operator!=( const DoubleMatrix& right ) {
		return !( *this == right );
	};
	//@}
	/*! \name Operations on DoubleMatrix */
	//@{
	/*! Resize the matrix */
	DoubleMatrix& resize( unsigned int rows, unsigned int cols ) {
		unsigned int newsize = rows*cols;
		double* tmp = new double[newsize];
		memoryCopy( tmp, shData->alldata, newsize );
		shData->alldata = tmp;
		for( unsigned int i=0; i<rows; i++ ) {
			shData->rowdata[i].resizeNoData( cols );
		}
		for( unsigned int i=0; i<cols; i++ ) {
			shData->coldata[i].resizeNoData( rows );
		}
		shData->nrows = rows;
		shData->ncols = cols;
		shData->tsize = newsize;
		for( unsigned int t=0; t<shData->tsize; t++ ) {
			unsigned int r = t/cols; //--- division may be expensive
			unsigned int c = t%cols; //--- module may be expensive
			shData->rowdata[r].shData->dataref[c].setRef( shData->alldata + t );
			shData->coldata[c].shData->dataref[r].setRef( shData->alldata + t );
		}
		return (*this);
	};
	/*! Behaves as a CopyConstructor: this do a completely substitution of underlying data
	 *  so, it does not honor fixed elements, hence it substitute all
	 *  informations taking also the new information about fixed element in src.<br/>
	 *  If you want only copying DoubleMatrix's values use copy method
	 */
	DoubleMatrix& operator=( const DoubleMatrix& src ) {
		if ( isprotected ) return (*this);
		if ( shData == src.shData ) return (*this);
		//--- eliminate the previous data
		shData->refcounts -= 1;
		if ( shData->refcounts == 0 ) {
			//--- the last destroy the data
			if ( shData->tsize > 0 ) {
				delete [](shData->rowdata);
				delete [](shData->coldata);
				delete [](shData->alldata);
			}
			delete shData;
		}
		//--- set the new data taken from src
		shData = src.shData;
		shData->refcounts++;
		//--- is not a temporary anymore !
		shData->temporary = false;
		return (*this);
	};
	/*! Copy the values from source honoring fixed elements
	 *  (i.e.: the fixed element of this remains unchanged )
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix& copy( const DoubleMatrix& src ) {
#ifdef NNFW_DEBUG
		if ( shData->tsize != src.shData->tsize || shData->nrows != src.shData->nrows || shData->ncols != src.shData->ncols ) {
			qCritical() << "Incompatibles Matrices in copy method (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int r=0; r<shData->nrows; r++ ) {
			for( unsigned int c=0; c<shData->ncols; c++ ) {
				shData->rowdata[r][c] = src.shData->rowdata[r][c];
			}
		}
		return (*this);
	};
	/*! It behaves exactly as copy method and not as operator= */
	DoubleMatrix& assignData( const DoubleMatrix& src ) {
		return copy( src );
	};
	/*! If the current data is shared by other objects, this method will create a new copy of the data
	 *  not shared by other objects.
	 */
	DoubleMatrix& detach() {
		if ( shData->refcounts > 1 ) {
			sharedData* tmp = new sharedData();
			tmp->refcounts = 1;
			tmp->nrows = shData->nrows;
			tmp->ncols = shData->ncols;
			tmp->tsize = shData->tsize;
			tmp->temporary = false;
			tmp->alldata = new double[shData->tsize];
			tmp->rowdata = new DoubleVector[ shData->nrows ];
			tmp->coldata = new DoubleVector[ shData->ncols ];
			for( unsigned int i=0; i<shData->nrows; i++ ) {
				tmp->rowdata[i].resizeNoData( shData->ncols );
			}
			for( unsigned int i=0; i<shData->ncols; i++ ) {
				tmp->coldata[i].resizeNoData( shData->nrows );
			}
			for( unsigned int t=0; t<tmp->tsize; t++ ) {
				unsigned int r = t/( tmp->ncols ); //--- division may be expensive
				unsigned int c = t%( tmp->ncols ); //--- module may be expensive
				tmp->rowdata[r].shData->dataref[c].setRef( tmp->alldata + t );
				tmp->coldata[c].shData->dataref[r].setRef( tmp->alldata + t );
			}
			shData->refcounts--;
			shData = tmp;
		}
		return (*this);
	};
	/*! Return a reference to element at position (row, col) */
	doubleRef& at( unsigned int row, unsigned int col ) {
#ifdef NNFW_DEBUG
		if ( row >= shData->nrows ) {
			qCritical() << "Accessing an element beyond Row boundary of matrix" ;
			return shData->rowdata[0][0];
		}
		if ( col >= shData->ncols ) {
			qCritical() << "Accessing an element beyond Column boundary of matrix" ;
			return shData->rowdata[0][0];
		}
#endif
		return shData->rowdata[row][col];
    };
	/*! Return a Const reference to element at position (row, col) */
	const doubleRef& at( unsigned int row, unsigned int col ) const {
#ifdef NNFW_DEBUG
		if ( row >= shData->nrows ) {
			qCritical() << "Accessing an element beyond Row boundary of matrix" ;
			return shData->rowdata[0][0];
		}
		if ( col >= shData->ncols ) {
			qCritical() << "Accessing an element beyond Column boundary of matrix" ;
			return shData->rowdata[0][0];
		}
#endif
		return shData->rowdata[row][col];
    };
	/*! Indexing operator<br>
	 *  \warning Boundary check activated only when DEBUG if defined
	 */
	DoubleVector& operator[]( unsigned int row ) {
#ifdef NNFW_DEBUG
		if( row >= shData->nrows ) {
			qCritical() << "Accessing elements outside boundary" ;
			return shData->rowdata[0];
		}
#endif
		return shData->rowdata[row];
    };
	/*! Indexing operator (Const Version)<br>
	 *  Boundary check activated only when DEBUG if defined
	 */
	const DoubleVector& operator[]( unsigned int row ) const {
#ifdef NNFW_DEBUG
		if( row >= shData->nrows ) {
			qCritical() << "Accessing elements outside boundary" ;
			return shData->rowdata[0];
		}
#endif
		return shData->rowdata[row];
    };
	/*! Return the i-th row of the Matrix (this behaves like operator[]) */
	DoubleVector& row( unsigned int r ) {
#ifdef NNFW_DEBUG
		if( r >= shData->nrows ) {
			qCritical() << "Accessing elements outside boundary" ;
			return shData->rowdata[0];
		}
#endif
		return shData->rowdata[r];
	};
	/*! Return the i-th row of the Matrix (Const version) (this behaves like operator[]) */
	const DoubleVector& row( unsigned int r ) const {
#ifdef NNFW_DEBUG
		if( r >= shData->nrows ) {
			qCritical() << "Accessing elements outside boundary" ;
			return shData->rowdata[0];
		}
#endif
		return shData->rowdata[r];
	};
	/*! Return the i-th column of the Matrix */
	DoubleVector& column( unsigned int c ) {
#ifdef NNFW_DEBUG
		if( c >= shData->ncols ) {
			qCritical() << "Accessing elements outside boundary" ;
			return shData->coldata[0];
		}
#endif
		return shData->coldata[c];
	};
	/*! Return the i-th column of the Matrix (Const version) */
	const DoubleVector& column( unsigned int c ) const {
#ifdef NNFW_DEBUG
		if( c >= shData->ncols ) {
			qCritical() << "Accessing elements outside boundary" ;
			return shData->coldata[0];
		}
#endif
		return shData->coldata[c];
	};
	/*! Fix the [i][j]-th value, hence the [i][j]-th value will never change anymore
	 *  until unsteady is called
	 *  \warning Boundary check activated only when DEBUG if defined
	 */
	DoubleMatrix& steady( unsigned int i, unsigned int j ) {
#ifdef NNFW_DEBUG
		if( i >= shData->nrows || j >= shData->ncols ) {
			qCritical() << "Fixating elements outside boundary" ;
			return (*this);
		}
#endif
		shData->rowdata[i].steady(j);
		shData->coldata[j].steady(i);
		return (*this);
	};
	/*! Reallow the modification of [i][j]-th value
	 *  \warning Boundary check activated only when DEBUG if defined
	 */
	DoubleMatrix& unsteady( unsigned int i, unsigned int j ) {
#ifdef NNFW_DEBUG
		if( i >= shData->nrows || j >= shData->ncols ) {
			qCritical() << "Un-Fixating elements outside boundary" ;
			return (*this);
		}
#endif
		shData->rowdata[i].unsteady(j);
		shData->coldata[j].unsteady(i);
		return (*this);
	};
	/*! Return true if the [i][j]-th value is a steady value */
	bool isSteady( unsigned int i, unsigned int j ) {
#ifdef NNFW_DEBUG
		if( i >= shData->nrows || j >= shData->ncols ) {
			qCritical() << "Accessing elements outside boundary" ;
			return false;
		}
#endif
		return shData->rowdata[i].isSteady(j);
	};
	/*! Set all values to value
	 *  This method honor the steady values... hence that values will remain at the same values
	 */
	DoubleMatrix& setAll( const double value ) {
		for( unsigned int r=0; r<shData->nrows; r++ ) {
			for( unsigned int c=0; c<shData->ncols; c++ ) {
				shData->rowdata[r][c] = value;
			}
		}
		return (*this);
	};
	/*! Set all diagonal values to 1 and others to zero
	 *  This method honor the steady values... hence that values will remain at the same values
	 */
	DoubleMatrix& setIdentity() {
		for( unsigned int r=0; r<shData->nrows; r++ ) {
			for( unsigned int c=0; c<shData->ncols; c++ ) {
				shData->rowdata[r][c] = (r==c);
			}
		}
		return (*this);
	};
	/*! Set all values to zero
	 *  This method honor the steady values... hence that values will remain at the same values
	 */
	DoubleMatrix& zeroing() {
		for( unsigned int r=0; r<shData->nrows; r++ ) {
			for( unsigned int c=0; c<shData->ncols; c++ ) {
				shData->rowdata[r][c] = 0;
			}
		}
		return (*this);
	};
	//@}
	/*! \name Operations between DoubleMatrices */
	//@{
	/*! operator +
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix operator+( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			qCritical() << "Incompatibles Matrices in operator + (dimension must be equals)";
			return (*this);
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				right.shData->alldata[i] = shData->alldata[i] + right.shData->alldata[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				shData->alldata[i] = shData->alldata[i] + right.shData->alldata[i];
			}
			return (*this);
		} else {
			DoubleMatrix ret( shData->nrows, shData->ncols );
			ret.shData->temporary = true;
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				ret.shData->alldata[i] = shData->alldata[i] + right.shData->alldata[i];
			}
			return ret;
		}
	};
	/*! operator -
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix operator-( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			qCritical() << "Incompatibles Matrices in operator - (dimension must be equals)";
			return (*this);
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				right.shData->alldata[i] = shData->alldata[i] - right.shData->alldata[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				shData->alldata[i] = shData->alldata[i] - right.shData->alldata[i];
			}
			return (*this);
		} else {
			DoubleMatrix ret( shData->nrows, shData->ncols );
			ret.shData->temporary = true;
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				ret.shData->alldata[i] = shData->alldata[i] - right.shData->alldata[i];
			}
			return ret;
		}
	};
	/*! operator * (matrix multiplication)
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 *  \warning It always allocate a new matrix on memory
	 */
	DoubleMatrix operator*( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->ncols != right.shData->nrows ) {
			qCritical() << "Incompatibles Matrices in operator * ";
			return (*this);
		}
#endif
		DoubleMatrix ret( shData->nrows, right.shData->ncols );
		ret.shData->temporary = true;
		ret.zeroing();
		for( unsigned int i=0; i<ret.shData->nrows; i++ ) {
			for( unsigned int j=0; j<ret.shData->ncols; j++ ) {
				for( unsigned int r=0; r<shData->ncols; r++ ) {
					ret[i][j] += shData->rowdata[i][r] * right[r][j];
				}
			}
		}
		return ret;
	};
	/*! operator % (element-by-element multiplication)
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix operator%( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			qCritical() << "Incompatibles Matrices in operator % (dimension must be equals)";
			return (*this);
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
					right.shData->alldata[i] = shData->alldata[i] * right.shData->alldata[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
					shData->alldata[i] = shData->alldata[i] * right.shData->alldata[i];
			}
			return (*this);
		} else {
			DoubleMatrix ret( shData->nrows, shData->ncols );
			ret.shData->temporary = true;
			for( unsigned int i=0; i<shData->tsize; i++ ) {
					ret.shData->alldata[i] = shData->alldata[i] * right.shData->alldata[i];
			}
			return ret;
		}
	};
	/*! operator / (element-by-element)
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix operator/( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			qCritical() << "Incompatibles Matrices in operator / (dimension must be equals)";
			return (*this);
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				right.shData->alldata[i] = shData->alldata[i] / right.shData->alldata[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				shData->alldata[i] = shData->alldata[i] / right.shData->alldata[i];
			}
			return (*this);
		} else {
			DoubleMatrix ret( shData->nrows, shData->ncols );
			ret.shData->temporary = true;
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				ret.shData->alldata[i] = shData->alldata[i] / right.shData->alldata[i];
			}
			return ret;
		}
	};
	/*! operator +=
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix& operator+=( const DoubleMatrix& right ) {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			qCritical() << "Incompatibles Matrices in operator += (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int r=0; r<shData->nrows; r++ ) {
			for( unsigned int c=0; c<shData->ncols; c++ ) {
				shData->rowdata[r][c] += right.shData->rowdata[r][c];
			}
		}
		return (*this);
	};
	/*! operator -=
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix& operator-=( const DoubleMatrix& right ) {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			qCritical() << "Incompatibles Matrices in operator -= (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int r=0; r<shData->nrows; r++ ) {
			for( unsigned int c=0; c<shData->ncols; c++ ) {
				shData->rowdata[r][c] -= right.shData->rowdata[r][c];
			}
		}
		return (*this);
	};
	/*! operator *= (matrix multiplication)
	 *  \warning this method resize this matrix and all informations about steady values will be lost.
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix& operator*=( const DoubleMatrix& right ) {
#ifdef NNFW_DEBUG
		if ( shData->ncols != right.shData->nrows ) {
			qCritical() << "Incompatibles Matrices in operator *= ";
			return (*this);
		}
#endif
		DoubleMatrix ret( shData->nrows, right.shData->ncols );
		ret.shData->temporary = true;
		ret.zeroing();
		for( unsigned int i=0; i<ret.shData->nrows; i++ ) {
			for( unsigned int j=0; j<ret.shData->ncols; j++ ) {
				for( unsigned int r=0; r<shData->ncols; r++ ) {
					ret[i][j] += shData->rowdata[i][r] * right[r][j];
				}
			}
		}
		(*this) = ret;
		return *this;
	};
	/*! operator %= (element-by-element multiplication)
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix& operator%=( const DoubleMatrix& right ) {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			qCritical() << "Incompatibles Matrices in operator %= (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int r=0; r<shData->nrows; r++ ) {
			for( unsigned int c=0; c<shData->ncols; c++ ) {
				shData->rowdata[r][c] *= right.shData->rowdata[r][c];
			}
		}
		return (*this);
	};
	/*! operator /= (element-by-element)
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix& operator/=( const DoubleMatrix& right ) {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			qCritical() << "Incompatibles Matrices in operator /= (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int r=0; r<shData->nrows; r++ ) {
			for( unsigned int c=0; c<shData->ncols; c++ ) {
				shData->rowdata[r][c] /= right.shData->rowdata[r][c];
			}
		}
		return (*this);
	};
	//@}
	/*! \name Operations between DoubleMatrix and DoubleVector */
	//@{
	//@}

private:
	// putting this NNFW_INTERNAL GCC comply with a warning
	class sharedData {
	public:
		/*! Numbers of Rows */
		unsigned int nrows;
		/*! Numbers of Columns */
		unsigned int ncols;
		/*! Size */
		unsigned int tsize;
		/*! Vector containing all data in row-major */
		double* alldata;
		/*! Vectors arranged to return rows of the matrix */
		DoubleVector* rowdata;
		/*! Vectors arranged to return columns of the matrix */
		DoubleVector* coldata;
		/*! references count */
		int refcounts;
		/*! if is temporary (used to avoid multiple copying of data during math expressions) */
		bool temporary;
	};
	/*! shared data  among Matrix istances */
	sharedData* shData;
	/*! if the vector is protected means that assignment operator= is not allowed */
	bool isprotected;

};

}

#endif

