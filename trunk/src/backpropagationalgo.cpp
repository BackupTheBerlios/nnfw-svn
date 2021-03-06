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

#include "neuralnet.h"
#include "matrixlinker.h"
#include "biasedcluster.h"
#include "derivableoutputfunction.h"
#include "backpropagationalgo.h"
#include "nnfwfactory.h"

using namespace std;

namespace nnfw {

BackPropagationAlgo::BackPropagationAlgo( BaseNeuralNet *n_n, UpdatableVec up_order, Real l_r )
	: LearningAlgorithm(n_n), learn_rate(l_r), update_order(up_order) {

	Cluster *cluster_temp;
	// pushing the info for output cluster
	const ClusterVec& outs = n_n->outputClusters();
	for( int i=0; i<(int)outs.size(); i++ ) {
		addCluster( outs[i], true );
	}
	// --- generate information for backpropagation of deltas
	for( int i=0; i<(int)update_order.size(); i++ ) {
		cluster_temp = dynamic_cast<Cluster*>(update_order[i]);
		if ( cluster_temp ) {
			addCluster( cluster_temp, false );
			continue;
		}
		MatrixLinker* linker_temp = dynamic_cast<MatrixLinker*>(update_order[i]);
		if ( linker_temp ) {	//Dot linker subclass of Matrixlinker
			addLinker( linker_temp );
		}
	}
	useMomentum = false;
	momentumv = 0.0f;
}

BackPropagationAlgo::~BackPropagationAlgo( ) {
	/* nothing to do ?!?! */
}

void BackPropagationAlgo::setTeachingInput( Cluster* output, const RealVec& ti ) {
	if ( mapIndex.count( output ) == 0 ) { 
		return;
	}
	int index = mapIndex[ output ];
	cluster_deltas_vec[index].deltas_outputs.assign_xminusy( output->outputs(), ti );
	return;
}

const RealVec& BackPropagationAlgo::getError( Cluster* cl ) {
	if ( mapIndex.count( cl ) == 0 ) {
		nWarning() << "Cluster not present in BackPropagationAlgo";
		return RealVec();
	}
		
	int index = mapIndex[ cl ];
	return cluster_deltas_vec[index].deltas_outputs;
}

void BackPropagationAlgo::enableMomentum() {
	for ( u_int i=0; i<cluster_deltas_vec.size(); ++i ) {
		for ( u_int j=0;  j<cluster_deltas_vec[i].incoming_linkers_vec.size(); ++j ) {
			// --- zeroing data
			cluster_deltas_vec[i].incoming_last_outputs[j].zeroing();
			cluster_deltas_vec[i].last_deltas_inputs.zeroing();
		}
	}
	useMomentum = true;
}

void BackPropagationAlgo::propagDeltas() {
	RealVec diff_vec;
	for( int i=0; i<(int)cluster_deltas_vec.size(); i++ ) {
		cluster_deltas_vec[i].incoming_linkers_vec;
		// --- propagate DeltaOutput to DeltaInputs
		diff_vec.resize( cluster_deltas_vec[i].deltas_inputs.size() );
		const DerivableOutputFunction* diff_output_function = dynamic_cast<const DerivableOutputFunction*>( cluster_deltas_vec[i].cluster->getFunction( ) );
		if ( diff_output_function == 0 ) {
#ifdef NNFW_DEBUG
			nWarning() << "No derivative for the activation function is defined!" ;
#endif
			cluster_deltas_vec[i].deltas_inputs.assign( cluster_deltas_vec[i].deltas_outputs );
		} else {
			Cluster* cl = cluster_deltas_vec[i].cluster;
			diff_output_function->derivate( cl->inputs(), cl->outputs(), diff_vec );
			cluster_deltas_vec[i].deltas_inputs.zeroing();
			cluster_deltas_vec[i].deltas_inputs.deltarule( 1.0, cluster_deltas_vec[i].deltas_outputs, diff_vec );
		}
		// --- propagate DeltaInputs to DeltaOutput through MatrixLinker
		for( u_int k=0; k<cluster_deltas_vec[i].incoming_linkers_vec.size( ); ++k ) {
			MatrixLinker* link = dynamic_cast<MatrixLinker*>(cluster_deltas_vec[i].incoming_linkers_vec[k]);
			if ( mapIndex.count(link->from()) == 0 ) {
				// --- the from() cluster is not in Learning
				continue;
			}
			int from_index = mapIndex[ link->from() ];
			RealMat::mul( cluster_deltas_vec[from_index].deltas_outputs, link->matrix(), cluster_deltas_vec[i].deltas_inputs );
		}
	}
	return;
}

void BackPropagationAlgo::learn() {
    zeroingDeltas();
	// --- propagating the error through the net
	propagDeltas();
    applyDeltas();
	return;
}

void BackPropagationAlgo::zeroingDeltas() {
	// --- zeroing previous step delta information
	for ( u_int i=0; i<cluster_deltas_vec.size(); ++i ) {
		if ( cluster_deltas_vec[i].isOutput ) continue;
		cluster_deltas_vec[i].deltas_outputs.zeroing();
	}
}

void BackPropagationAlgo::applyDeltas() {
	// --- make the learn !!
	for ( u_int i=0; i<cluster_deltas_vec.size(); ++i ) {
		RealVec minus_ones( cluster_deltas_vec[i].cluster->outputs().size( ), -1.0f );
		cluster_deltas_vec[i].modcluster->rule( -learn_rate, minus_ones, cluster_deltas_vec[i].deltas_inputs );

		for ( u_int j=0;  j<cluster_deltas_vec[i].incoming_linkers_vec.size(); ++j ) {
			cluster_deltas_vec[i].incoming_modlinkers[j]->rule(
				-learn_rate,
				cluster_deltas_vec[i].incoming_linkers_vec[j]->from()->outputs(),
				cluster_deltas_vec[i].deltas_inputs
			);
			if ( !useMomentum ) continue;
			// --- add the momentum
			cluster_deltas_vec[i].incoming_modlinkers[j]->rule(
				-learn_rate*momentumv,
				cluster_deltas_vec[i].incoming_last_outputs[j],
				cluster_deltas_vec[i].last_deltas_inputs
			);
			// --- save datas for momentum on the next step
			cluster_deltas_vec[i].incoming_last_outputs[j].assign( cluster_deltas_vec[i].incoming_linkers_vec[j]->from()->outputs() );
			cluster_deltas_vec[i].last_deltas_inputs.assign( cluster_deltas_vec[i].deltas_inputs );
		}
	}
}

void BackPropagationAlgo::learn( const Pattern& pat ) {
	// --- set the inputs of the net
	const ClusterVec& clins = net()->inputClusters();
	for( u_int i=0; i<clins.size(); i++ ) {
		clins[i]->inputs().assign( pat.inputsOf( clins[i] ) );
	}
	// --- spread the net
	net()->step();
	// --- set the teaching input
	const ClusterVec& clout = net()->outputClusters();
	for( u_int i=0; i<clout.size(); i++ ) {
		setTeachingInput( clout[i], pat.outputsOf( clout[i] ) );
	}
	learn();
}

Real BackPropagationAlgo::calculateMSE( const Pattern& pat ) {
	// --- set the inputs of the net
	const ClusterVec& clins = net()->inputClusters();
	for( u_int i=0; i<clins.size(); i++ ) {
		clins[i]->inputs().assign( pat.inputsOf( clins[i] ) );
	}
	// --- spread the net
	net()->step();
	// --- calculate the MSE
	const ClusterVec& clout = net()->outputClusters();
	Real mseacc = 0.0;
	int dim = (int)clout.size();
	for( int i=0; i<dim; i++ ) {
		mseacc += RealVec::mse( clout[i]->outputs(), pat.outputsOf( clout[i] ) );
	}
	return mseacc/dim;
}

void BackPropagationAlgo::addCluster( Cluster* cl, bool isOut ) {
	if( mapIndex.count( cl ) == 0 ) {
		cluster_deltas temp;
		int size = cl->numNeurons();
		temp.cluster = cl;
		temp.modcluster = Factory::createModifierFor( temp.cluster );
		temp.isOutput = isOut;
		temp.deltas_outputs.resize( size );
		temp.deltas_inputs.resize( size );
		temp.last_deltas_inputs.resize( size );
		cluster_deltas_vec.push_back( temp );
		mapIndex[cl] = cluster_deltas_vec.size()-1;
	}
}

void BackPropagationAlgo::addLinker( Linker* link ) {
	if ( mapIndex.count( link->to() ) == 0 ) {
		cluster_deltas temp;
		int size = link->to()->numNeurons();
		temp.cluster = link->to();
		temp.modcluster = Factory::createModifierFor( temp.cluster );
		temp.isOutput = false;
		temp.deltas_outputs.resize( size );
		temp.deltas_inputs.resize( size );
		temp.last_deltas_inputs.resize( size );
		temp.incoming_linkers_vec.push_back( link );
		temp.incoming_modlinkers.push_back( Factory::createModifierFor( link ) );
		temp.incoming_last_outputs.push_back( RealVec( link->from()->numNeurons() ) );
		cluster_deltas_vec.push_back( temp );
		mapIndex[temp.cluster] = cluster_deltas_vec.size()-1;
	}
	else {
		int tmp = mapIndex[link->to()];
		cluster_deltas_vec[ tmp ].incoming_linkers_vec.push_back( link );
		cluster_deltas_vec[ tmp ].incoming_modlinkers.push_back( Factory::createModifierFor( link ) );
		cluster_deltas_vec[ tmp ].incoming_last_outputs.push_back( RealVec( link->from()->numNeurons() ) );
	}
}

}
