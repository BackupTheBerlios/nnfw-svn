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
#include "libclusterupdaters.h"
#include "messages.h"
#include "random.h"
#include <cstdio>
#include <cstring>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

BiasedCluster::BiasedCluster( u_int numNeurons, const char* name )
    : Cluster( numNeurons, name), biasesdata(numNeurons), tmpdata(numNeurons) {
    biasesdata.zeroing();
    tmpdata.zeroing();
/*    biasesdata = new Real[numNeurons];
    memset( biasesdata, 0, sizeof(Real)*numNeurons );
    tmpdata = new Real[numNeurons];
    memset( tmpdata, 0, sizeof(Real)*numNeurons );*/
}

BiasedCluster::~BiasedCluster() {
/*    delete []biasesdata;
    delete []tmpdata;*/
}

void BiasedCluster::update() {
    if ( isSingleUpdater() ) {
        tmpdata.assign( inputs() );
        tmpdata -= biasesdata;
        updaters()[0]->update( tmpdata, outputs() );
    } else {
        for ( u_int i = 0; i<size(); i++ ) {
            updaters()[i]->update( inputs()[i] - biasesdata[i], outputs()[i] );
        }
    }	
	oldInputs().assign( inputs() );
	oldInputs() -= biasesdata;
	resetInputs();
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
    biasesdata.assign( size(), bias );
/*    for( u_int i=0; i<size(); i++ ) {
        biasesdata[i] = bias;
    }*/
}

void BiasedCluster::setBiases( const RealVec& bias ) {
    biasesdata.assign( bias );
/*    u_int dim = biasesdata.size();
    for( u_int i=0; i<dim; i++ ) {
        setBias( i, biasesdata[i] );
    }*/
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

}
