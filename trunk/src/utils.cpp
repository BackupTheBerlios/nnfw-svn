/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
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
#include "nnfwfactory.h"
#include "propertized.h"
#include <stack>

namespace nnfw {

BaseNeuralNet* feedForwardNet( U_IntVec layers, const char* clusterType, const char* linkerType ) {
    BaseNeuralNet* net = new BaseNeuralNet();
    Cluster* prec = 0;
    Cluster* curr = 0;
    Linker* ml = 0;
    UpdatableVec ord;
    char buf[150];
    u_int clCount = 1;
    u_int mlCount = 1;
    for( u_int i=0; i<layers.size(); i++ ) {
        PropertySettings prop;
        sprintf( buf, "%d", layers[i] );
        prop["numNeurons"] = buf;
        sprintf( buf, "%s%d", clusterType, clCount );
        prop["name"] = buf;
        curr = Factory::createCluster( clusterType, prop );
        if ( i == 0 ) {
            net->addCluster( curr, true );
        } else if ( i == (layers.size()-1) ) {
            net->addCluster( curr, false, true );
        } else {
            net->addCluster( curr );
        }
        clCount++;
        //ord << curr;
        if ( prec != 0 ) {
            PropertySettings prop;
			prop["baseneuralnet"] = Variant( net );
            sprintf( buf, "%s", prec->getName() );
            prop["from"] = buf;
            sprintf( buf, "%s", curr->getName() );
            prop["to"]   = curr;
            sprintf( buf, "%s%d", linkerType, mlCount );
            prop["name"] = buf;
            ml = Factory::createLinker( linkerType, prop );
            net->addLinker( ml );
/*            Updatable* tmp = ord[ord.size()-1];
            ord[ord.size()-1] = ml;
            ord.push_back(tmp);*/
            mlCount++;
        }
        ord << ml << curr;
        prec = curr;
    }
    net->setOrder( ord );
    return net;
}

typedef struct {
    MatrixLinker* ml;
    GradientBiasedCluster* post;
} linkInfo;
typedef struct {
    BiasedCluster* cl;
    GradientMatrixLinker* post;
} clusInfo;

LearningNetwork* backpropagationFor( BaseNeuralNet* net ) {
    LearningNetwork* learnNet = new LearningNetwork();
    TeachBlockVec ord;

    std::stack<linkInfo> qml;
    std::stack<clusInfo> qcl;

    // Inizializzo qcl con i BiasedCluster dello strato di output
    const ClusterVec cv = net->outputClusters();
    u_int dim = cv.size();
    for( u_int i=0; i<dim; i++ ) {
        BiasedCluster* cl = dynamic_cast<BiasedCluster*>( cv[i] );
        if ( !cl ) continue;
        clusInfo info;
        info.cl = cl;
        info.post = 0;
        qcl.push( info );
    }

    char buf[150];
    while( qcl.size() != 0 || qml.size() != 0 ) {
        // Inserisco nella learnNet tutti i BiasedCluster dello strato corrente e
        // costruisco la coda dei MatrixLinker entranti su questi BiasedCluster
        while( qcl.size() != 0 ) {
            clusInfo info = qcl.top();
            qcl.pop();
            sprintf( buf, "Gradient%s", (info.cl)->getName() );
            GradientBiasedCluster* gcl = new GradientBiasedCluster( info.cl, 0, info.post, buf );
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
        // costruisco la coda dei BiasedCluster di partenza dei MatrixLinker della coda
        while( qml.size() != 0 ) {
            linkInfo info = qml.top();
            qml.pop();
            sprintf( buf, "Gradient%s", (info.ml)->getName() );
            GradientMatrixLinker* gml = new GradientMatrixLinker( info.ml, 0, info.post, buf );
            learnNet->addTeachBlock( gml );
            ord << gml;
            BiasedCluster* preC = dynamic_cast<BiasedCluster*>( (info.ml)->getFrom() );
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
        GradientBiasedCluster* gcl = dynamic_cast<GradientBiasedCluster*>(bls[i]);
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

