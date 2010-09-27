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

namespace nnfw {

Cluster::Cluster( unsigned int numNeurons, QString name )
	: Updatable(name), inputdata(numNeurons, true), outputdata(numNeurons, true) {
	this->numneurons = numNeurons;
	outputdata.zeroing();
	inputdata.zeroing();
	accOff = true;
	setNeedReset( false );
	//! SigmoidFunction as Default
	updater = new SigmoidFunction( 1.0 );
}

Cluster::Cluster( ConfigurationParameters& params, QString prefix ) :
	Updatable(params, prefix),
	sdfgsdafsfsadfsaddfsadfs
{
	QUIQUIQUI
	addProperty( "numNeurons", Variant::t_uint, this, &Cluster::numNeuronsP );
	addProperty( "accumulate", Variant::t_bool, this, &Cluster::accumP, &Cluster::setAccumP );
	addProperty( "inputs", Variant::t_realvec, this, &Cluster::inputsP, &Cluster::setInputsP );
	addProperty( "outputs", Variant::t_realvec, this, &Cluster::outputsP, &Cluster::setOutputsP );
	addProperty( "outfunction", Variant::t_outfunction, this, &Cluster::getFunctionP, &Cluster::setFunction );
}

Cluster::~Cluster() {
	delete updater;
}

void Cluster::setFunction( const OutputFunction& up ) {
	delete updater;
	updater = up.clone();
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

	QUIQUIQUI

	params.createParameter(prefix, "value", QString::number(valuev));
}

}
