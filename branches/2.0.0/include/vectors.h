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

#ifndef VECTORS_H
#define VECTORS_H

/*! \file
 *  \brief This file contains definitions of Vectors used for algebra
 *
 */

#include "memutils.h"
#include <exception>

namespace nnfw {

/*! \brief doubleRef Class
 *  \internal
 *  \par Motivation
 *  This allow to implement vector's element that appear fixed
 *  \par Description
 *  \par Warnings
 */
class NNFW_API doubleRef {
public:
	/*! constructor */
	doubleRef() : ref(&dummy), ref2(&dummy) {
	};
	/*! conversion utility */
	operator double() {
		return *ref;
	};
	/*! assign operator */
	doubleRef& operator=( const doubleRef& src ) {
		ref2 = src.ref2;
		return (*this);
	};
	/*! assign operator for double */
	doubleRef& operator=( const double& src ) {
		*ref2 = src;
		return (*this);
	};
	/*! operator+= for double */
	doubleRef& operator+=( const double& src ) {
		*ref2 += src;
		return (*this);
	};
	/*! operator-= for double */
	doubleRef& operator-=( const double& src ) {
		*ref2 -= src;
		return (*this);
	};
	/*! operator*= for double */
	doubleRef& operator*=( const double& src ) {
		*ref2 *= src;
		return (*this);
	};
	/*! operator/= for double */
	doubleRef& operator/=( const double& src ) {
		*ref2 /= src;
		return (*this);
	};
	/*! configure as not-fixed data */
	void setRef( double* src) {
		ref = src;
		ref2 = src;
	};
	/*! configure as not-fixed data */
	void setNoSteady() {
		ref2 = ref;
	};
	/*! configure as fixed data */
	void setSteady() {
		ref2 = &dummy;
	};
	/*! return true if is steady value */
	bool isSteady() {
		return ref != ref2;
	};
private:
	//--- How it works:
	//--- ref is always returned when the data is retrieved,
	//--- while ref2 is used during all assignments operation
	//--- FIXED data: ref2 points to dummy variable, in this way each assignment will change
	//---             the dummy variable and not the data pointed by ref
	//--- NOT FIXED : ref2 points to same variable of ref, in this way each assignement will change
	//---             the data pointed by ref (i.e. the source data)
	double* ref;
	double* ref2;
	static double dummy;
};

/*! \brief DoubleVector Class
 *  \par Motivation
 *  This represent a vector of double values
 *  \par Description
 *  The DoubleVector use an implicit-sharing mechanism based on copy-on-write
 *  \par Warnings
 *  This class is also used by other class for structuring their underlyng data. In these case, changing the structure of
 *  a DoubleVector using directly the assign operator= can result in uncoerent state of the object owning the DoubleVector.
 *  There special constructor that takes a boolean value, isinternal, for flag such situations.
 *  When a user attempt to call the operator= on a DoubleVector with isinternal flag on an Exception will be throw.
 */
class NNFW_API DoubleVector {
public:
	/*! \name Constructors */
	//@{
	/*! Default Constructor */
	DoubleVector() {
		shData = new sharedData();
		shData->vsize = 0;
		//shData->data = 0;
		//shData->dataref = 0;
		shData->refcounts = 1;
		shData->temporary = false;
		shData->nodata = false;
		this->isinternal = false;
	};
	/*! Construct a vector of dimension size setting all values to zero
	 *  \param isinternal if this parameter is true means that the DoubleVector is a internal part of another structure or object. In this case you are not allowed to change its structure but only the values
	 *  \warning If isinternal is true, then the operator= will raise an exception
	 */
	DoubleVector( unsigned int size, bool isinternal = false ) {
		shData = new sharedData();
		shData->vsize = size;
		shData->data = new double[size];
		memoryZeroing( shData->data, size );
		shData->dataref = new doubleRef[size];
		shData->refcounts = 1;
		shData->temporary = false;
		shData->nodata = false;
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i].setRef( shData->data + i );
		}
		this->isinternal = isinternal;
	};
	/*! Construct a vector of dimension size setting all the values as specified
	 *  \param isinternal if this parameter is true means that the DoubleVector is a internal part of another structure or object. In this case you are not allowed to change its structure but only the values
	 *  \warning If isinternal is true, then the operator= will raise an exception
	 */
	DoubleVector( unsigned int size, double value, bool isinternal = false ) {
		shData = new sharedData();
		shData->vsize = size;
		shData->data = new double[size];
		shData->dataref = new doubleRef[size];
		shData->refcounts = 1;
		shData->temporary = false;
		shData->nodata = false;
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->data[i] = value;
			shData->dataref[i].setRef( shData->data + i );
		}
		this->isinternal = isinternal;
	};
	/*! Construct by copying data from const double* vector
	 *  \param isinternal if this parameter is true means that the DoubleVector is a internal part of another structure or object. In this case you are not allowed to change its structure but only the values
	 *  \warning If isinternal is true, then the operator= will raise an exception
	 */
	DoubleVector( const double* r, unsigned int dim, bool isinternal = false ) {
		shData = new sharedData();
		shData->vsize = dim;
		shData->data = new double[dim];
		shData->dataref = new doubleRef[dim];
		shData->refcounts = 1;
		shData->temporary = false;
		shData->nodata = false;
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->data[i] = r[i];
			shData->dataref[i].setRef( shData->data + i );
		}
		this->isinternal = isinternal;
	};
	/*! The Copy-Constructor */
	DoubleVector( const DoubleVector& src ) {
		shData = src.shData;
		shData->refcounts += 1;
		//--- is not a temporary anymore !
		shData->temporary = false;
		isinternal = false;
	};
	/*! This is a special constructor used for copy a DoubleVector and mark it as temporary for speed-up
	 *  calculation.
	 *  It's intended for internal use only
	 *  DON'T USE UNLESS YOUR ARE VERY CONFIDENT on How the special temporary DoubleVector are used
	 *  \internal
	 *  The idea behind the temporary DoubleVector works in this way:
	 *  when an operator is called, and it has to create a new fresh copy of the data and return it, then
	 *  it will call the this constructor that will allocate new data for storing the calculations.
	 *  After that, the DoubleVector is returned by the operator and from now it is marked as temporary
	 *  If the DoubleVector will be passed to another operator (like in chained operations), then the operator
	 *  instead of creating the new data again, it will use the DoubleVector marked as temporary
	 *  At the end of operations it will be assigned and then the operator= unmark the DoubleVector as temporary
	 *  For example, suppose this code:
	 *  \code
	 *  DoubleVector v1 = v2 + v3*v4 - v5;
	 *  \endcode
	 *  When the first operator* is called, it will create a temporary DoubleVector and then all others operators will
	 *  write on it, reducing the allocation of memory from three to one !
	 *  In the example above, because the DoubleVector v1 has been created exactly when the operations have been done,
	 *  then v1 still marked as temporary, then successive operations like the example below will not allocate further
	 *  memory:
	 *  \code
	 *  DoubleVector v1 = v2 + v3*v4 - v5;
	 *  DoubleVector v6 = v1*v4 + v2;
	 *  \endcode
	 *  A DoubleVector will be unmarked as temporary when the copy-constructor and the operator= will be called.
	 *  \param dummy is not used, and exists only to avoid that the constructor has the same signature
	 *   of DoubleVector( unsigned int, bool internal )
	 */
	DoubleVector( unsigned int dim, bool temp, unsigned int dummy ) {
		dummy = 0; // just for avoid warning :-P
		shData = new sharedData();
		shData->vsize = dim;
		shData->data = new double[dim];
		shData->dataref = new doubleRef[dim];
		shData->refcounts = 1;
		shData->temporary = temp;
		shData->nodata = false;
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i].setRef( shData->data + i );
		}
		this->isinternal = false;
	};
	/*! Destructor */
	~DoubleVector() {
		shData->refcounts -= 1;
		if ( shData->refcounts == 0 ) {
			//--- the last destroy the data
			if ( shData->vsize > 0 ) {
				delete [](shData->dataref);
				if ( !(shData->nodata) ) {
					delete [](shData->data);
				}
			}
			delete shData;
		}
	};
	//@}
	/*! \name Exceptions throw by DoubleVector */
	//@{
	/*! Thrown when a user attempt to call the operator= on a DoubleVector with isinternal flag on */
	class NNFW_API AssignmentNotAllowed : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "It is not allowed to use the operator=() on DoubleVector used as internal data\n";
		};
	};
	/*! Thrown when a user attempt to resize a DoubleVector with isinternal flag on */
	class NNFW_API ResizeNotAllowed : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "It is not allowed to resize a DoubleVector used as internal data\n";
		};
	};
	/*! Thrown when a user attempt to do calculations with incompatible DoubleVectors */
	class NNFW_API IncompatibleVectors : public std::exception {
	public:
		IncompatibleVectors( const char* why ) {
			this->why = why;
		};
		virtual const char* what() const throw() {
			return why;
		};
	private:
		const char* why;
	};
	/*! Thrown when a user attempt to access outside boundary of a DoubleVector */
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
	/*! \name Informations about VectorData */
	//@{
	/*! Return the size of VectorData */
	unsigned int size() const {
		return shData->vsize;
	};
	/*! Return True if the DoubleVector doesn't contain any data (i.e.: size==0 ) */
	bool isNull() const {
		return (shData->vsize==0);
	};
	/*! Equal Operator */
	bool operator==( const DoubleVector& b ) const {
		if ( shData == b.shData ) return true;
		if ( shData->vsize != b.shData->vsize ) return false;
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			if ( shData->dataref[i] != b.shData->dataref[i] ) return false;
		}
		return true;
	};
	/*! Not-Equal Operator */
	bool operator!=( const DoubleVector& b ) const {
		return !( *this == b );
	};
	//@}
	/*! \name Operations on DoubleVector */
	//@{
	/*! Behaves as a CopyConstructor: this do a completely substitution of underlying data
	 *  so, it does not honor fixed elements, hence it substitute all
	 *  informations taking also the new information about fixed element in src.<br/>
	 *  If you want copy only the DoubleVector's values use copyValues method instead.
	 *  \warning If the isinternal flag is true, then the operator= will raise the AssignmentNotAllowed
	 */
	DoubleVector& operator=( const DoubleVector& src ) {
		if ( isinternal ) throw AssignmentNotAllowed();
		if ( src.shData == shData ) return (*this);
		//--- eliminate the previous data
		shData->refcounts -= 1;
		if ( shData->refcounts == 0 ) {
			//--- the last destroy the data
			if ( shData->vsize > 0 ) {
				delete [](shData->dataref);
				if ( !(shData->nodata) ) {
					delete [](shData->data);
				}
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
	 *  \note this method does not check the dimension of the two vectors, but behaves safely in this way:
	 *  - if src vector is bigger, then it copy all values from src without go beyond the size of this vector
	 *  - if src vector is smaller, then it copy all values from src and left unchanged all values that are 
	 *    beyond the limit of src vector
	 */
	DoubleVector& copyValues( const DoubleVector& src ) {
		// if shared data is the same, they already share the same values
		if ( shData == src.shData ) return (*this);
		detach();
		unsigned int max = qMin( shData->vsize, src.size() );
		for( unsigned int i=0; i<max; i++ ) {
			shData->dataref[i] = src[i];
		}
		return (*this);
	};
	/*! Copy the values from source honoring fixed elements starting from the offsets specified
	 *  (i.e.: the fixed element of this remains unchanged )
	 *  \param srcOffset is the offset from which the values are read from the src vector
	 *  \param thisOffset is the offset on which the values from the src vector are placed
	 *  \note this method does not check the dimension of the two vectors, but behaves safely
	 *        like the copyValues method
	 */
	DoubleVector& copyValues( const DoubleVector& src, unsigned int srcOffset, unsigned int thisOffset ) {
		// if shared data is the same and offsets are zero, then there is nothing to do
		if ( shData == src.shData && srcOffset == 0 && thisOffset == 0 ) return (*this);
		detach();
		unsigned int max = qMin( shData->vsize-thisOffset, src.size()-srcOffset );
		for( unsigned int i=0; i<max; i++ ) {
			shData->dataref[i+thisOffset] = src[i+srcOffset];
		}
		return (*this);
	};
	/*! Copy the values from source honoring fixed elements starting from the offsets specified
	 *  using the stride specified
	 *  (i.e.: the fixed element of this remains unchanged )
	 *  \param srcOffset is the offset from which the values are read from the src vector
	 *  \param thisOffset is the offset on which the values from the src vector are placed
	 *  \param stride is the stride; stride zero is not allowed, stride == 1 behaves like copyValues without stride,
	 *        stride > 1 it jump 'stride' values when coping, leaving the jumped values unchanged
	 *  \note this method does not check the dimension of the two vectors, but behaves safely
	 *        like the copyValues method
	 *  \warning if stride == 0, then it does not copy any values and return the vector unchanged
	 */
	DoubleVector& copyValues( const DoubleVector& src, unsigned int srcOffset, unsigned int thisOffset, unsigned int stride ) {
		if ( stride == 0 ) return (*this);
		// if shared data is the same and offsets are zero, then there is nothing to do
		if ( shData == src.shData && srcOffset == 0 && thisOffset == 0 ) return (*this);
		detach();
		unsigned int max = qMin( shData->vsize-thisOffset, src.size()-srcOffset );
		for( unsigned int i=0; i<max; i=i+stride ) {
			shData->dataref[i+thisOffset] = src[i+srcOffset];
		}
		return (*this);
	};
	/*! If the current data is shared by other objects, this method will create a new copy of the data
	 *  not shared by other objects.
	 */
	DoubleVector& detach() {
		if ( shData->refcounts > 1 ) {
			sharedData* tmp = new sharedData();
			unsigned int size = shData->vsize;
			tmp->vsize = size;
			tmp->data = new double[size];
			if ( shData->nodata ) {
				for( unsigned int i=0; i<size; i++ ) {
					tmp->data[i] = shData->dataref[i];
				}
			} else {
				memoryCopy( tmp->data, shData->data, size );
			}
			tmp->dataref = new doubleRef[size];
			tmp->refcounts = 1;
			tmp->temporary = false;
			tmp->nodata = false;
			for( unsigned int i=0; i<size; i++ ) {
				tmp->dataref[i].setRef( tmp->data + i );
			}
			shData->refcounts--;
			shData = tmp;
		}
		return (*this);
	};
	/*! unary operator + */
	const DoubleVector& operator+() const {
		return (*this);
	};
	/*! unary operator - */
	const DoubleVector operator-() const {
		if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i] = -shData->dataref[i];
			}
			return (*this);
		} else {
			DoubleVector ret( shData->vsize, true, 0 );
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				ret.shData->dataref[i] = -shData->dataref[i];
			}
			return ret;
		}
	};
	/*! operator +
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	const DoubleVector operator+( const DoubleVector& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			throw IncompatibleVectors("Incompatible DoubleVectors in operator+ (dimension must be equals)");
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				right.shData->dataref[i] = shData->dataref[i] + right.shData->dataref[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i] = shData->dataref[i] + right.shData->dataref[i];
			}
			return (*this);
		} else {
			DoubleVector ret( shData->vsize, true, 0 );
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				ret.shData->dataref[i] = shData->dataref[i] + right.shData->dataref[i];
			}
			return ret;
		}
	};
	/*! operator -
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	const DoubleVector operator-( const DoubleVector& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			throw IncompatibleVectors("Incompatible DoubleVectors in operator- (dimension must be equals)");
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				right.shData->dataref[i] = shData->dataref[i] - right.shData->dataref[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i] = shData->dataref[i] - right.shData->dataref[i];
			}
			return (*this);
		} else {
			DoubleVector ret( shData->vsize, true, 0 );
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				ret.shData->dataref[i] = shData->dataref[i] - right.shData->dataref[i];
			}
			return ret;
		}
	};
	/*! operator *
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	const DoubleVector operator*( const DoubleVector& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			throw IncompatibleVectors("Incompatible DoubleVectors in operator* (dimension must be equals)");
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				right.shData->dataref[i] = shData->dataref[i] * right.shData->dataref[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i] = shData->dataref[i] * right.shData->dataref[i];
			}
			return (*this);
		} else {
			DoubleVector ret( shData->vsize, true, 0 );
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				ret.shData->dataref[i] = shData->dataref[i] * right.shData->dataref[i];
			}
			return ret;
		}
	};
	/*! operator /
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	const DoubleVector operator/( const DoubleVector& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			throw IncompatibleVectors("Incompatible DoubleVectors in operator/ (dimension must be equals)");
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				right.shData->dataref[i] = shData->dataref[i] / right.shData->dataref[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i] = shData->dataref[i] / right.shData->dataref[i];
			}
			return (*this);
		} else {
			DoubleVector ret( shData->vsize, true, 0 );
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				ret.shData->dataref[i] = shData->dataref[i] / right.shData->dataref[i];
			}
			return ret;
		}
	};
	/*! operator +=
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	DoubleVector& operator+=( const DoubleVector& right ) {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			throw IncompatibleVectors("Incompatible DoubleVectors in operator+= (dimension must be equals)");
		}
#endif
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] + right.shData->dataref[i];
		}
		return (*this);
	};
	/*! operator -=
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	DoubleVector& operator-=( const DoubleVector& right ) {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			throw IncompatibleVectors("Incompatible DoubleVectors in operator-= (dimension must be equals)");
		}
#endif
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] - right.shData->dataref[i];
		}
		return (*this);
	};
	/*! operator *=
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	DoubleVector& operator*=( const DoubleVector& right ) {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			throw IncompatibleVectors("Incompatible DoubleVectors in operator*= (dimension must be equals)");
		}
#endif
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] * right.shData->dataref[i];
		}
		return (*this);
	};
	/*! operator /=
	 *  \warning Dimensionality check activated only when NNFW_DEBUG is defined
	 */
	DoubleVector& operator/=( const DoubleVector& right ) {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			throw IncompatibleVectors("Incompatible DoubleVectors in operator/= (dimension must be equals)");
		}
