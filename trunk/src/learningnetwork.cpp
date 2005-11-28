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

#include "learningnetwork.h"
#include "updatable.h"
#include "blockslearning.h"
#include "messages.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

LearningNetwork::LearningNetwork()
    : blocksv(), bls(), mask() {
    dimBls = 0;
}

LearningNetwork::~LearningNetwork() {
    // NOthing to do
}

void LearningNetwork::addTeachBlock( BaseTeachBlock* b ) {
    if ( find( b ) ) {
        nnfwMessage( NNFW_ERROR, "TeachBlock already added! addTeachBlock will be ignored" );
        return;    
    }
    blocksv.push_back(b);
}

bool LearningNetwork::isIsolated( BaseTeachBlock* b ) const {
    // Uhmmm... come lo implemento ?!?!
    nnfwMessage( NNFW_WARNING, "Not implemented yet" );
    return false;
}

const TeachBlockVec& LearningNetwork::teachBlocks() const {
    return blocksv;
}

void LearningNetwork::setOrder( BaseTeachBlock* blocks[], u_int dim ) {
    bls.clear();
    for( u_int i=0; i<dim; i++ ) {
        if ( find(blocks[i]) ) {
            bls.push_back( blocks[i] );
        }
    }
    dimBls = bls.size();
    mask.resize( dimBls );
    mask.assign( dimBls, true );
    return;
}

void LearningNetwork::setOrder( TeachBlockVec& bv ) {
    u_int dim = bv.size();
    bls.clear();
    for( u_int i=0; i<dim; i++ ) {
        if ( find(bv[i]) ) {
            bls.push_back( bv[i] );
        }
    }
    dimBls = bls.size();
    mask.resize( dimBls );
    mask.assign( dimBls, true );
    return;    
}

void LearningNetwork::setOrder( orderPattern ordp ) {
    // Potrei ordinarli senza avere la classe BaseNeuralNet, ma ci metterei O(n^2) (giusto?!?!)
    // con la classe BaseNeuralNet solo O(n)
    nnfwMessage( NNFW_WARNING, "Not implemented yet" );
}

void LearningNetwork::disable( BaseTeachBlock* b ) {
    for( u_int i = 0; i<dimBls; i++ ) {
        if ( bls[i] == b ) {
            mask[i] = false;
            return;
        }
    }
    //nnfwMessage( NNFW_WARNING, "an attemp to Disabling a BaseTeachBlock not present in LearningNetwork" );
    return;
}

void LearningNetwork::enable( BaseTeachBlock* b ) {
    for( u_int i = 0; i<dimBls; i++ ) {
        if ( bls[i] == b ) {
            mask[i] = true;
            return;
        }
    }
    //nnfwMessage( NNFW_WARNING, "an attemp to Enabling a BaseTeachBlock not present in LearningNetwork" );
    return;
}

bool LearningNetwork::isEnabled( BaseTeachBlock* b ) {
    for( u_int i = 0; i<dimBls; i++ ) {
        if ( bls[i] == b ) {
            return mask[i];
        }
    }
    //nnfwMessage( NNFW_WARNING, "BaseTeachBlock not present in LearningNetwork" );
    return false;
}

bool LearningNetwork::find( const BaseTeachBlock* b ) const {
    u_int dim = blocksv.size();
    for( u_int i = 0; i<dim; i++ ) {
        if ( blocksv[i] == b ) {
            return true;
        }
    }
    return false;
}

}
