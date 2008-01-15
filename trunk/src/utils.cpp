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
            sprintf( buf, "%s", prec->name() );
            prop["from"] = buf;
            sprintf( buf, "%s", curr->name() );
            prop["to"]   = buf;
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

}