#endif
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] / right.shData->dataref[i];
		}
		return (*this);
	};
	/*! operator += with a scalar */
	DoubleVector& operator+=( const double& right ) {
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] + right;
		}
		return (*this);
	};
	/*! operator -= with a scalar */
	DoubleVector& operator-=( const double& right ) {
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] - right;
		}
		return (*this);
	};
	/*! operator *= with a scalar */
	DoubleVector& operator*=( const double& right ) {
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] * right;
		}
		return (*this);
	};
	/*! operator /= with a scalar */
	DoubleVector& operator/=( const double& right ) {
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] / right;
		}
		return (*this);
	};
	/*! Fix the i-th value, hence the i-th value will never change anymore
	 *  until unsteady is called
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	DoubleVector& steady( unsigned int i ) {
#ifdef NNFW_DEBUG
		if( i >= shData->vsize ) {
			throw OutsideBoundaries("Fixating elements outside boundary");
		}
#endif
		detach();
		shData->dataref[i].setSteady();
		return (*this);
	};
	/*! Reallow the modification of i-th value
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	DoubleVector& unsteady( unsigned int i ) {
#ifdef NNFW_DEBUG
		if( i >= shData->vsize ) {
			throw OutsideBoundaries("Un-Fixating elements outside boundary");
		}
#endif
		detach();
		shData->dataref[i].setNoSteady();
		return (*this);
	};
	/*! Return true if the i-th value is a steady value
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	bool isSteady( unsigned int i ) const {
#ifdef NNFW_DEBUG
		if( i >= shData->vsize ) {
			throw OutsideBoundaries("isSteady - Accessing element outside boundary");
		}
#endif
		return shData->dataref[i].isSteady();
	};
	/*! Set all values to zero
	 *  This method honor the steady values... hence that values will remain at the same values
	 */
	DoubleVector& zeroing() {
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = 0;
		}
		return (*this);
	};
	/*! Set all values to value
	 *  This method honor the steady values... hence that values will remain at the same values
	 */
	DoubleVector& setAll( const double value ) {
		detach();
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = value;
		}
		return (*this);
	};
	/*! Indexing operator
	 *  \warning Boundary check activated only when NNFW_DEBUG is defined
	 */
	doubleRef& operator[]( unsigned int index ) {
#ifdef NNFW_DEBUG
		if( index >= shData->vsize ) {
			throw OutsideBoundaries("operator[] - Accessing elements outside boundary");
		}
#endif
		detach();
		return shData->dataref[index];
	};
	/*! Indexing operator (Const Version)
	 *  \warning Boundary check activated only when NNFW_DEBUG if defined
	 */
	double operator[]( unsigned int index ) const {
#ifdef NNFW_DEBUG
		if( index >= shData->vsize ) {
			throw OutsideBoundaries("operator[] - Accessing elements outside boundary");
		}
#endif
		return shData->dataref[index];
	};
	/*! Resize the VectorData */
	DoubleVector& resize( unsigned int newsize ) {
		if ( shData->nodata || isinternal ) {
			throw ResizeNotAllowed();
		}
		detach();
		if ( newsize > shData->vsize ) {
			double* newdata = new double[newsize];
			memoryCopy( newdata, shData->data, shData->vsize );
			delete [](shData->data);
			delete [](shData->dataref);
			shData->data = newdata;
			shData->vsize = newsize;
			shData->dataref = new doubleRef[ newsize ];
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i].setRef( shData->data + i );
			}
		} else {
			shData->vsize = newsize;
		}
		return (*this);
	};
	/*! Append an element; the dimesion increase by one */
	DoubleVector& append( const double& value ) {
		//--- detach (and throw) done by resize
		resize( shData->vsize+1 );
		shData->data[ shData->vsize-1 ] = value;
		return (*this);
	};
	/*! Append Operator; the dimesion increase by one */
	DoubleVector& operator<<( const double& value ) {
		//--- detach (and throw) done by resize (append)
		append( value );
		return (*this);
	};
	//@}

