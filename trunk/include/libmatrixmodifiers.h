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

#ifndef LIBMATRIXMODIFIER_H
#define LIBMATRIXMODIFIER_H

#include "types.h"

/*! \file
 *  \brief Library of common matrix modifiers
 *
 *  Details...
 *
 *  \todo See whether most of MatrixModifiers subclasses can just extend HebbMatrixModifier
 *	      This is the reason why I left its modify() method virtual!
 *		  In which case: should we change this class name?
 */

#include "modifier.h"
#include "types.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Basic Matrix Modifier object
 *
 *  This basic modifier modify the matrix (weights) acording to the simple (hebbian) rule:
 *  deltaW = lr * input * output
 *  It requires a vector of inputs, and a vector of outputs
 *  I called it 'basic' because it can be used also for other non-hebbian algorithms (such as the delta rule or even in back-propagation)
 *  Indeed, most of matrix modifiers will inherit from this one
 */
	class  BasicMatrixModifier : public MatrixModifier {
public:
    //! Constructor
    BasicMatrixModifier( RealMat& matrix, RealVec& input, RealVec& output, Real lRate = 0.1 ) 
		: MatrixModifier( matrix ), inp( input ), out( output ), lr( lRate ) { 
			/* Here we need to check that input and output vector dimensions correspond to those of the matrix! */ 
		};

    //! Destructor
    ~BasicMatrixModifier() { /* Nothing to do */ };

    //! Modify the object according to the simple Hebb rule
    virtual void modify();

    //! Get the input vector
	RealVec& getInputVector(){
		return inp;
	};

    //! Set the input vector
	void setInputVector( RealVec& vector ) {
		//dimensions check needed
		inp = vector;	
	};

    //! Get the output vector
	RealVec& getOutputVector(){
		return out;
	};

    //! Set the output vector
	void setOutputVector( RealVec& vector ) {
		//dimensions check needed
		out = vector;	
	};

    //! Get the learning rate vector
	Real getLearningRate(){
		return lr;
	};

    //! Set the learning rate vector
	void setLearningRate( Real lRate ) {
		lr = lRate;	
	};
	
protected:
    //! The vector of input
	RealVec& inp;
    //! The vector of output
	RealVec& out;
    //! The learning rate
	Real lr;
};


/*! \brief Sparse Matrix Modifier object
 *
 *  This SparseMatrixModifier modify inherit from the base modifier but must be used if you want to modify a sparse matrix acording to the simple (hebbian) rule:
 *  deltaW = lr * input * output
 *  It requires the matrix to be modified, a vector of inputs, a vector of outputs, and the mask of modifiable elements (existing links)
 */
	class  SparseMatrixModifier : public BasicMatrixModifier {
public:
    //! Constructor
    SparseMatrixModifier( RealMat& matrix, RealVec& input, RealVec& output, MatrixData<bool>& otherMask, Real lRate = 0.1 ) 
		: BasicMatrixModifier( matrix, input, output, lRate ), mask( otherMask ) { 
			/* Here we need to check that input and output vector dimensions correspond to those of the matrix! */ 
		};

    //! Destructor
    ~SparseMatrixModifier() { /* Nothing to do */ };

    //! Modify only the existing connections according to the simple Hebb rule
    virtual void modify();

private:
	//! The mask for existing connections
	MatrixData<bool>& mask;
};


/*! \brief Sparse Matrix Modifier object
 *
 *  This SparseMatrixModifier modify inherit from the base modifier but must be used if you want to modify a sparse matrix acording to the simple (hebbian) rule:
 *  deltaW = lr * input * output
 *  It requires the matrix to be modified, a vector of inputs, a vector of outputs, and the mask of modifiable elements (existing links)
 */
	class  SparseCovarianceMatrixModifier : public BasicMatrixModifier {
public:
    //! Constructor
    SparseCovarianceMatrixModifier( RealMat& matrix, RealVec& input, RealVec& output, MatrixData<bool>& otherMask, RealVec& meanInp, RealVec &meanOut, Real lRate = 0.1 ) 
		: BasicMatrixModifier( matrix, input, output, lRate ), mask( otherMask ), mInp( meanInp ), mOut( meanOut ) { 
			/* Here we need to check that input and output vector dimensions correspond to those of the matrix! */ 
		};

    //! Destructor
    ~SparseCovarianceMatrixModifier() { /* Nothing to do */ };

	//! Get the meanInput vector
	RealVec& getMeanInputVector(){
		return mInp;
	};

    //! Set the meanInput vector
	void setMeanInputVector( RealVec& vector ) {
		//dimensions check needed
		mInp = vector;	
	};

    //! Get the meanOutput vector
	RealVec& getMeanOutputVector(){
		return mOut;
	};

    //! Set the meanOutput vector
	void setMeanOutputVector( RealVec& vector ) {
		//dimensions check needed
		mOut = vector;	
	};

    //! Modify only the existing connections according to the simple Hebb rule
    virtual void modify();

private:
	//! The mask for existing connections
	MatrixData<bool>& mask;
	//! The mean input activation vector
	RealVec& mInp;
	//! The mean output activation vector
	RealVec& mOut;
};


}

#endif
