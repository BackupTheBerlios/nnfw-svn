/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2011 Gianluca Massera <emmegian@yahoo.it>                *
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
#include <exception>

namespace nnfw {

/*! \brief DoubleMatrix Class
 *  This represent a 2D matrix of double values
 *  \par Description
 *  The DoubleMatrix use an implicit-sharing mechanism based on copy-on-write
 *  \par Warnings
 *  This class is also used by other class for structuring their underlyng data. In these case, changing the structure of
 *  a DoubleMatrix using directly the assign operator= can result in uncoerent state of the object owning the DoubleMatrix.
 *  There special constructor that takes a boolean value, isinternal, for flag such situations.
 *  When a user attempt to call the operator= on a DoubleMatrix with isinternal flag on an Exception will be throw.
 */
class NNFW_API DoubleMatrix {
public:
	/*! \name Constructors */
	//@{
	/*! Construct an empty matrix of dimension size */
	DoubleMatrix( unsigned int rows, unsigned int cols, bool isinternal = false ) {
		shData = new sharedData();
		shData->refcounts = 1;
		shData->nrows = rows;
		shData->ncols = cols;
		shData->tsize = rows*cols;
		shData->temporary = false;
		shData->alldata = new double[ shData->tsize ];
		shData->alldataref = new doubleRef[ shData->tsize ];
		shData->rowdata = new DoubleVector[ rows ];
		shData->coldata = new DoubleVector[ cols ];
		for( unsigned int i=0; i<rows; i++ ) {
			// this will also activate isinternal flag on DoubleVectors
			shData->rowdata[i].resizeNoData( cols );
		}
		for( unsigned int i=0; i<cols; i++ ) {
			// this will also activate isinternal flag on DoubleVectors
			shData->coldata[i].resizeNoData( rows );
		}
		for( unsigned int t=0; t<shData->tsize; t++ ) {
			unsigned int r = t/cols; //--- division may be expensive
			unsigned int c = t%cols; //--- module may be expensive
			shData->rowdata[r].shData->dataref[c].setRef( shData->alldata + t );
			shData->coldata[c].shData->dataref[r].setRef( shData->alldata + t );
			shData->alldataref[t].setRef( shData->alldata + t );
		}
		this->isinternal = isinternal;
	};
	/*! Copy-Constructor */
	DoubleMatrix( const DoubleMatrix& src ) {
		shData = src.shData;
		shData->refcounts++;
		//--- is not a temporary anymore !
		shData->temporary = false;
		isinternal = false;
	};
	/*! This is a special constructor used for copy a DoubleMatrix and mark it as temporary for speed-up
	 *  calculation.
	 *  It's intended for internal use only
	 *  DON'T USE UNLESS YOUR ARE VERY CONFIDENT on How the special temporary DoubleMatrix are used
	 *  \internal
	 *  The idea behind the temporary DoubleMatrix works in this way:
	 *  when an operator is called, and it has to create a new fresh copy of the data and return it, then
	 *  it will call this constructor that will allocate new data for storing the calculations.
	 *  After that, the DoubleMatrix is returned by the operator and from now it is marked as temporary
	 *  If the DoubleMatrix will be passed to another operator (like in chained operations), then the operator
	 *  instead of creating the new data again, it will use the DoubleMatrix marked as temporary
	 *  At the end of operations it will be assigned and then the operator= unmark the DoubleMatrix as temporary
	 *  For example, suppose this code:
	 *  \code
	 *  DoubleMatrix v1 = v2 + v3*v4 - v5;
	 *  \endcode
	 *  When the first operator* is called, it will create a temporary DoubleMatrix and then all others operators will
	 *  write on it, reducing the allocation of memory from three to one !
	 *  In the example above, because the DoubleMatrix v1 has been created exactly when the operations have been done,
	 *  then v1 still marked as temporary, then successive operations like the example below will not allocate further
	 *  memory:
	 *  \code
	 *  DoubleMatrix v1 = v2 + v3*v4 - v5;
	 *  DoubleMatrix v6 = v1*v4 + v2;
	 *  \endcode
	 *  A DoubleMatrix will be unmarked as temporary when the copy-constructor and the operator= will be called.
	 *  \param dummy is not used, and exists only to avoid that the constructor has the same signature
	 *   of DoubleMatrix( unsigned int, unsigned int, bool internal )
	 */
	DoubleMatrix(  unsigned int rows, unsigned int cols, bool temp, unsigned int dummy ) {
		dummy = 0; // just for avoid the warning :-P
		shData = new sharedData();
		shData->refcounts = 1;
		shData->nrows = rows;
		shData->ncols = cols;
		shData->tsize = rows*cols;
		shData->temporary = temp;
		shData->alldata = new double[ shData->tsize ];
		shData->alldataref = new doubleRef[ shData->tsize ];
		shData->rowdata = new DoubleVector[ rows ];
		shData->coldata = new DoubleVector[ cols ];
		for( unsigned int i=0; i<rows; i++ ) {
			// this will also activate isinternal flag on DoubleVectors
			shData->rowdata[i].resizeNoData( cols );
		}
		for( unsigned int i=0; i<cols; i++ ) {
			// this will also activate isinternal flag on DoubleVectors
			shData->coldata[i].resizeNoData( rows );
		}
		for( unsigned int t=0; t<shData->tsize; t++ ) {
			unsigned int r = t/cols; //--- division may be expensive
			unsigned int c = t%cols; //--- module may be expensive
			shData->rowdata[r].shData->dataref[c].setRef( shData->alldata + t );
			shData->coldata[c].shData->dataref[r].setRef( shData->alldata + t );
			shData->alldataref[t].setRef( shData->alldata + t );
		}
		this->isinternal = false;
	};
	/*! Destructor */
	~DoubleMatrix() {
		shData->refcounts -= 1;
		if ( shData->refcounts == 0 ) {
			//--- the last destroy the data
			if ( shData->tsize > 0 ) {
				delete [](shData->rowdata);
				delete [](shData->coldata);
				delete [](shData->alldataref);
				delete [](shData->alldata);
			}
			delete shData;
		}
	};
	//@}
	/*! \name Exceptions throw by DoubleMatrix */
	//@{
	/*! Thrown when a user attempt to call the operator= on a DoubleMatrix with isinternal flag on */
	class NNFW_API AssignmentNotAllowed : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "It is not allowed to use the operator=() on DoubleMatrix used as internal data\n";
		};
	};
	/*! Thrown when a user attempt to resize a DoubleMatrix with isinternal flag on */
	class NNFW_API ResizeNotAllowed : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "It is not allowed to resize a DoubleMatrix used as internal data\n";
		};
	};
	/*! Thrown when a user attempt to do calculations with incompatible DoubleMatrix */
	class NNFW_API IncompatibleMatrices : public std::exception {
	public:
		IncompatibleMatrices( const char* why ) {
			this->why = why;
		};
		virtual const char* what() const throw() {
			return why;
		};
	private:
		const char* why;
	};
	/*! Thrown when a user attempt to access outside boundary of a DoubleMatrix */
	class NNFW_API OutsideBoundaries : public std::exception {
	public:
		OutsideBoundaries( const char* why ) {
			this->why = why;
		};
		virtual const char* what() const throw() {
			return why;
		};
	private:
		const char* why;
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
	bool operator==( const DoubleMatrix& right ) const {
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
	bool operator!=( const DoubleMatrix& right ) const {
		return !( *this == right );
	};
	//@}
	/*! \name Operations on DoubleMatrix */
	//@{
	/*! Resize the matrix
	 *  \warning If the isinternal flag is true, then the resize will raise the ResizeNotAllowed
	 */
	DoubleMatrix& resize( unsigned int rows, unsigned int cols ) {
		if ( isinternal ) throw ResizeNotAllowed();
		DoubleMatrix newMatrix( rows, cols );
		newMatrix.copyValues( *this );
		//--- the assignment will release the shared data, so there is no need to call detach()
		(*this) = newMatrix;
		return (*this);
	};
	/*! Behaves as a CopyConstructor: this do a completely substitution of underlying data
	 *  so, it does not honor fixed elements, hence it substitute all
	 *  informations taking also the new information about fixed element in src.<br/>
	 *  If you want copy only DoubleMatrix's values use copy method
	 *  \warning If the isinternal flag is true, then the operator= will raise the AssignmentNotAllowed
	 */
	DoubleMatrix& operator=( const DoubleMatrix& src ) {
		if ( isinternal ) throw AssignmentNotAllowed();
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
	 *  \note this method does not check the dimension of the two matrices, but behaves safely
	 *    calculating the minimal boundaries between source and destination, and it will copy only
	 *    values in that range
	 */
	DoubleMatrix& copyValues( const DoubleMatrix& src ) {
		// if shared data is the same, they already share the same values
		if ( shData == src.shData ) return (*this);
		detach();
		unsigned int rowMax = qMin( shData->nrows, src.shData->nrows );
		unsigned int colMax = qMin( shData->ncols, src.shData->ncols );
		for( unsigned int r=0; r<rowMax; r++ ) {
			for( unsigned int c=0; c<colMax; c++ ) {
				shData->rowdata[r][c] = src.shData->rowdata[r][c];
			}
		}
		return (*this);
	};
	/*! Copy the values from source honoring fixed elements starting from the offsets specified
	 *  (i.e.: the fixed element of this remains unchanged )
	 *  \param srcRowOffset is the row offset from which the values are read from the src matrix
	 *  \param srcColOffset is the col offset from which the values are read from the src matrix
	 *  \param thisRowOffset is the row offset on which the values from the src matrix are placed
	 *  \param thisColOffset is the row offset on which the values from the src matrix are placed
	 *  \note this method does not check the dimension of the two vectors, but behaves safely
	 *        like the copyValues method
	 */
	DoubleMatrix& copyValues( const DoubleMatrix& src, unsigned int srcRowOffset, unsigned int srcColOffset, unsigned int thisRowOffset, unsigned int thisColOffset ) {
		// if shared data is the same and offsets are zero, then there is nothing to do
		if ( shData == src.shData && srcRowOffset == 0 && srcColOffset == 0 && thisRowOffset == 0 && thisColOffset == 0 ) {
			return (*this);
		}
		detach();
		unsigned int rowMax = qMin( shData->nrows-thisRowOffset, src.shData->nrows-srcRowOffset );
		unsigned int colMax = qMin( shData->ncols-thisColOffset, src.shData->ncols-thisColOffset );
		for( unsigned int r=0; r<rowMax; r++ ) {
			for( unsigned int c=0; c<colMax; c++ ) {
				shData->rowdata[r+thisRowOffset][c+thisColOffset] = src.shData->rowdata[r+srcRowOffset][c+srcColOffset];
			}
		}
		return (*this);
	};
	/*! Copy the values from source honoring fixed elements starting from the offsets specified
	 *  (i.e.: the fixed element of this remains unchanged )
	 *  \param srcRowOffset is the row offset from which the values are read from the src matrix
	 *  \param srcColOffset is the col offset from which the values are read from the src matrix
	 *  \param thisRowOffset is the row offset on which the values from the src matrix are placed
	 *  \param thisColOffset is the row offset on which the values from the src matrix are placed
	 *  \param rowStride is the stride; stride zero is not allowed, stride == 1 behaves like copyValues without stride,
	 *        stride > 1 it jump 'stride' rows when coping, leaving the jumped values unchanged
	 *  \param colStride is the stride; stride zero is not allowed, stride == 1 behaves like copyValues without stride,
	 *        stride > 1 it jump 'stride' columuns when coping, leaving the jumped values unchanged
	 *  \note this method does not check the dimension of the two matrices, but behaves safely
	 *        like the copyValues method
	 */
	DoubleMatrix& copyValues( const DoubleMatrix& src, unsigned int srcRowOffset, unsigned int srcColOffset, unsigned int thisRowOffset, unsigned int thisColOffset, unsigned int rowStride, unsigned int colStride ) {
		if ( rowStride == 0 || colStride == 0 ) return (*this);
		// if shared data is the same and offsets are zero, then there is nothing to do
		if ( shData == src.shData && srcRowOffset == 0 && srcColOffset == 0 && thisRowOffset == 0 && thisColOffset == 0 ) {
			return (*this);
		}
		detach();
		unsigned int rowMax = qMin( shData->nrows-thisRowOffset, src.shData->nrows-srcRowOffset );
		unsigned int colMax = qMin( shData->ncols-thisColOffset, src.shData->ncols-thisColOffset );
		for( unsigned int r=0; r<rowMax; r=r+rowStride ) {
			for( unsigned int c=0; c<colMax; c=c+colStride ) {
				shData->rowdata[r+thisRowOffset][c+thisColOffset] = src.shData->rowdata[r+srcRowOffset][c+srcColOffset];
			}
		}
		return (*this);
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
	/*! Return a reference to element at position (row, col)
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	doubleRef& at( unsigned int row, unsigned int col ) {
#ifdef NNFW_DEBUG
		if ( row >= shData->nrows ) throw OutsideBoundaries("Accessing an element beyond Row boundary of matrix");
		if ( col >= shData->ncols ) throw OutsideBoundaries("Accessing an element beyond Column boundary of matrix");
#endif
		detach();
		return shData->rowdata[row][col];
    };
	/*! Return a Const reference to element at position (row, col)
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	double at( unsigned int row, unsigned int col ) const {
#ifdef NNFW_DEBUG
		if ( row >= shData->nrows ) throw OutsideBoundaries("Accessing an element beyond Row boundary of matrix");
		if ( col >= shData->ncols ) throw OutsideBoundaries("Accessing an element beyond Column boundary of matrix");
#endif
		return shData->rowdata[row][col];
    };
	/*! Indexing operator<br>
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	DoubleVector& operator[]( unsigned int row ) {
#ifdef NNFW_DEBUG
		if ( row >= shData->nrows ) throw OutsideBoundaries("Accessing an element beyond Row boundary of matrix");
#endif
		detach();
		return shData->rowdata[row];
    };
	/*! Indexing operator (Const Version)<br>
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	DoubleVector operator[]( unsigned int row ) const {
#ifdef NNFW_DEBUG
		if ( row >= shData->nrows ) throw OutsideBoundaries("Accessing an element beyond Row boundary of matrix");
#endif
		return shData->rowdata[row];
    };
	/*! Return the i-th row of the Matrix (this behaves like operator[]) */
	DoubleVector& row( unsigned int r ) {
#ifdef NNFW_DEBUG
		if ( r >= shData->nrows ) throw OutsideBoundaries("Accessing an element beyond Row boundary of matrix");
#endif
		detach();
		return shData->rowdata[r];
	};
	/*! Return the i-th row of the Matrix (Const version) (this behaves like operator[]) */
	DoubleVector row( unsigned int r ) const {
#ifdef NNFW_DEBUG
		if ( r >= shData->nrows ) throw OutsideBoundaries("Accessing an element beyond Row boundary of matrix");
#endif
		return shData->rowdata[r];
	};
	/*! Return the i-th column of the Matrix */
	DoubleVector& column( unsigned int c ) {
#ifdef NNFW_DEBUG
		if ( c >= shData->ncols ) throw OutsideBoundaries("Accessing an element beyond Column boundary of matrix");
#endif
		detach();
		return shData->coldata[c];
	};
	/*! Return the i-th column of the Matrix (Const version) */
	DoubleVector column( unsigned int c ) const {
#ifdef NNFW_DEBUG
		if ( c >= shData->ncols ) throw OutsideBoundaries("Accessing an element beyond Column boundary of matrix");
#endif
		return shData->coldata[c];
	};
	/*! Fix the [i][j]-th value, hence the [i][j]-th value will never change anymore
	 *  until unsteady is called
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	DoubleMatrix& steady( unsigned int i, unsigned int j ) {
#ifdef NNFW_DEBUG
		if( i >= shData->nrows || j >= shData->ncols ) throw OutsideBoundaries("Fixating elements outside boundary");
#endif
		detach();
		shData->rowdata[i].steady(j);
		shData->coldata[j].steady(i);
		shData->alldataref[ shData->nrows*i + j ].setSteady();
		return (*this);
	};
	/*! Reallow the modification of [i][j]-th value
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	DoubleMatrix& unsteady( unsigned int i, unsigned int j ) {
#ifdef NNFW_DEBUG
		if( i >= shData->nrows || j >= shData->ncols ) throw OutsideBoundaries("Un-Fixating elements outside boundary");
#endif
		detach();
		shData->rowdata[i].unsteady(j);
		shData->coldata[j].unsteady(i);
		shData->alldataref[ shData->nrows*i + j ].setNoSteady();
		return (*this);
	};
	/*! Return true if the [i][j]-th value is a steady value */
	bool isSteady( unsigned int i, unsigned int j ) const {
#ifdef NNFW_DEBUG
		if( i >= shData->nrows || j >= shData->ncols ) throw OutsideBoundaries("Accessing elements outside boundary");
#endif
		return shData->rowdata[i].isSteady(j);
	};
	/*! Set all values to value
	 *  This method honor the steady values... hence that values will remain at the same values
	 */
	DoubleMatrix& setAll( const double value ) {
		detach();
		for( unsigned int i=0; i<shData->tsize; i++ ) {
			shData->alldataref[i] = value;
		}
		return (*this);
	};
	/*! Set all diagonal values to 1 and others to zero
	 *  This method honor the steady values... hence that values will remain at the same values
	 */
	DoubleMatrix& setIdentity() {
		detach();
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
		detach();
		for( unsigned int i=0; i<shData->tsize; i++ ) {
			shData->alldataref[i] = 0;
		}
		return (*this);
	};
	//@}
	/*! \name Operations between DoubleMatrices */
	//@{
	/*! operator +
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	const DoubleMatrix operator+( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			throw IncompatibleMatrices("Incompatibles Matrices in operator+ (dimension must be equals)");
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				right.shData->alldataref[i] = shData->alldata[i] + right.shData->alldata[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				shData->alldataref[i] = shData->alldata[i] + right.shData->alldata[i];
			}
			return (*this);
		} else {
			DoubleMatrix ret( shData->nrows, shData->ncols, true, 0 );
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				ret.shData->alldataref[i] = shData->alldata[i] + right.shData->alldata[i];
			}
			return ret;
		}
	};
	/*! operator -
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	const DoubleMatrix operator-( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			throw IncompatibleMatrices("Incompatibles Matrices in operator- (dimension must be equals)");
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				right.shData->alldataref[i] = shData->alldata[i] - right.shData->alldata[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				shData->alldataref[i] = shData->alldata[i] - right.shData->alldata[i];
			}
			return (*this);
		} else {
			DoubleMatrix ret( shData->nrows, shData->ncols, true, 0 );
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				ret.shData->alldataref[i] = shData->alldata[i] - right.shData->alldata[i];
			}
			return ret;
		}
	};
	/*! operator * (matrix multiplication)
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 *  \warning It always allocate a new matrix on memory
	 */
	const DoubleMatrix operator*( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->ncols != right.shData->nrows ) {
			throw IncompatibleMatrices("Incompatibles Matrices in operator* (left matrix column dimension must be equal to right matrix row dimension)");
		}
#endif
		// FIXME: this is very slow and inefficient way to multiple matrices
		DoubleMatrix ret( shData->nrows, right.shData->ncols, true, 0 );
		ret.zeroing();
		for( unsigned int i=0; i<ret.shData->nrows; i++ ) {
			for( unsigned int j=0; j<ret.shData->ncols; j++ ) {
				for( unsigned int r=0; r<shData->ncols; r++ ) {
					ret.shData->rowdata[i][j] += shData->rowdata[i][r] * right.shData->rowdata[r][j];
				}
			}
		}
		return ret;
	};
	/*! operator % (element-by-element multiplication)
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	const DoubleMatrix operator%( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			throw IncompatibleMatrices("Incompatibles Matrices in operator% (dimension must be equals)");
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				right.shData->alldataref[i] = shData->alldata[i] * right.shData->alldata[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				shData->alldataref[i] = shData->alldata[i] * right.shData->alldata[i];
			}
			return (*this);
		} else {
			DoubleMatrix ret( shData->nrows, shData->ncols, true, 0 );
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				ret.shData->alldataref[i] = shData->alldata[i] * right.shData->alldata[i];
			}
			return ret;
		}
	};
	/*! operator / (element-by-element)
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	const DoubleMatrix operator/( const DoubleMatrix& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			throw IncompatibleMatrices("Incompatibles Matrices in operator/ (dimension must be equals)");
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				right.shData->alldataref[i] = shData->alldata[i] / right.shData->alldata[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				shData->alldataref[i] = shData->alldata[i] / right.shData->alldata[i];
			}
			return (*this);
		} else {
			DoubleMatrix ret( shData->nrows, shData->ncols, true, 0 );
			for( unsigned int i=0; i<shData->tsize; i++ ) {
				ret.shData->alldataref[i] = shData->alldata[i] / right.shData->alldata[i];
			}
			return ret;
		}
	};
	/*! operator +=
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	DoubleMatrix& operator+=( const DoubleMatrix& right ) {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			throw IncompatibleMatrices("Incompatibles Matrices in operator += (dimension must be equals)");
		}
#endif
		detach();
		for( unsigned int i=0; i<shData->tsize; i++ ) {
			shData->alldataref[i] += right.shData->alldata[i];
		}
		return (*this);
	};
	/*! operator -=
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	DoubleMatrix& operator-=( const DoubleMatrix& right ) {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			throw IncompatibleMatrices("Incompatibles Matrices in operator -= (dimension must be equals)");
		}
#endif
		detach();
		for( unsigned int i=0; i<shData->tsize; i++ ) {
			shData->alldataref[i] -= right.shData->alldata[i];
		}
		return (*this);
	};
	/*! operator %= (element-by-element multiplication)
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	DoubleMatrix& operator%=( const DoubleMatrix& right ) {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			throw IncompatibleMatrices("Incompatibles Matrices in operator %= (dimension must be equals)");
		}
#endif
		detach();
		for( unsigned int i=0; i<shData->tsize; i++ ) {
			shData->alldataref[i] *= right.shData->alldata[i];
		}
		return (*this);
	};
	/*! operator /= (element-by-element)
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleMatrix& operator/=( const DoubleMatrix& right ) {
#ifdef NNFW_DEBUG
		if ( shData->tsize != right.shData->tsize || shData->nrows != right.shData->nrows || shData->ncols != right.shData->ncols ) {
			throw IncompatibleMatrices("Incompatibles Matrices in operator /= (dimension must be equals)");
		}
#endif
		detach();
		for( unsigned int i=0; i<shData->tsize; i++ ) {
			shData->alldataref[i] /= right.shData->alldata[i];
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
		/*! All Data wrapped by doubleRef */
		doubleRef* alldataref;
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
	/*! if the matrix is internal means that it is not possible to change its structure:
	 *  - assignment operator= is not allowed
	 *  - resize is not allowed
	 */
	bool isinternal;
};

}

#endif

