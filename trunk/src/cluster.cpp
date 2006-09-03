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
#include "libclusterupdaters.h"
#include "messages.h"
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

    //! SigmoidUpdater as Default Updater
    updater = new SigmoidUpdater( 1.0 );

    //! Properties definition
    addProperty( "inputs", Variant::t_realvec, this, &Cluster::inputsP, &Cluster::setInputsP );
    addProperty( "outputs", Variant::t_realvec, this, &Cluster::outputsP, &Cluster::setOutputsP );
}

Cluster::~Cluster() {
    delete updater;
}

void Cluster::setUpdater( const ClusterUpdater& up ) {
    delete updater;
    updater = up.clone();
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

}
