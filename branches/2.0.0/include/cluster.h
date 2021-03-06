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

#ifndef CLUSTER_H
#define CLUSTER_H

/*! \file
 *  \brief This file contains the declarations of the Cluster class
 */

#include "types.h"
#include "updatable.h"
#include "outputfunction.h"
#include <memory>

namespace nnfw {

/*! \brief Define the common interface among Clusters
 *
 *  \par Motivation
 *    The Cluster class define the common interface amog Cluster. The subclasses may extends this interface
 *    for specific purpose (ex. SimpleCluster), but the BaseNeuralNet, the Linker and other classes depends
 *    only by this class. This abstraction allow to isolate the specific implementation of various classes
 *  \par Description
 *    The Cluster class represent an abstract group of neurons. There isn't a neuron class. The Cluster
 *    class represent a group of neurons as two arrays: inputs and outputs. The inputs array represent the
 *    inputs of the neurons 'contained' in the cluster, and the outputs of this neurons are calculated by
 *    appling the function provided by OutputFunction.<br>
 *    The number of neuron returned by size() method is also the dimension of inputs and outputs arrays<br>
 *    You can sets one subclasses of OutputFunction by setUpdater methods. If you don't specify an index when
 *    set a OutputFunction then this OutputFunction will be used to update the output of all neurons. Otherwise,
 *    you can specifiy different OutputFunction for different neuron.
 *    \code
 * // create a SimpleCluster, a specialized subclass of Cluster
 * SimpleCluster* simple = new SimpleCluster( 10 ); // this cluster contains 10 neurons
 * // set the SigmoidUpdater for all neurons
 * simple->setUpdater( new SigmoidUpdater( 1.0 ) );
 *    \endcode
 *  \par Warnings
 *    <b>For whose want to implement a subclass of Cluster: </b>
 *    This class allocate the memory for inputs, outputs and updaters array. So, a subclass have to implements only
 *    the update method.
 *    The getInputs and getOutputs returns a valid array of internal data, and not simply a copy
 *    of the internal data. Look at the following code:
 *    \code
 * RealVec& in = cluster->inputs();
 * in[2] = 3.0;   // This statement will be changes the inputs of third neuron.
 * // the above statement is equivalent with the following
 * cluster->setInput( 2, 3.0 );
 *    \endcode
 *    The reasons behind this kind of behaviour its the efficiency!! When another class must do heavy calculation
 *    on all inputs of a Cluster (as MatrixLinker do), then its more efficient that it takes the array returned
 *    by inputs (or outputs) and works over them.
 * \nosubgrouping
 *
 */
class NNFW_API Cluster : public Updatable {
public:
	/*! \name Constructors */
	//@{
	/*! Construct a Cluster */
	Cluster( unsigned int numNeurons, QString name = "unnamed" );
	/*! Constructor */
	Cluster( ConfigurationParameters& params, QString prefix );
	/*! Destructor */
	virtual ~Cluster();
	//@}
	/*! \name Methods affecting whole Cluster */
	//@{
	/*! Return the number of neurons (the length of input and output arrays) */
	unsigned int numNeurons() const {
		return numneurons;
	};
	/*! Return true if inputs needs a reset */
	bool needReset() {
		return needRst;
	};
	/*! Enable/Disable accumulation mode<br>
	 *  If accumulation is enabled (true) then linkers attached to this Cluster will never resetInput and accumulates data,
	 *  otherwise the inputs will be resetted at each step of neural network
	*/
	void setAccumulate( bool mode ) {
		accOff = !mode;
	};
	/*! return true if the Cluster will accumulates inputs */
	bool isAccumulate() const {
		return !accOff;
	};
	/*! Randomize the parameters of the Cluster<br>
	 *  The parameters randomized by this method will be specified by sub-classes
	 */
	virtual void randomize( double min, double max ) = 0;
	//@}
	/*! \name Operations on Input's vector */
	//@{
	/*! Set the input of neuron
	 * Details...
	 */
	void setInput( unsigned int neuron, double value );
	/*! Set the inputs from the vector given */
	void setInputs( const DoubleVector& inputs );
	/*! Set all the inputs with the same value
	 * Details...
	 */
	void setAllInputs( double value );
	/*! Reset the inputs of this cluster; the inputs will be set to zero.
	 * Details...
	 */
	void resetInputs();
	/*! Get the input of neuron
	 */
	double getInput( unsigned int neuron ) const;
	/*! Get the array of inputs */
	DoubleVector& inputs() {
		return *inputdataptr;
	};
	/*! Get the array of inputs */
	DoubleVector inputs() const {
		return *inputdataptr;
	};
	//@}
	/*! \name Operations on Output's vector */
	//@{
	/*! Force the output of the neuron at value specified */
	void setOutput( unsigned int neuron, double value );
	/*! Set the outputs from the vector given */
	void setOutputs( const DoubleVector& outputs );
	/*! Get the output of neuron */
	double getOutput( unsigned int neuron ) const;
	/*! Get the array of outputs */
	DoubleVector& outputs() {
		return *outputdataptr;
	};
	/*! Get the array of outputs */
	DoubleVector outputs() const {
		return *outputdataptr;
	};
	//@}
	/*! \name Operations on OutputFunction */
	//@{
	/*! Set the output function for all neurons contained<br>
	 *  This method create an internal copy of the OutputFunction passed <br>
	 *  \warning This function delete the previous updater class registered !!! <br>
	 */
	void setOutFunction( OutputFunction* up );
	/*! Get the Output function */
	OutputFunction* outFunction() const {
		return updater.get();
	};
	//@}
	/*! \name Saving functions */
	//@{
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * This saves the name property, remember to call this in child classes
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}

protected:
	/*! Set the state of 'needReset'<br>
	 *  Used by subclasses into update implementation
	 */
	void setNeedReset( bool b ) {
		needRst = accOff && b;
	};
	/*! Pointer to data Input */
	DoubleVector* inputdataptr;
	/*! Pointer to data Output */
	DoubleVector* outputdataptr;
private:
	/*! Number of neurons */
	unsigned int numneurons;
	/*! Input of neurons */
	DoubleVector inputdata;
	/*! Output of neurons */
	DoubleVector outputdata;
	/*! OutputFunction Object */
	std::auto_ptr<OutputFunction> updater;
	/*! True if the inputs needs a reset */
	bool needRst;
	/*! In Accumulated mode the needRst is always false, and then linkers attached to this will never resetInputs
	 *  --- Warns for developers --- accOff == true means NO-ACCUMULATION
	 */
	bool accOff;
};

}

#endif
