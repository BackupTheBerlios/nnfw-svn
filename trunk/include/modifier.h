/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
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

#ifndef MODIFIER_H
#define MODIFIER_H

/*! \file
 */

#include "types.h"
#include "clonable.h"

namespace nnfw {

/*! \brief Modifier object
 *
 *  The modifier object is a the basic building blok for any kind of learning algorithm
 */
class NNFW_API Modifier : public Clonable {
public:
	/*! \name Constructors */
	//@{
	
    //! Constructor
    Modifier() { /* Nothing to do */ };
    //! Destructor
    virtual ~Modifier() { /* Nothing to do */ };

	//@}
	/*! \name Interface */
	//@{

    //! Modify the object
    virtual void modify() = 0;

	//! Clone
	virtual Modifier* clone() const = 0;

	//@}
};

/*! \brief MatrixModifier object
 *
 *  Abstract modifier class which links to a Realmat object (the matrix of network's parameters to be modified)
 */
class NNFW_API MatrixModifier : public Modifier {
public:
	/*! \name Constructors */
	//@{

    //! Constructor
	MatrixModifier( RealMat& matrix ) : m( matrix ) { /* Nothing to do */ };

    //! Destructor
    virtual ~MatrixModifier() { /* Nothing to do */ };

	//@}
	/*! \name Interface */
	//@{

    //! Modify the object
    virtual void modify() = 0;
    
	//! Get the matrix
	RealMat& matrix() const {
		return m;
	};
    
	//! Set the matrix
	void setMatrix( RealMat& matrix ) {
		m = matrix;	
	};

	//@}
	
protected:
    //! The matrix to be modified
    RealMat& m;
};

/*! \brief VectorModifier object
 *
 *  Abstract modifier class which links to a Realvec object (the vector of network's parameters to be modified)
 */
class NNFW_API VectorModifier : public Modifier {
public:
	/*! \name Constructors */
	//@{

    //! Constructor
	VectorModifier( RealVec& vector ) : v( vector ) { /* Nothing to do */ };
    
	//! Destructor
    virtual ~VectorModifier() { /* Nothing to do */ };
    
	//@}
	/*! \name Interface */
	//@{

	//! Modify the object
    virtual void modify() = 0;
    
	//! Get the vector
	RealVec& vector() const {
		return v;
	};
    
	//! Set the vector
	void setVector( RealVec& vector ) {
		v = vector;	
	};

	//@}
	
protected:
    //! The vector to be modified
    RealVec& v;
};

}

#endif

