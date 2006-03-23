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
    : Updatable(name) {
    this->numNeurons = numNeurons;
    outputdata = new Real[this->numNeurons];
    inputdata = new Real[this->numNeurons];
    memset( inputdata, 0, sizeof(Real)*this->numNeurons );
    memset( outputdata, 0, sizeof(Real)*this->numNeurons );
    //! Allocation for poolUpdater
    poolUpdater = new ( ClusterUpdater ( *[this->numNeurons] ) );
    //! SigmoidUpdater as Default Updater
    setUpdater( new SigmoidUpdater( 1.0 ) );
}

Cluster::~Cluster() {
    delete []outputdata;
    delete []inputdata;
    delete []poolUpdater;
}

void Cluster::setUpdater( ClusterUpdater* up ) {
    singleUpdater = up;
    singleUpd = true;
    // Copy this updater to poolUpdater for future settings by setUpdater( up, numNeuron )
    //  and for simpler implementation of getUpdater
    for( u_int i = 0; i<numNeurons; i++ ) {
        poolUpdater[i] = singleUpdater;
    }
}

void Cluster::setUpdater( ClusterUpdater* up, u_int neuron ) {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setUpdater will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    poolUpdater[neuron] = up;
    singleUpd = false;
}

const ClusterUpdater* Cluster::getUpdater( u_int neuron ) const {
    if ( neuron >= numNeurons ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setInput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        // ---- FIXME returning reference to temporary object !! (NON E' vero che e' temporary... e un memory leak)
        return new DummyUpdater();
    }
    return poolUpdater[ neuron ];
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

void Cluster::setAllInputs( Real value ) {
    for ( u_int i = 0; i<numNeurons; i++ ) {
        inputdata[i] = value;
    }
}

void Cluster::resetInputs() {
    memset( inputdata, 0, numNeurons*sizeof( Real ) );
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
