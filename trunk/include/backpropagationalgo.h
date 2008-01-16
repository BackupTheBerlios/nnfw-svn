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

	/*! Starts a single training step. */
	void learn( );

	/*! Return the error calculated */
	const RealVec& getError( Cluster* );
	//@}

private:
	//! The Real learning rate factor
	Real learn_rate;
	//! The update order
	UpdatableVec update_order;

	//! The struct of Clusters and Deltas
	class cluster_deltas {
	public:
		Cluster* cluster;
		AbstractModifier* modcluster;
		bool isOutput;
		RealVec deltas_outputs;
		RealVec deltas_inputs;
		LinkerVec incoming_linkers_vec;
		VectorData<AbstractModifier*> incoming_modlinkers;
	};
	//! map to help looking for cluster_deltas info
	std::map<Cluster*, int> mapIndex;
	//! The VectorData of struct of Clusters and Deltas
	VectorData<cluster_deltas> cluster_deltas_vec;
	// --- propagate delta through the net
	void propagDeltas();

};

}

#endif

