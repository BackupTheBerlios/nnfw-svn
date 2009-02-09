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

#ifndef VECTORS_H
#define VECTORS_H

/*! \file
 *  \brief This file contains definitions of Vectors used for algebra
 *
 */

#include "memutils.h"

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
 *  \par Description
 *  \par Warnings
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
		this->isprotected = false;
	};
	/*! Construct a vector of dimension size setting all values to zero
	 *  \param isprotected if the parameter is true, then the operator= is disable (not allowed);
	 */
	DoubleVector( unsigned int size, bool isprotected = false ) {
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
		this->isprotected = isprotected;
	};
	/*! Construct a vector of dimension size setting all the values as specified */
	DoubleVector( unsigned int size, double value, bool isprotected = false ) {
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
		this->isprotected = isprotected;
	};
	/*! Construct by copying data from const T* vector */
	DoubleVector( const double* r, unsigned int dim, bool isprotected = false ) {
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
		this->isprotected = isprotected;
	};
	/*! The Copy-Constructor */
	DoubleVector( const DoubleVector& src ) {
		shData = src.shData;
		shData->refcounts += 1;
		//--- is not a temporary anymore !
		shData->temporary = false;
		isprotected = false;
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
	bool operator==( const DoubleVector& b ) {
		if ( shData == b.shData ) return true;
		if ( shData->vsize != b.shData->vsize ) return false;
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			if ( shData->dataref[i] != b.shData->dataref[i] ) return false;
		}
		return true;
	};
	/*! Not-Equal Operator */
	bool operator!=( const DoubleVector& b ) {
		return !( *this == b );
	};
	//@}
	/*! \name Operations on DoubleVector */
	//@{
	/*! Behaves as a CopyConstructor: this do a completely substitution of underlying data
	 *  so, it does not honor fixed elements, hence it substitute all
	 *  informations taking also the new information about fixed element in src.<br/>
	 *  If you want only copying DoubleVector's values use copy method
	 *  \warning when constructed with isprotected to true, then this method is not allowed
	 */
	DoubleVector& operator=( const DoubleVector& src ) {
		if ( isprotected ) return (*this);
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
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleVector& copy( const DoubleVector& src ) {
#ifdef NNFW_DEBUG
		if ( shData->vsize != src.shData->vsize ) {
			qCritical() << "Incompatible DoubleVector in copy method (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = src[i];
		}
		return (*this);
	};
	/*! It behaves exactly as copy method and not as operator= */
	DoubleVector& assignData( const DoubleVector& src ) {
		return copy( src );
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
	/*! operator +
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleVector operator+( const DoubleVector& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			qCritical() << "Incompatible DoubleVector in operator + (dimension must be equals)";
			return (*this);
		}
#endif		
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				((DoubleVector&)right)[i] = shData->dataref[i] + right[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i] = shData->dataref[i] + right[i];
			}
			return (*this);
		} else {
			DoubleVector ret( shData->vsize );
			ret.shData->temporary = true;
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				ret[i] = shData->dataref[i] + right[i];
			}
			return ret;
		}
	};
	/*! operator -
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleVector operator-( const DoubleVector& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			qCritical() << "Incompatible DoubleVector in operator - (dimension must be equals)";
			return (*this);
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				((DoubleVector&)right)[i] = shData->dataref[i] - right[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i] = shData->dataref[i] - right[i];
			}
			return (*this);
		} else {
			DoubleVector ret( shData->vsize );
			ret.shData->temporary = true;
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				ret[i] = shData->dataref[i] - right[i];
			}
			return ret;
		}
	};
	/*! operator *
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleVector operator*( const DoubleVector& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			qCritical() << "Incompatible DoubleVector in operator * (dimension must be equals)";
			return (*this);
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				((DoubleVector&)right)[i] = shData->dataref[i] * right[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i] = shData->dataref[i] * right[i];
			}
			return (*this);
		} else {
			DoubleVector ret( shData->vsize );
			ret.shData->temporary = true;
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				ret[i] = shData->dataref[i] * right[i];
			}
			return ret;
		}
	};
	/*! operator /
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleVector operator/( const DoubleVector& right ) const {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			qCritical() << "Incompatible DoubleVector in operator / (dimension must be equals)";
			return (*this);
		}
#endif
		if ( right.shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				((DoubleVector&)right)[i] = shData->dataref[i] / right[i];
			}
			return right;
		} else if ( shData->temporary ) {
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				shData->dataref[i] = shData->dataref[i] / right[i];
			}
			return (*this);
		} else {
			DoubleVector ret( shData->vsize );
			ret.shData->temporary = true;
			for( unsigned int i=0; i<shData->vsize; i++ ) {
				ret[i] = shData->dataref[i] / right[i];
			}
			return ret;
		}
	};
	/*! operator +=
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleVector& operator+=( const DoubleVector& right ) {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			qCritical() << "Incompatible DoubleVector in operator += (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] + right[i];
		}
		return (*this);
	};
	/*! operator -=
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleVector& operator-=( const DoubleVector& right ) {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			qCritical() << "Incompatible DoubleVector in operator -= (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] - right[i];
		}
		return (*this);
	};
	/*! operator *=
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleVector& operator*=( const DoubleVector& right ) {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			qCritical() << "Incompatible DoubleVector in operator *= (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] * right[i];
		}
		return (*this);
	};
	/*! operator /=
	 *  \warning Dimensionality check activated only when DEBUG is defined
	 */
	DoubleVector& operator/=( const DoubleVector& right ) {
#ifdef NNFW_DEBUG
		if ( shData->vsize != right.shData->vsize ) {
			qCritical() << "Incompatible DoubleVector in operator+ (dimension must be equals)";
			return (*this);
		}
#endif
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] / right[i];
		}
		return (*this);
	};
	/*! operator += with a scalar */
	DoubleVector& operator+=( const double& right ) {
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] + right;
		}
		return (*this);
	};
	/*! operator -= with a scalar */
	DoubleVector& operator-=( const double& right ) {
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] - right;
		}
		return (*this);
	};
	/*! operator *= with a scalar */
	DoubleVector& operator*=( const double& right ) {
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] * right;
		}
		return (*this);
	};
	/*! operator /= with a scalar */
	DoubleVector& operator/=( const double& right ) {
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = shData->dataref[i] / right;
		}
		return (*this);
	};
	/*! Fix the i-th value, hence the i-th value will never change anymore
	 *  until unsteady is called
	 *  \warning Boundary check activated only when DEBUG if defined
	 */
	DoubleVector& steady( unsigned int i ) {
#ifdef NNFW_DEBUG
		if( i >= shData->vsize ) {
			qCritical() << "Fixating elements outside boundary" ;
			return (*this);
		}
#endif
		shData->dataref[i].setSteady();
		return (*this);
	};
	/*! Reallow the modification of i-th value
	 *  \warning Boundary check activated only when DEBUG if defined
	 */
	DoubleVector& unsteady( unsigned int i ) {
#ifdef NNFW_DEBUG
		if( i >= shData->vsize ) {
			qCritical() << "Un-Fixating elements outside boundary" ;
			return (*this);
		}
#endif
		shData->dataref[i].setNoSteady();
		return (*this);
	};
	/*! Return true if the i-th value is a steady value */
	bool isSteady( unsigned int i ) {
#ifdef NNFW_DEBUG
		if( i >= shData->vsize ) {
			qCritical() << "Accessing elements outside boundary" ;
			return false;
		}
#endif
		return shData->dataref[i].isSteady();
	};
	/*! Set all values to zero
	 *  This method honor the steady values... hence that values will remain at the same values
	 */
	DoubleVector& zeroing() {
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = 0;
		}
		return (*this);
	};
	/*! Set all values to value
	 *  This method honor the steady values... hence that values will remain at the same values
	 */
	DoubleVector& setAll( const double value ) {
		for( unsigned int i=0; i<shData->vsize; i++ ) {
			shData->dataref[i] = value;
		}
		return (*this);
	};
	/*! Indexing operator
	 *  \warning Boundary check activated only when DEBUG is defined
	 */
	doubleRef& operator[]( unsigned int index ) {
#ifdef NNFW_DEBUG
		if( index >= shData->vsize ) {
			qCritical() << "Accessing elements outside boundary" ;
			return shData->dataref[0];
		}
#endif
		return shData->dataref[index];
	};
	/*! Indexing operator (Const Version)
	 *  \warning Boundary check activated only when DEBUG if defined
	 */
	double operator[]( unsigned int index ) const {
#ifdef NNFW_DEBUG
		if( index >= shData->vsize ) {
			qCritical() << "Accessing elements outside boundary" ;
			return shData->dataref[0];
		}
#endif
		return shData->dataref[index];
	};
	/*! Resize the VectorData */
	DoubleVector& resize( unsigned int newsize ) {
		if ( shData->nodata ) {
			qFatal("You cannot resize DoubleVector returned by row or column views of Matrices");
		}
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
		resize( shData->vsize+1 );
		shData->data[ shData->vsize-1 ] = value;
		return (*this);
	};
	/*! Append Operator; the dimesion increase by one */
	DoubleVector& operator<<( const double& value ) {
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
	/*! if the vector is protected means that assignment operator= is not allowed */
	bool isprotected;

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
		isprotected = true;
	};

#ifdef C_NNFW_API
	//--- for accessing from C interface implementation
	friend double* getRawData( DoubleVector& );
#endif
};

}

#endif
