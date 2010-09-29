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

#include "cluster.h"
#include "liboutputfunctions.h"
#include <algorithms>

namespace nnfw {

Cluster::Cluster( unsigned int numNeurons, QString name )
	: Updatable(name), inputdata(numNeurons, true), outputdata(numNeurons, true) {
	this->numneurons = numNeurons;
	outputdata.zeroing();
	inputdata.zeroing();
	accOff = true;
	setNeedReset( false );
	//! SigmoidFunction as Default
	setFunction(new SigmoidFunction( 1.0 ));
}

Cluster::Cluster( ConfigurationParameters& params, QString prefix ) :
	Updatable(params, prefix),
	inputdata(1, true), // The number of neurons will be changed during configuration
	outputdata(1, true) // The number of neurons will be changed during configuration
{
	numneurons = 1;
	QString str = params.getValue(prefix + "numNeurons");
	if (!str.isNull()) {
		bool ok;
		numneurons = str.toUInt(&ok);
		if (!ok) {
			numneurons = 1;
		}
	}
	// Resizing inputdata and outputdata
	inputdata.resize(numneurons);
	outputdata.resize(numneurons);

	accOff = true;
	str = params.getValue(prefix + "accumulate").toLower();
	// If accumulate is true, we set accOff to false
	if ((str == "t") || (str == "true") || (str == "1")) {
		accOff = false;
	}

	// Delta is a vector, that is a list of space-separated values
	str = params.getValue(prefix + "inputs");
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		unsigned int cycleLength = numneurons;
		if (list.size() != numneurons) {
#ifdef NNFW_DEBUG
			qWarning() << "The number of elements of the inputs vector in configuration file (" << list.size()
			           << ") is different from the number of neurons (" << numneurons << ").";
#endif
			cycleLength = std::min(list.size(), numneurons);
		}

		for (unsigned int i = 0; i < cycleLength; i++) {
			bool ok;
			inputdata[i] = list[i].toDouble(&ok);
			if (!ok) {
				inputdata[i] = 0.0;
			}
		}
	}

	// Delta is a vector, that is a list of space-separated values
	str = params.getValue(prefix + "outputs");
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		unsigned int cycleLength = numneurons;
		if (list.size() != numneurons) {
#ifdef NNFW_DEBUG
			qWarning() << "The number of elements of the output vector in configuration file (" << list.size()
			           << ") is different from the number of neurons (" << numneurons << ").";
#endif
			cycleLength = std::min(list.size(), numneurons);
		}

		for (unsigned int i = 0; i < cycleLength; i++) {
			bool ok;
			outputdata[i] = list[i].toDouble(&ok);
			if (!ok) {
				outputdata[i] = 0.0;
			}
		}
	}

	// The output function needs to be already configured when we get here
	setFunction(params.getObjectFromParameter(prefix + "outfunction", false, true));
}

Cluster::~Cluster() {
	// No need to delete anything, we use auto_ptr
}

void Cluster::setFunction( OutputFunction *up ) {
	updater.reset(up);
	updater->setCluster( this );
}

void Cluster::setInput( unsigned int neuron, double value ) {
#ifdef NNFW_DEBUG
	if ( neuron >= numneurons ) {
		qWarning() << "The neuron " << neuron << " doesn't exists! The operation setInput will be ignored";
		return;
	}
#endif
	inputdata[neuron] = value;
}

void Cluster::setInputs( const DoubleVector& inputs ) {
	inputdata.copy( inputs );
}

void Cluster::setAllInputs( double value ) {
	inputdata.setAll( value );
	setNeedReset( false );
}

void Cluster::resetInputs() {
	inputdata.zeroing();
	setNeedReset( false );
}

double Cluster::getInput( unsigned int neuron ) const {
#ifdef NNFW_DEBUG
	if ( neuron >= numneurons ) {
		qWarning() << "The neuron " << neuron << " doesn't exists! The operation getInput will return 0.0";
		return 0.0;
	}
#endif
	return inputdata[neuron];
}

void Cluster::setOutput( unsigned int neuron, double value ) {
#ifdef NNFW_DEBUG
	if ( neuron >= numneurons ) {
		qWarning() << "The neuron " << neuron << " doesn't exists! The operation setOutput will be ignored";
		return;
	}
#endif
	outputdata[neuron] = value;
}

void Cluster::setOutputs( const DoubleVector& outputs ) {
	outputdata.copy( outputs );
}

double Cluster::getOutput( unsigned int neuron ) const {
#ifdef NNFW_DEBUG
	if ( neuron >= numneurons ) {
		qWarning() << "The neuron " << neuron << " doesn't exists! The operation getOutput will return 0.0";
		return 0.0;
	}
#endif
	return outputdata[neuron];
}

void Cluster::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "Cluster", this);

	params.createParameter(prefix, "numNeurons", QString::number(numneurons));

	params.createParameter(prefix, "accumulate", (accumulate ? "False" : "True"));

	// First creating a string list, then transforming to a single string
	QStringList list;
	for (unsigned int i = 0; i < inputdata.size(); i++) {
		list.push_back(QString::number(inputdata[i]));
	}
	params.createParameter(prefix, "inputs", list.join(" "));

	// Doing the same with outputdata
	list.clear();
	for (unsigned int i = 0; i < outputdata.size(); i++) {
		list.push_back(QString::number(outputdata[i]));
	}
	params.createParameter(prefix, "outputs", list.join(" "));

	// The output function needs to be already configured when we get here
	params.createParameter(prefix, "outfunction", updater.get()));
}

}
