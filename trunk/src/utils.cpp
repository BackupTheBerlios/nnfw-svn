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
#include <stack>

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
            Updatable* tmp = ord[ord.size()-1];
            ord[ord.size()-1] = ml;
            ord.push_back(tmp);
            mlCount++;
        }
        prec = curr;
    }
    net->setOrder( ord );
    return net;
}

typedef struct {
    MatrixLinker* ml;
    GradientSimpleCluster* post;
} linkInfo;
typedef struct {
    SimpleCluster* cl;
    GradientMatrixLinker* post;
} clusInfo;

LearningNetwork* backpropagationFor( BaseNeuralNet* net ) {
    LearningNetwork* learnNet = new LearningNetwork();
    TeachBlockVec ord;

    std::stack<linkInfo> qml;
    std::stack<clusInfo> qcl;

    // Inizializzo qcl con i SimpleCluster dello strato di output
    const ClusterVec cv = net->outputClusters();
    u_int dim = cv.size();
    for( u_int i=0; i<dim; i++ ) {
        SimpleCluster* cl = dynamic_cast<SimpleCluster*>( cv[i] );
        if ( !cl ) continue;
        clusInfo info;
        info.cl = cl;
        info.post = 0;
        qcl.push( info );
    }

    while( qcl.size() != 0 || qml.size() != 0 ) {
        // Inserisco nella learnNet tutti i SimpleCluster dello strato corrente e
        // costruisco la coda dei MatrixLinker entranti su questi SimpleCluster
        while( qcl.size() != 0 ) {
            clusInfo info = qcl.top();
            qcl.pop();
            GradientSimpleCluster* gcl = new GradientSimpleCluster( info.cl, 0, info.post );
            learnNet->addTeachBlock( gcl );
            ord << gcl;
            const LinkerVec lv = net->linkers( info.cl, false );
            dim = lv.size();
            for( u_int i=0; i<dim; i++ ) {
                MatrixLinker* ml = dynamic_cast<MatrixLinker*>( lv[i] );
                if ( !ml ) continue;
                linkInfo infoM;
                infoM.ml = ml;
                infoM.post = gcl;
                qml.push( infoM );
            }
        }
        // Inserisco nella learnNet tutti i MatrixLinker dello strato corrente e
        // costruisco la coda dei SimpleCluster di partenza dei MatrixLinker della coda
        while( qml.size() != 0 ) {
            linkInfo info = qml.top();
            qml.pop();
            GradientMatrixLinker* gml = new GradientMatrixLinker( info.ml, 0, info.post );
            learnNet->addTeachBlock( gml );
            ord << gml;
            SimpleCluster* preC = dynamic_cast<SimpleCluster*>( (info.ml)->getFrom() );
            if ( preC != 0 ) {
                clusInfo infoC;
                infoC.cl = preC;
                infoC.post = gml;
                qcl.push( infoC );
            }
        }
    }

    learnNet->setOrder( ord );
    return learnNet;
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

