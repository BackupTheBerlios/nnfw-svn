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

#include "biasedcluster.h"
#include "liboutputfunctions.h"
#include "randomgenerator.h"

namespace nnfw {

BiasedCluster::BiasedCluster( unsigned int numNeurons, QString name )
	: Cluster( numNeurons, name), biasesdata(numNeurons, true), tempdata(numNeurons) {
	biasesdata.zeroing();
}

BiasedCluster::BiasedCluster( ConfigurationParameters& params, QString prefix )
	: Cluster( params, prefix ), biasesdata(numNeurons(), true), tempdata(numNeurons()) {
	// biases is a vector, that is a list of space-separated values
	QString str = params.getValue(prefix + "biases");
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		unsigned int cycleLength = numNeurons();
		if (list.size() != (int)numNeurons()) {
#ifdef NNFW_DEBUG
			qWarning() << "The number of elements of the biases vector in configuration file (" << list.size()
			           << ") is different from the number of neurons (" << numNeurons() << ").";
#endif
			cycleLength = qMin(list.size(), (int)numNeurons());
		}
		for( unsigned int i=0; i<cycleLength; i++) {
			bool ok;
			biasesdata[i] = list[i].toDouble(&ok);
			if (!ok) {
				biasesdata[i] = 0.0;
			}
		}
	}
}

BiasedCluster::~BiasedCluster() {
}

void BiasedCluster::update() {
	//--- in order to avoid to create a temporary vector at each call
	//--- it use the tempdata and the subtract function of algebra.h
	outFunction()->apply( subtract( tempdata, inputs(), biases() ), outputs() );
	setNeedReset( true );
}

void BiasedCluster::setBias( unsigned int neuron, double bias ) {
	biasesdata[neuron] = bias;
}

void BiasedCluster::setAllBiases( double bias ) {
	biasesdata.setAll( bias );
}

void BiasedCluster::setBiases( const DoubleVector& bias ) {
	biasesdata.copyValues( bias );
}

double BiasedCluster::getBias( unsigned int neuron ) {
	return biasesdata[neuron];
}

void BiasedCluster::randomize( double min, double max ) {
	for ( unsigned int i = 0; i < numNeurons(); i++ ) {
		biasesdata[i] = globalRNG->getDouble( min, max );
	}
}

void BiasedCluster::save(ConfigurationParameters& params, QString prefix)
{
	Cluster::save( params, prefix );
	params.startObjectParameters(prefix, "BiasedCluster", this);
	// First creating a string list, then transforming to a single string
	QStringList list;
	for (unsigned int i = 0; i < biasesdata.size(); i++) {
		list.push_back(QString::number(biasesdata[i]));
	}
	params.createParameter(prefix, "biases", list.join(" "));
}

}
