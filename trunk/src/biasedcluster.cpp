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

#include "biasedcluster.h"
#include "liboutputfunctions.h"
#include "random.h"
#include <cstdio>
#include <cstring>


namespace nnfw {

BiasedCluster::BiasedCluster( u_int numNeurons, const char* name )
    : Cluster( numNeurons, name), biasesdata(numNeurons), tempdata(numNeurons) {
    biasesdata.zeroing();
    tempdata.zeroing();
    propdefs();
    setTypename( "BiasedCluster" );
}

BiasedCluster::BiasedCluster( PropertySettings& prop )
    : Cluster( prop ), biasesdata( size() ), tempdata( size() ) {
    Variant& v = prop["biases"];
    if ( v.isNull() ) {
        biasesdata.zeroing();
    } else {
        setBiases( v );
    }
    propdefs();
    setTypename( "BiasedCluster" );
}

BiasedCluster::~BiasedCluster() {
}

void BiasedCluster::update() {
    tempdata.assign_xminusy( inputs(), biases() );
    getFunction()->apply( tempdata, outputs() );
    setNeedReset( true );
}

void BiasedCluster::setBias( u_int neuron, Real bias ) {
    if ( neuron >= size() ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation setBias will be ignored", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return;
    }
    biasesdata[neuron] = bias;
}

void BiasedCluster::setAllBiases( Real bias ) {
    biases().assign( size(), bias );
}

void BiasedCluster::setBiases( const RealVec& bias ) {
    biases().assign( bias );
}

Real BiasedCluster::getBias( u_int neuron ) {
    if ( neuron >= size() ) {
        char msg[100];
        sprintf( msg, "The neuron %u doesn't exists! The operation getBias will return 0.0", neuron );
        nnfwMessage( NNFW_ERROR, msg );
        return 0.0;
    }
    return biasesdata[neuron];
}

void BiasedCluster::randomize( Real min, Real max ) {
    for ( u_int i = 0; i < size(); i++ ) {
        biasesdata[i] = Random::flatReal( min, max );
    }
}

void BiasedCluster::propdefs() {
    addProperty( "biases", Variant::t_realvec, this, &BiasedCluster::getBiases, &BiasedCluster::setBiases );
}

}
