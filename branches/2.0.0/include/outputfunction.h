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

#ifndef OUTPUTFUNCTION_H
#define OUTPUTFUNCTION_H

/*! \file
 *  \brief This file contains the declaration of the abstract OutputFunction Class
 *
 */

#include "types.h"
#include "clonable.h"

namespace nnfw {

/*! \brief OutputFunction Class
 *
 *  Details...
 */
class NNFW_API OutputFunction : public Clonable {
public:
	/*! \name Constructors */
	//@{
	/*! Constructor */
	OutputFunction() : tmp1(1), tmp2(1) { /*nothing to do*/ };
	/*! Destructor */
	virtual ~OutputFunction() { /*nothing to do*/ };
	//@}
	/*! \name Interface */
	//@{
	/*! Calculate the outputs of neurons by the net inputs given */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs ) = 0;
	/*! Calculate the outputs of a single neuron */
	double apply( double input ) {
		tmp1[0] = input;
		apply( tmp1, tmp2 );
		return tmp2[0];
	};
	/*! Compute the derivate of the function represent<br>
	 *  Given the input of neurons and the corresponding output of neurons, it calculate
	 *  the derivate.
	 *  \return if it return false, then the OutputFunction is not derivable, and the derivates
	 *  DoubleVector can contains trash data.
	 */
	virtual bool derivate( const DoubleVector& inputs, const DoubleVector& outputs, DoubleVector& derivates ) const {
		Q_UNUSED( inputs );
		Q_UNUSED( outputs );
		Q_UNUSED( derivates );
		return false;
	};
	/*! Set the Cluster which it is inserted<br>
	 *  This method it's not necessary for simple OutputFunction like SigmoidFunction, LinearFunction, etc...
	 *  but can be very helpfull for some particular and advanced function that requires to access the data
	 *  of the Cluster in which it is inserted into. (like PoolFunction)<br>
	 *  If you don't have to access to the Cluster data ignore it, but if you needs then reimplement for
	 *  know the the function is inserted into a Cluster.
	 */
	virtual void setCluster( Cluster* ) { /* nothing to do */ };
	/*! Clone method returns a new allocated clone of this object */
	// This is here just to tell the compiler that cloning a cluster returns
	// an output function and not a Clonable (as the base class function
	// declaration states)
	virtual OutputFunction* clone() const = 0;
	//@}
private:
	/*! temporary RealVec for speed-up apply with a single value */
	DoubleVector tmp1;
	/*! temporary RealVec for speed-up apply with a single value */
	DoubleVector tmp2;
};

}

#endif
