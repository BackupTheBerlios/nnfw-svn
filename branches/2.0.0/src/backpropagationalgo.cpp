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

#include "neuralnet.h"
#include "matrixlinker.h"
#include "biasedcluster.h"
#include "backpropagationalgo.h"

namespace nnfw {

BackPropagationAlgo::BackPropagationAlgo( NeuralNet *n_n, UpdatableList up_order, double l_r )
	: LearningAlgorithm(n_n), learn_rate(l_r), update_order(up_order) {
	useMomentum = false;
	momentumv = 0.0f;
	neuralNetChanged();
}

BackPropagationAlgo::BackPropagationAlgo()
	: LearningAlgorithm(), learn_rate(0.0) {
	useMomentum = false;
	momentumv = 0.0f;
	neuralNetChanged();
}

BackPropagationAlgo::~BackPropagationAlgo( ) {
	/* nothing to do ?!?! */
}

void BackPropagationAlgo::neuralNetChanged() {
	NeuralNet* net = neuralNet();
	if ( !net ) return;
	//--- clear all data
	mapIndex.clear();
	cluster_deltas_vec.clear();
	//--- insert new data for the new NeuralNet
	Cluster *cluster_temp;
	// pushing the info for output cluster
	ClusterList outs = net->outputClusters();
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
}

void BackPropagationAlgo::setUpdateOrder( const UpdatableList& update_order ) {
	this->update_order = update_order;
	this->neuralNetChanged();
}

void BackPropagationAlgo::setTeachingInput( Cluster* output, const DoubleVector& ti ) {
	if ( mapIndex.count( output ) == 0 ) { 
		return;
	}
	int index = mapIndex[ output ];
	subtract( cluster_deltas_vec[index].deltas_outputs, output->outputs(), ti );
	return;
}

DoubleVector BackPropagationAlgo::getError( Cluster* cl ) {
	if ( mapIndex.count( cl ) == 0 ) {
		qWarning() << "Cluster not present in BackPropagationAlgo";
		return DoubleVector();
	}
	int index = mapIndex[ cl ];
	return cluster_deltas_vec[index].deltas_outputs;
}

void BackPropagationAlgo::enableMomentum() {
	for ( int i=0; i<cluster_deltas_vec.size(); ++i ) {
		for ( int j=0;  j<cluster_deltas_vec[i].incoming_linkers_vec.size(); ++j ) {
			// --- zeroing data
			cluster_deltas_vec[i].incoming_last_outputs[j].zeroing();
			cluster_deltas_vec[i].last_deltas_inputs.zeroing();
		}
	}
	useMomentum = true;
}

void BackPropagationAlgo::propagDeltas() {
	DoubleVector diff_vec;
	for( int i=0; i<(int)cluster_deltas_vec.size(); i++ ) {
		cluster_deltas_vec[i].incoming_linkers_vec;
		// --- propagate DeltaOutput to DeltaInputs
		cluster_deltas_vec[i].deltas_inputs.copyValues( cluster_deltas_vec[i].deltas_outputs );
		Cluster* cl = cluster_deltas_vec[i].cluster;
		OutputFunction* func = cl->outFunction();
		diff_vec.resize( cluster_deltas_vec[i].deltas_inputs.size() );
		if ( func->derivate( cl->inputs(), cl->outputs(), diff_vec ) ) {
			cluster_deltas_vec[i].deltas_inputs *= diff_vec;
		}
		// --- propagate DeltaInputs to DeltaOutput through MatrixLinker
		for( int k=0; k<cluster_deltas_vec[i].incoming_linkers_vec.size( ); ++k ) {
			MatrixLinker* link = dynamic_cast<MatrixLinker*>(cluster_deltas_vec[i].incoming_linkers_vec[k]);
			if ( mapIndex.count(link->from()) == 0 ) {
				// --- the from() cluster is not in Learning
				continue;
			}
			int from_index = mapIndex[ link->from() ];
			amul( cluster_deltas_vec[from_index].deltas_outputs, link->matrix(), cluster_deltas_vec[i].deltas_inputs );
		}
	}
	return;
}

void BackPropagationAlgo::learn() {
	// --- zeroing previous step delta information
	for ( int i=0; i<cluster_deltas_vec.size(); ++i ) {
		if ( cluster_deltas_vec[i].isOutput ) continue;
		cluster_deltas_vec[i].deltas_outputs.zeroing();
	}
	// --- propagating the error through the net
	propagDeltas();
	// --- make the learn !!
	for ( int i=0; i<cluster_deltas_vec.size(); ++i ) {
		if ( cluster_deltas_vec[i].cluster != NULL) {
			DoubleVector minus_ones( cluster_deltas_vec[i].cluster->outputs().size( ), -1.0f );
			deltarule( cluster_deltas_vec[i].cluster->biases(), -learn_rate, minus_ones, cluster_deltas_vec[i].deltas_inputs );
		}

		for ( int j=0;  j<cluster_deltas_vec[i].incoming_linkers_vec.size(); ++j ) {
			if ( cluster_deltas_vec[i].incoming_linkers_vec[j] != NULL ) {
				deltarule(
					cluster_deltas_vec[i].incoming_linkers_vec[j]->matrix(),
					-learn_rate,
					cluster_deltas_vec[i].incoming_linkers_vec[j]->from()->outputs(),
					cluster_deltas_vec[i].deltas_inputs
				);
				if ( !useMomentum ) continue;
				// --- add the momentum
				deltarule(
					cluster_deltas_vec[i].incoming_linkers_vec[j]->matrix(),
					-learn_rate*momentumv,
					cluster_deltas_vec[i].incoming_last_outputs[j],
					cluster_deltas_vec[i].last_deltas_inputs
				);
				// --- save datas for momentum on the next step
				cluster_deltas_vec[i].incoming_last_outputs[j].copyValues( cluster_deltas_vec[i].incoming_linkers_vec[j]->from()->outputs() );
				cluster_deltas_vec[i].last_deltas_inputs.copyValues( cluster_deltas_vec[i].deltas_inputs );
			}
		}
	}
	return;
}

void BackPropagationAlgo::learn( const Pattern& pat ) {
	// --- set the inputs of the net
	ClusterList clins = neuralNet()->inputClusters();
	for( int i=0; i<clins.size(); i++ ) {
		clins[i]->inputs().copyValues( pat.inputsOf( clins[i] ) );
	}
	// --- spread the net
	neuralNet()->step();
	// --- set the teaching input
	ClusterList clout = neuralNet()->outputClusters();
	for( int i=0; i<clout.size(); i++ ) {
		setTeachingInput( clout[i], pat.outputsOf( clout[i] ) );
	}
	learn();
}

double BackPropagationAlgo::calculateMSE( const Pattern& pat ) {
	// --- set the inputs of the net
	ClusterList clins = neuralNet()->inputClusters();
	for( int i=0; i<clins.size(); i++ ) {
		clins[i]->inputs().copyValues( pat.inputsOf( clins[i] ) );
	}
	// --- spread the net
	neuralNet()->step();
	// --- calculate the MSE
	ClusterList clout = neuralNet()->outputClusters();
	double mseacc = 0.0;
	int dim = (int)clout.size();
	for( int i=0; i<dim; i++ ) {
		mseacc += mse( clout[i]->outputs(), pat.outputsOf( clout[i] ) );
	}
	return mseacc/dim;
}

void BackPropagationAlgo::addCluster( Cluster* cl, bool isOut ) {
	if( mapIndex.count( cl ) == 0 ) {
		cluster_deltas temp;
		int size = cl->numNeurons();
		temp.cluster = dynamic_cast<BiasedCluster*>(cl);
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
		temp.cluster = dynamic_cast<BiasedCluster*>(link->to());
		temp.isOutput = false;
		temp.deltas_outputs.resize( size );
		temp.deltas_inputs.resize( size );
		temp.last_deltas_inputs.resize( size );
		temp.incoming_linkers_vec.push_back( dynamic_cast<MatrixLinker*>(link) );
		temp.incoming_last_outputs.push_back( DoubleVector( link->from()->numNeurons() ) );
		cluster_deltas_vec.push_back( temp );
		mapIndex[temp.cluster] = cluster_deltas_vec.size()-1;
	}
	else {
		int tmp = mapIndex[link->to()];
		cluster_deltas_vec[ tmp ].incoming_linkers_vec.push_back( dynamic_cast<MatrixLinker*>(link) );
		cluster_deltas_vec[ tmp ].incoming_last_outputs.push_back( DoubleVector( link->from()->numNeurons() ) );
	}
}

void BackPropagationAlgo::configure(ConfigurationParameters& params, QString prefix) {
	params.startRememberingGroupObjectAssociations();
	learn_rate = params.getValue( prefix + "rate" ).toDouble();
	momentumv = params.getValue( prefix + "momentum" ).toDouble();
	if ( momentumv == 0.0 ) {
		useMomentum = false;
	} else {
		useMomentum = true;
	}
	QString str = params.getValue( prefix + "order" );
	update_order.clear();
	if ( !str.isNull() ) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		foreach( QString upl, list ) {
			Updatable* up = params.getObjectFromGroup<Updatable>( upl, true );
			update_order << up;
		}
	}
	NeuralNet* net = params.getObjectFromParameter<NeuralNet>( prefix+"neuralnet", false, true );
	setNeuralNet( net );
	params.stopRememberingGroupObjectAssociations();
}

void BackPropagationAlgo::save(ConfigurationParameters& params, QString prefix) {
	params.startObjectParameters(prefix, "BackPropagationAlgo", this);
	params.createParameter( prefix, "neuralnet", neuralNet() );
	params.createParameter( prefix, "rate", QString::number(learn_rate) );
	if ( useMomentum ) {
		params.createParameter( prefix, "momentum", QString::number(momentumv) );
	}
	QStringList list;
	foreach( Updatable* up, update_order ) {
		list << up->name();
	}
	params.createParameter( prefix, "order", list.join(" ") );
	//--- save the neuralnet in the group corresponding to its name
	neuralNet()->save( params, neuralNet()->name() );
}

}
