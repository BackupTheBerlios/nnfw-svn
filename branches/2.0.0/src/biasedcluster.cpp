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

#include "biasedcluster.h"
#include "liboutputfunctions.h"
#include "randomgenerator.h"

namespace nnfw {

BiasedCluster::BiasedCluster( unsigned int numNeurons, QString name )
	: Cluster( numNeurons, name), biasesdata(numNeurons, true), tempdata(numNeurons) {
	biasesdata.zeroing();
}

BiasedCluster::~BiasedCluster() {
}

void BiasedCluster::update() {
	//--- in order to avoid to create a temporary vector at each call
	//--- it use the tempdata and the minus function of algebra.h
	function()->apply( minus( tempdata, inputs(), biases() ), outputs() );
	setNeedReset( true );
}

void BiasedCluster::setBias( unsigned int neuron, double bias ) {
#ifdef NNFW_DEBUG
	if ( neuron >= numNeurons() ) {
		qWarning() << "The neuron " << neuron << " doesn't exists! The operation setBias will be ignored";
		return;
	}
#endif
	biasesdata[neuron] = bias;
}

void BiasedCluster::setAllBiases( double bias ) {
	biases().setAll( bias );
}

void BiasedCluster::setBiases( const DoubleVector& bias ) {
	biases().copy( bias );
}

double BiasedCluster::getBias( unsigned int neuron ) {
#ifdef NNFW_DEBUG
	if ( neuron >= numNeurons() ) {
		qWarning() << "The neuron " << neuron << "doesn't exists! The operation getBias will return 0.0";
		return 0.0;
	}
#endif
	return biasesdata[neuron];
}

void BiasedCluster::randomize( double min, double max ) {
	for ( unsigned int i = 0; i < numNeurons(); i++ ) {
		biasesdata[i] = globalRNG->getDouble( min, max );
	}
}

BiasedCluster* BiasedCluster::clone() const {
	BiasedCluster* newclone = new BiasedCluster( numNeurons(), name() );
	newclone->setAccumulate( this->isAccumulate() );
	newclone->inputs().copy( this->inputs() );
	newclone->outputs().copy( this->outputs() );
	newclone->setBiases( this->biasesdata );
	newclone->setFunction( *(this->function()) );
	return newclone;
}

}
