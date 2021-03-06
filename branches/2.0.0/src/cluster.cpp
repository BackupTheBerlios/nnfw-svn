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

#include "cluster.h"
#include "liboutputfunctions.h"
//#include <algorithms>
#include <exception>

namespace nnfw {

Cluster::Cluster( unsigned int numNeurons, QString name )
	: Updatable(name), inputdata(numNeurons,true), outputdata(numNeurons,true) {
	this->numneurons = numNeurons;
	inputdataptr = &inputdata;
	inputdata.zeroing();
	outputdataptr = &outputdata;
	outputdata.zeroing();
	accOff = true;
	setNeedReset( false );
	// SigmoidFunction as Default
	setOutFunction(new SigmoidFunction( 1.0 ));
}

Cluster::Cluster( ConfigurationParameters& params, QString prefix ) :
	Updatable(params, prefix)
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
	// Creating inputdata and outputdata
	inputdata = DoubleVector(numneurons, true );
	inputdataptr = &inputdata;
	inputdata.zeroing();
	outputdata = DoubleVector(numneurons, true );
	outputdataptr = &outputdata;
	outputdata.zeroing();

	accOff = true;
	str = params.getValue(prefix + "accumulate").toLower();
	// If accumulate is true, we set accOff to false
	if ((str == "t") || (str == "true") || (str == "1")) {
		accOff = false;
	}

	// inputs is a vector, that is a list of space-separated values
	str = params.getValue(prefix + "inputs");
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		unsigned int cycleLength = numneurons;
		if (list.size() != (int)numneurons) {
#ifdef NNFW_DEBUG
			qWarning() << "The number of elements of the inputs vector in configuration file (" << list.size()
			           << ") is different from the number of neurons (" << numneurons << ").";
#endif
			cycleLength = std::min(list.size(), (int)numneurons);
		}
		for( unsigned int i=0; i<cycleLength; i++) {
			bool ok;
			(*inputdataptr)[i] = list[i].toDouble(&ok);
			if (!ok) {
				(*inputdataptr)[i] = 0.0;
			}
		}
	}

	// outputs is a vector, that is a list of space-separated values
	str = params.getValue(prefix + "outputs");
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		unsigned int cycleLength = numneurons;
		if (list.size() != (int)numneurons) {
#ifdef NNFW_DEBUG
			qWarning() << "The number of elements of the output vector in configuration file (" << list.size()
			           << ") is different from the number of neurons (" << numneurons << ").";
#endif
			cycleLength = std::min(list.size(), (int)numneurons);
		}
		for( unsigned int i=0; i<cycleLength; i++) {
			bool ok;
			(*outputdataptr)[i] = list[i].toDouble(&ok);
			if (!ok) {
				(*outputdataptr)[i] = 0.0;
			}
		}
	}

	// The group for the outputFunction is "prefix/OutFunction"
	try {
		setOutFunction(params.getObjectFromGroup<OutputFunction>( prefix + "OutFunction", true ));
	} catch( std::exception& e ) {
		// SigmoidFunction as Default
		setOutFunction(new SigmoidFunction( 1.0 ));
	}
	setNeedReset( false );
}

Cluster::~Cluster() {
	// No need to delete anything else, we use auto_ptr
}

void Cluster::setOutFunction( OutputFunction *up ) {
	updater.reset(up);
	updater->setCluster( this );
}

void Cluster::setInput( unsigned int neuron, double value ) {
	(*inputdataptr)[neuron] = value;
}

void Cluster::setInputs( const DoubleVector& inputs ) {
	inputdataptr->copyValues( inputs );
}

void Cluster::setAllInputs( double value ) {
	inputdataptr->setAll( value );
	setNeedReset( false );
}

void Cluster::resetInputs() {
	inputdataptr->zeroing();
	setNeedReset( false );
}

double Cluster::getInput( unsigned int neuron ) const {
	return (*inputdataptr)[neuron];
}

void Cluster::setOutput( unsigned int neuron, double value ) {
	(*outputdataptr)[neuron] = value;
}

void Cluster::setOutputs( const DoubleVector& outputs ) {
	outputdataptr->copyValues( outputs );
}

double Cluster::getOutput( unsigned int neuron ) const {
	return (*outputdataptr)[neuron];
}

void Cluster::save(ConfigurationParameters& params, QString prefix)
{
	Updatable::save( params, prefix );
	params.startObjectParameters(prefix, "Cluster", this);
	params.createParameter(prefix, "numNeurons", QString::number(numneurons));
	params.createParameter(prefix, "accumulate", (isAccumulate() ? "True" : "False"));
	// First creating a string list, then transforming to a single string
	QStringList list;
	for (unsigned int i = 0; i < inputdataptr->size(); i++) {
		list.push_back(QString::number((*inputdataptr)[i]));
	}
	params.createParameter(prefix, "inputs", list.join(" "));
	// Doing the same with outputdata
	list.clear();
	for (unsigned int i = 0; i < outputdataptr->size(); i++) {
		list.push_back(QString::number((*outputdataptr)[i]));
	}
	params.createParameter(prefix, "outputs", list.join(" "));
	// and finally the outfunction will be saved in the group "prefix/OutFunction"
	updater->save( params, params.createSubGroup(prefix, "OutFunction") );
}

}
