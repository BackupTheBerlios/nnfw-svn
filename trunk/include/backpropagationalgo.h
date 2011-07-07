/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                *
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

#ifndef BACKPROPAGATIONALGO_H
#define BACKPROPAGATIONALGO_H

/*! \file
 */

#include "types.h"
#include "learningalgorithm.h"
#include <map>

namespace nnfw {

class AbstractModifier;

/*! \brief Back-Propagation Algorithm implementation
 *
 */
class NNFW_API BackPropagationAlgo : public LearningAlgorithm {
public:
	/*! \name Constructors */
	//@{

	/*! Constructor
	 *
	 *  \param neural_network the BaseNeuralNet neural network to train
	 *  \param update_order the UpdatableVec for the backpropagation sequence
	 *  \param learn_rate the Real learning rate factor
	 */
	BackPropagationAlgo( BaseNeuralNet *n_n, UpdatableVec update_order, Real l_r = 0.1f );

	//! Destructor
	~BackPropagationAlgo( );

	//@}
	/*! \name Interface */
	//@{

	/*! Set the teaching input for Cluster passed
	 *  \param teach_input the RealVec teaching input
	 */
	void setTeachingInput( Cluster* output, const RealVec& ti );
	
	/*! Don't modify data about that Cluster */
	void dontLearn( Cluster* cluster );
	/*! Don't modify weight about that Linker */
	void dontLearn( Linker* linker );

	virtual void learn();

	/*! Starts a single training step. */
	virtual void learn( const Pattern& );

	/*! Calculate the Mean Square Error respect to Pattern passed */
	virtual Real calculateMSE( const Pattern& );

	/*! Set the learning rate */
	void setRate( Real newrate ) {
		learn_rate = newrate;
	};

	/*! return the learning rate */
	Real rate() {
		return learn_rate;
	};

	/*! Set the momentum value */
	void setMomentum( Real newmom ) {
		momentumv = newmom;
	};

	/*! return the momentum */
	Real momentum() {
		return momentumv;
	};

	/*! Enable the momentum */
	void enableMomentum();

	/*! Disable momentum */
	void disableMomentum() {
		useMomentum = false;
	};

	/*! This method returns the deltas calculated by the Back-propagation Algorithm.
	 *  These deltas are set every time new targets are defined for the output layer(s),
	 *  which are then used to update network weights when the method learn() is called.<br>
	 *  They are also useful to calculate the network performance, but for that it must be used outside
	 *  the <em>learning cycle</em> (a full learning iteration, that corresponds to present the network
	 *  with all the patterns of the train data set). For that you must call
	 *  getError( Cluster * anyOutputCluster ) for each line of your training set (you'll get a RealVec
	 *  with the deltas for each unit of the cluster considered).<br>
	 *  Then you can use those values to calculate your desired performance measure.<br>
	 *  For instance: if you use it to calculate the Mean Square Error (MSE) of the network
	 *  for your train data set you must accumulate the square of the the getError( anyOutputCluster ) 
	 *  output for each line of the set, and at the end divide it by the length of that same set
	 *  (by definition the MSE is the sum of the squared differences between the target and actual
	 *  output of a sequence of values). Getting the Root Mean Squared Error (RMSE) from this is
	 *  trivial (you just need to calculate the square root of the MSE).
	 *
	 *  \warning The data returned by getError( Cluster * ) is computed every time you set a new output target,
	 *  which means every time you call the setTeachingInput( Cluster * anyOutputCluster, const RealVec &
	 *  teaching_input ) method. If your network has more than one output layer you have to call
	 *  setTeachingInput() for all the output clusters before calling getError() for any of the clusters.
	 */
	const RealVec& getError( Cluster* );
	//@}

	void zeroingDeltas();
	void applyDeltas();
	// --- propagate delta through the net
	void propagDeltas();

private:
	//! The Real learning rate factor
	Real learn_rate;
	//! Momentum
	Real momentumv;
	//! bool switch for enable/disable momentum
	Real useMomentum;
	//! The update order
	UpdatableVec update_order;
	//! Flags for Cluster
	std::map<Cluster*, bool> learnableClusters;
	std::map<Linker*, bool> learnableLinkers;

	//! The struct of Clusters and Deltas
	class NNFW_API cluster_deltas {
	public:
		Cluster* cluster;
		AbstractModifier* modcluster;
		bool isOutput;
		RealVec deltas_outputs;
		RealVec deltas_inputs;
		RealVec last_deltas_inputs;
		LinkerVec incoming_linkers_vec;
		VectorData<AbstractModifier*> incoming_modlinkers;
		VectorData<RealVec> incoming_last_outputs;
	};
	//! map to help looking for cluster_deltas info
	std::map<Cluster*, int> mapIndex;
	//! The VectorData of struct of Clusters and Deltas
	VectorData<cluster_deltas> cluster_deltas_vec;
	// --- add a Cluster into the structures above
	void addCluster( Cluster*, bool );
	// --- add a Linker into the structures above
	void addLinker( Linker* );

};

}

#endif

