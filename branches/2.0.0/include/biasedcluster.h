/******************************************************************************
 *  Neural Network Framework.                                                 *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>              *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the Free Software               *
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA *
 *****************************************************************************/

#ifndef BIASEDCLUSTER_H
#define BIASEDCLUSTER_H

/*! \file
 *  \brief This file contains the declaration of BiasedCluster class
 *
 *  Details...
 */

#include "types.h"
#include "cluster.h"
#include "outputfunction.h"

namespace nnfw {

/*! \brief In a BiasedCluster each neuron has an input, an output and a bias value.
 *
 *  \par Motivation
 *  \par Description
 *   All neurons in the Cluster have the same transfer function which
 *   is given to the constructor by the outfunction property (see API doc).
 *  \par Warnings
 *
 */
class NNFW_API BiasedCluster : public Cluster {
public:
	/*! \name Constructors */
	//@{
	/*! Construct a Cluster that contains numNeurons neurons */
	BiasedCluster( unsigned int numNeurons, QString name = "unnamed" );
	/*! Destructor */
	virtual ~BiasedCluster();
	//@}
	/*! \name Interface */
	//@{
	/*! Update the outputs of neurons */
	void update();
	/*! Set the bias of the neuron */
	void setBias( unsigned int neuron, double bias );
	/*! Set all biases with the same value */
	void setAllBiases( double bias );
	/*! Set the biases from the vector given */
	void setBiases( const DoubleVector& biases );
	/*! Get bias of the neuron */
	double getBias( unsigned int neuron );
	/*! Get the array of biases, this returns the actual array not a copy.
	 *  This allows you to change the biases using the pointer returned!
	 */
	DoubleVector biases() {
		return biasesdata;
	};
	/*! Randomize the biases of BiasedCluster */
	void randomize( double min, double max );
	/*! Clone this BiasedCluster */
	virtual BiasedCluster* clone() const;
	/*! Return its typename */
	virtual QString typeName() {
		return "BiasedCluster";
	};
	//@}

private:
	DoubleVector biasesdata;
	DoubleVector tempdata;
};

}

#endif
