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

#ifndef OUTPUTFUNCTION_H
#define OUTPUTFUNCTION_H

/*! \file
 *  \brief This file contains the declaration of the abstract OutputFunction Class
 *
 */

#include "types.h"
#include <factory/parametersettable.h>
#include <configuration/configurationparameters.h>
#include <QRegExp>

namespace nnfw {

/*! \brief OutputFunction Class
 *
 *  Details...
 */
class NNFW_API OutputFunction : public ParameterSettableWithConfigureFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Constructor */
	OutputFunction() : clusterv(NULL), tmp1(1), tmp2(1) { /*nothing to do*/ };
	/*! Destructor */
	virtual ~OutputFunction() { /*nothing to do*/ };
	//@}
	/*! \name Exceptions throw by OutputFunction */
	//@{
	/*! Thrown when a user attempt to call setCluster on an OutputFunction already inserted into a Cluster*/
	class NNFW_API SetClusterException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "setCluster called on an OutputFunction already configured to be part of another Cluster!\n";
		};
	};
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
	 *  This is automatically called, and in normal case you don't have to call it directly
	 */
	void setCluster( Cluster* cl ) {
		if ( cl != NULL ) throw SetClusterException();
		clusterv = cl;
		clusterSetted();
	};
	//@}
protected:
	/*! The method setCluster will call clusterSetted to inform subclasses that the OutputFunction
	 *  has been inserted into a Cluster.
	 */
	virtual void clusterSetted() { /* nothing to do */ };
	/*! Cluster on which the OutputFunction is inserted */
	Cluster* clusterv;
private:
	/*! temporary RealVec for speed-up apply with a single value */
	DoubleVector tmp1;
	/*! temporary RealVec for speed-up apply with a single value */
	DoubleVector tmp2;
};

}

#endif
