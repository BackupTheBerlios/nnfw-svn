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

#include "types.h"
#include "utils.h"
#include "neuralnet.h"
#include "simplecluster.h"
#include "matrixlinker.h"
#include "blockslearning.h"
#include "learningnetwork.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

BaseNeuralNet* feedForwardNet( U_IntVec layers ) {
    BaseNeuralNet* net = new BaseNeuralNet();
    SimpleCluster* prec = 0;
    SimpleCluster* curr = 0;
    UpdatableVec ord;
    char buf[50];
    u_int clCount = 1;
    u_int mlCount = 1;
    for( u_int i=0; i<layers.size(); i++ ) {
        sprintf( buf, "SimpleCluster%d", clCount );
        curr = new SimpleCluster( layers[i], buf );
        if ( i == 0 ) {
            net->addCluster( curr, true );
        } else if ( i == (layers.size()-1) ) {
            net->addCluster( curr, false, true );
        } else {
            net->addCluster( curr );
        }
        clCount++;
        ord << curr;
        if ( prec != 0 ) {
            sprintf( buf, "MatrixLinker%d", mlCount );
            MatrixLinker* ml = new MatrixLinker( prec, curr, buf );
            net->addLinker( ml );
            ord << ml;
            mlCount++;
        }
        prec = curr;
    }
    net->setOrder( ord );
    return net;
}

LearningNetwork* backpropagationFor( BaseNeuralNet* net ) {
    // da implementare
}

void setParamsOfGradientBlocks( LearningNetwork* learnNet, Real rate, Real momento ) {
    const TeachBlockVec bls = learnNet->teachBlocks();
    for( u_int i=0; i<bls.size(); i++ ) {
        GradientSimpleCluster* gcl = dynamic_cast<GradientSimpleCluster*>(bls[i]);
        if ( gcl != 0 ) {
            gcl->setRate( rate );
            gcl->setMomentum( momento );
            continue;
        }
        GradientMatrixLinker* gml = dynamic_cast<GradientMatrixLinker*>(bls[i]);
        if ( gml != 0 ) {
            gml->setRate( rate );
            gml->setMomentum( momento );
            continue;
        }
    }
}

}

