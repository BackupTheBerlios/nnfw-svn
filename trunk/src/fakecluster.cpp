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

#include "fakecluster.h"
#include "libclusterupdaters.h"
#include "messages.h"
#include "random.h"
#include <cstdio>
#include <cstring>


/**********************************************
 *  Implementation of FakeCluster Class       *
 **********************************************/

FakeCluster::FakeCluster( u_int size ) {
    dim = size;
    data = new nnfwReal[dim];
    updater = new DummyUpdater();
}

FakeCluster::~FakeCluster() {
    delete []data;
    delete updater;
}


u_int FakeCluster::inputSize() const {
    return dim;
}

u_int FakeCluster::outputSize() const {
    return dim;
}

void FakeCluster::setUpdater( ClusterUpdater* ) {
    nnfwMessage( NNFW_INFORMATION, "You are trying to set a ClusterUpdater on a FakeCluster !! ;-)" );
}

void FakeCluster::update() {
    return;
}

const ClusterUpdater* FakeCluster::getUpdater() const {
    return updater;
}

void FakeCluster::setInput( u_int neuron, nnfwReal value ) {
    if ( neuron >= dim ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setInput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    data[neuron] = value;
}

void FakeCluster::setAllInputs( nnfwReal value ) {
    for ( u_int i = 0; i<dim; i++ ) {
        data[i] = value;
    }
}

void FakeCluster::resetInputs() {
    memset( data, 0, dim*sizeof( nnfwReal ) );
}

nnfwReal FakeCluster::getInput( u_int neuron ) const {
    if ( neuron >= dim ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getInput will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return data[neuron];
}

nnfwReal* FakeCluster::getInputs() {
    return data;
}

void FakeCluster::setOutput( u_int neuron, nnfwReal value ) {
    if ( neuron >= dim ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setOutput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    data[neuron] = value;
}

nnfwReal FakeCluster::getOutput( u_int neuron ) const {
    if ( neuron >= dim ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getOutput will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return data[neuron];
}

nnfwReal* FakeCluster::getOutputs() {
    return data;
}

void FakeCluster::randomize( nnfwReal min, nnfwReal max ) {
    nnfwMessage( NNFW_INFORMATION, "What means 'randomize a Fake Cluster' ???" );
    return;
}