private:
	// putting this NNFW_INTERNAL GCC comply with a warning
	class sharedData {
	public:
		/*! The actual size of VectorData */
		unsigned int vsize;
		/*! Data */
		double* data;
		/*! references count */
		int refcounts;
		/*! Data wrapped by doubleRef */
		doubleRef* dataref;
		/*! if is temporary (used to avoid multiple copying of data during math expressions) */
		bool temporary;
		/*! if true the data field is not valid */
		bool nodata;
	};
	/*! shared data among DoubleVector istances */
	sharedData* shData;
	/*! if the vector is internal means that it is not possible to change its structure:
	 *  - assignment operator= is not allowed
	 *  - resize is not allowed
	 */
	bool isinternal;

	friend class DoubleMatrix;
	/*! Private method used by Matrix for setting DoubleVector for row-view and col-view. <br/>
	 *  This method resize only the dataRef array ignoring the data and set-up flag
	 *  nodata to true in order to avoid accessing to non-existing data <br/>
	 *  dataRef will be setted pointing to data passed
	 */
	void resizeNoData( unsigned int newsize ) {
		if ( shData->vsize > 0 ) {
			if ( shData->data ) { delete [](shData->data); }
			delete [](shData->dataref);
		}
		shData->vsize = newsize;
		shData->dataref = new doubleRef[ newsize ];
		shData->nodata = true;
		isinternal = true;
	};
};

}

#endif
