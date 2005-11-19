/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005 Gianluca Massera <emmegian@yahoo.it>                     *
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

#include "simplecluster.h"
#include "libclusterupdaters.h"
#include "messages.h"
#include "random.h"
#include <cstdio>
#include <cstring>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/**********************************************
 *  Implementation of SimpleCluster Class     *
 **********************************************/

SimpleCluster::SimpleCluster( u_int numNeurons, const char* name )
    : Cluster(name) {
    this->numNeurons = numNeurons;
    outputdata = new Real[this->numNeurons];
    inputdata = new Real[this->numNeurons];
    biases = new Real[this->numNeurons];
    memset( inputdata, 0, sizeof(Real)*this->numNeurons );
    memset( outputdata, 0, sizeof(Real)*this->numNeurons );
    memset( biases, 0, sizeof(Real)*this->numNeurons );
    //! SigmoidUpdater as Default Updater
    singleUpdater = new SigmoidUpdater( 1.0 );
    singleUpd = true;
    //! Allocation for poolUpdater
    poolUpdater = new ( ClusterUpdater ( *[this->numNeurons] ) );

    tmpdata = new Real[this->numNeurons];
    memset( tmpdata, 0, sizeof(Real)*this->numNeurons );
}

SimpleCluster::~SimpleCluster() {
    delete []outputdata;
    delete []inputdata;
    delete []biases;
    delete []poolUpdater;
}


u_int SimpleCluster::size() const {
    return numNeurons;
}

void SimpleCluster::setUpdater( ClusterUpdater* up ) {
    singleUpdater = up;
    singleUpd = true;
    // Copy this updater to poolUpdater for future settings by setUpdater( up, numNeuron )
    //  and for simpler implementation of getUpdater
    for( u_int i = 0; i<numNeurons; i++ ) {
        poolUpdater[i] = singleUpdater;
    }
}

void SimpleCluster::setUpdater( ClusterUpdater* up, u_int neuron ) {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setUpdater will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    poolUpdater[neuron] = up;
    singleUpd = false;
}

void SimpleCluster::update() {
    if ( singleUpd ) {
        for ( u_int i = 0; i<numNeurons; i++ ) {
            tmpdata[i] = inputdata[i] - biases[i];
        }
        singleUpdater->update( tmpdata, outputdata, numNeurons );
    } else {
        for ( u_int i = 0; i<numNeurons; i++ ) {
            poolUpdater[i]->update( inputdata[i] - biases[i], outputdata[i] );
        }
    }
}

const ClusterUpdater* SimpleCluster::getUpdater( u_int neuron ) const {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setInput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        // ---- FIXME returning reference to temporary object !!!!!!!!
        return new DummyUpdater();
    }
    return poolUpdater[ neuron ];
}

void SimpleCluster::setInput( u_int neuron, Real value ) {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setInput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    inputdata[neuron] = value;
}

void SimpleCluster::setAllInputs( Real value ) {
    for ( u_int i = 0; i<numNeurons; i++ ) {
        inputdata[i] = value;
    }
}

void SimpleCluster::resetInputs() {
    memset( inputdata, 0, numNeurons*sizeof( Real ) );
}

Real SimpleCluster::getInput( u_int neuron ) const {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getInput will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return inputdata[neuron];
}

Real* SimpleCluster::getInputs() {
    return inputdata;
}

void SimpleCluster::setOutput( u_int neuron, Real value ) {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setOutput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    outputdata[neuron] = value;
}

Real SimpleCluster::getOutput( u_int neuron ) const {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getOutput will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return outputdata[neuron];
}

Real* SimpleCluster::getOutputs() {
    return outputdata;
}

void SimpleCluster::setBias( u_int neuron, Real bias ) {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setBias will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    biases[neuron] = bias;
}

Real SimpleCluster::getBias( u_int neuron ) {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getBias will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return biases[neuron];
}

void SimpleCluster::randomize( Real min, Real max ) {
    for ( u_int i = 0; i < numNeurons; i++ ) {
        biases[i] = Random::flatReal( min, max );
    }
}

}
