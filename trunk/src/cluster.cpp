/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2006 Gianluca Massera <emmegian@yahoo.it>                     *
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
#include <cstdio>
#include <cstring>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/**********************************************
 *  Implementation of Cluster Class     *
 **********************************************/

Cluster::Cluster( u_int numNeurons, const char* name )
    : Updatable(name), inputdata(numNeurons), outputdata(numNeurons) {
    this->numNeurons = numNeurons;
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
    }
    // --- Configuring Dimension
    this->numNeurons = prop["size"].getUInt();
    inputdata.resize( numNeurons );
    outputdata.resize( numNeurons );
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

void Cluster::setInput( u_int neuron, Real value ) {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setInput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    inputdata[neuron] = value;
}

void Cluster::setInputs( const RealVec& inputs ) {
	inputdata.assign( inputs );
}

void Cluster::setAllInputs( Real value ) {
    inputdata.assign( numNeurons, value );
    setNeedReset( false );
}

void Cluster::resetInputs() {
    inputdata.zeroing();
    setNeedReset( false );
}

Real Cluster::getInput( u_int neuron ) const {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getInput will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return inputdata[neuron];
}

void Cluster::setOutput( u_int neuron, Real value ) {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setOutput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    outputdata[neuron] = value;
}

void Cluster::setOutputs( const RealVec& outputs ) {
	outputdata.assign( outputs );
}

Real Cluster::getOutput( u_int neuron ) const {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getOutput will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return outputdata[neuron];
}

void Cluster::propdefs() {
    addProperty( "size", Variant::t_uint, this, &Cluster::sizeP );
    addProperty( "accumulate", Variant::t_bool, this, &Cluster::accumP, &Cluster::setAccumP );
    addProperty( "inputs", Variant::t_realvec, this, &Cluster::inputsP, &Cluster::setInputsP );
    addProperty( "outputs", Variant::t_realvec, this, &Cluster::outputsP, &Cluster::setOutputsP );
    addProperty( "outfunction", Variant::t_outfunction, this, &Cluster::getFunctionP, &Cluster::setFunction );
}

}
