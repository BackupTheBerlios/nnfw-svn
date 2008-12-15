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

#include "cluster.h"
#include "liboutputfunctions.h"
#include "random.h"

namespace nnfw {

/**********************************************
 *  Implementation of Cluster Class     *
 **********************************************/

Cluster::Cluster( unsigned int numNeurons, const char* name )
    : Updatable(name), inputdata(numNeurons), outputdata(numNeurons) {
    this->numneurons = numNeurons;
    outputdata.zeroing();
    inputdata.zeroing();
    accOff = true;
    setNeedReset( false );

    //! SigmoidFunction as Default
    updater = new SigmoidFunction( 1.0 );

    //! Properties definition
    propdefs();
    // setTypename( "Cluster" ); --- it's no instianciable
}

Cluster::Cluster( PropertySettings& prop )
    : Updatable(prop), inputdata(0), outputdata(0) {
    // --- Configuring Name
    Variant& v = prop["name"];
    if ( !v.isNull() ) {
        setName( prop["name"].getString() );
    } else {
		nFatal() << "the property name of Cluster is mandatory";
		exit(1);
	}
    // --- Configuring Dimension
	v = prop["numNeurons"];
	if ( !v.isNull() ) {
    	this->numneurons =
			convertStringTo( prop["numNeurons"], Variant::t_uint ).getUInt();
	} else {
		nFatal() << "the dimension of Cluster is mandatory";
		exit(1);
	}
    inputdata.resize( numneurons );
    outputdata.resize( numneurons );
    // --- Configuring Accumulate modality
    v = prop["accumulate"];
    if ( v.isNull() ) {
        accOff = true;
    } else {
        accOff = !( prop["accumulate"].getBool() );
    }
    setNeedReset( false );
    // --- Configuring OutputFunction
    v = prop["outfunction"];
    if ( v.isNull() ) {
        updater = new SigmoidFunction( 1.0 );
    } else {
        updater = v.getOutputFunction()->clone();
    }
    //! Properties definition
    propdefs();
    // setTypename( "Cluster" ); --- it's no instianciable
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
		nError() << "The neuron " << neuron << " doesn't exists! The operation setInput will be ignored";
        return;
    }
#endif
    inputdata[neuron] = value;
}

void Cluster::setInputs( const RealVec& inputs ) {
	inputdata.assign( inputs );
}

void Cluster::setAllInputs( double value ) {
    inputdata.assign( numneurons, value );
    setNeedReset( false );
}

void Cluster::resetInputs() {
    inputdata.zeroing();
    setNeedReset( false );
}

double Cluster::getInput( unsigned int neuron ) const {
#ifdef NNFW_DEBUG
    if ( neuron >= numneurons ) {
        nError() << "The neuron " << neuron << " doesn't exists! The operation getInput will return 0.0";
        return 0.0;
    }
#endif
    return inputdata[neuron];
}

void Cluster::setOutput( unsigned int neuron, double value ) {
#ifdef NNFW_DEBUG
    if ( neuron >= numneurons ) {
        nError() << "The neuron " << neuron << " doesn't exists! The operation setOutput will be ignored";
        return;
    }
#endif
    outputdata[neuron] = value;
}

void Cluster::setOutputs( const RealVec& outputs ) {
	outputdata.assign( outputs );
}

double Cluster::getOutput( unsigned int neuron ) const {
#ifdef NNFW_DEBUG
    if ( neuron >= numneurons ) {
        nError() << "The neuron " << neuron << " doesn't exists! The operation getOutput will return 0.0";
        return 0.0;
    }
#endif
    return outputdata[neuron];
}

Cluster* Cluster::clone() const {
	nError() << "The clone() method has to implemented by subclasses";
	return 0;
}

void Cluster::propdefs() {
    addProperty( "numNeurons", Variant::t_uint, this, &Cluster::numNeuronsP );
    addProperty( "accumulate", Variant::t_bool, this, &Cluster::accumP, &Cluster::setAccumP );
    addProperty( "inputs", Variant::t_realvec, this, &Cluster::inputsP, &Cluster::setInputsP );
    addProperty( "outputs", Variant::t_realvec, this, &Cluster::outputsP, &Cluster::setOutputsP );
    addProperty( "outfunction", Variant::t_outfunction, this, &Cluster::getFunctionP, &Cluster::setFunction );
}

}
