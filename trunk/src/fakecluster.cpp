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

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/**********************************************
 *  Implementation of FakeCluster Class       *
 **********************************************/

FakeCluster::FakeCluster( u_int size, const char* name )
    : Cluster(name) {
    dim = size;
    data = new Real[dim];
    updater = DummyUpdater();
}

FakeCluster::~FakeCluster() {
    delete []data;
}


u_int FakeCluster::size() const {
    return dim;
}

void FakeCluster::setUpdater( ClusterUpdater* ) {
    nnfwMessage( NNFW_INFORMATION, "You are trying to set a ClusterUpdater on a FakeCluster !! ;-)" );
}

void FakeCluster::setUpdater( ClusterUpdater*, u_int ) {
    nnfwMessage( NNFW_INFORMATION, "You are trying to set a ClusterUpdater on a FakeCluster !! ;-)" );
}

void FakeCluster::update() {
    return;
}

const ClusterUpdater* FakeCluster::getUpdater( u_int ) const {
    return &updater;
}

void FakeCluster::setInput( u_int neuron, Real value ) {
    if ( neuron >= dim ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setInput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    data[neuron] = value;
}

void FakeCluster::setAllInputs( Real value ) {
    for ( u_int i = 0; i<dim; i++ ) {
        data[i] = value;
    }
}

void FakeCluster::resetInputs() {
    memset( data, 0, dim*sizeof( Real ) );
}

Real FakeCluster::getInput( u_int neuron ) const {
    if ( neuron >= dim ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getInput will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return data[neuron];
}

Real* FakeCluster::getInputs() {
    return data;
}

void FakeCluster::setOutput( u_int neuron, Real value ) {
    if ( neuron >= dim ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setOutput will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    data[neuron] = value;
}

Real FakeCluster::getOutput( u_int neuron ) const {
    if ( neuron >= dim ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getOutput will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return data[neuron];
}

Real* FakeCluster::getOutputs() {
    return data;
}

void FakeCluster::randomize( Real , Real ) {
    nnfwMessage( NNFW_INFORMATION, "What means 'randomize a Fake Cluster' ???" );
    return;
}

}
