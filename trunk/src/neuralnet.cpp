/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                *
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

#include "neuralnet.h"
#include "nnfwfactory.h"
#include <algorithm>
#include <functional>
#include <cstring>


namespace nnfw {

/**********************************************
 *  Implementation of BaseNeuralNet Class     *
 **********************************************/

BaseNeuralNet::BaseNeuralNet() {
    dimUps = 0;
}

BaseNeuralNet::~BaseNeuralNet() {
}

void BaseNeuralNet::addCluster( Cluster* c, bool isInput, bool isOutput ) {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to addCluster! This operation will be ignored" ;
        return;
    }
#endif
    // Check if the Cluster is already added
    if ( find( c ) ) {
#ifdef NNFW_DEBUG
        nError() << "Cluster already added! addCluster will be ignored" ;
#endif
        return;
    }
	ids4t& ids = clsIdsMap[c];
	ids[0] = clustersv.size();
	ids[1] = ids[2] = ids[3] = -1;
    clustersv.push_back( c );
    if ( isInput ) {
		ids[1] = inclusters.size();
        inclusters.push_back( c );
    }
    if ( isOutput ) {
		ids[2] = outclusters.size();
        outclusters.push_back( c );
    }
	if ( !isInput && !isOutput ) {
		ids[3] = hidclusters.size();
        hidclusters.push_back( c );
	}
	clsMap[c->name()] = c;
    return;
}

bool BaseNeuralNet::removeCluster( Cluster* c ) {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to removeCluster! This operation will return false" ;
        return false;
    }
#endif
    if ( !find( c ) ) {
        return false;
    }
	ids4t& ids = clsIdsMap[c];
	clustersv.erase( ids[0] );
	inclusters.erase( ids[1] );
	outclusters.erase( ids[2] );
	hidclusters.erase( ids[3] );
	clsMap.erase( c->name() );
	clsIdsMap.erase( c );
    return true;
}

void BaseNeuralNet::markAsInput( Cluster* c ) {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to addCluster! This operation will be ignored" ;
        return;
    }
#endif
    // Check if the Cluster exists
    if ( !find( c ) ) {
#ifdef NNFW_DEBUG
        nError() << "attempt to mark a Cluster not present in this net!" ;
#endif
        return;
    }
	if ( clsIdsMap[c][1] != -1 ) {
		return;
	}
	clsIdsMap[c][1] = inclusters.size();
    inclusters.push_back( c );
}

void BaseNeuralNet::markAsOutput( Cluster* c ) {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to addCluster! This operation will be ignored" ;
        return;
    }
#endif
    // Check if the Cluster exists
    if ( !find( c ) ) {
#ifdef NNFW_DEBUG
        nError() << "attempt to mark a Cluster not present in this net!" ;
#endif
        return;
    }
	if ( clsIdsMap[c][2] != -1 ) {
		return;
	}
	clsIdsMap[c][2] = outclusters.size();
	outclusters.push_back( c );
}

void BaseNeuralNet::unmark( Cluster* c ) {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to addCluster! This operation will be ignored" ;
        return;
    }
#endif
    // Check if the Cluster exists
    if ( !find( c ) ) {
		return;
	}
	ids4t& ids = clsIdsMap[c];
	if ( ids[1] != -1 ) {
		inclusters.erase( ids[1] );
		ids[1] = -1;
	}
	if ( ids[2] != -1 ) {
		outclusters.erase( ids[2] );
		ids[2] = -1;
	}
	if ( ids[3] == -1 ) {
		ids[3] = hidclusters.size();
		hidclusters.push_back( c );
	}
    return;
}

void BaseNeuralNet::unmarkAll( ) {
	// --- it could be more efficient !!
    inclusters.clear();
    outclusters.clear();
	hidclusters = clustersv;
	IdsMap::iterator it;
	for( it = clsIdsMap.begin(); it!=clsIdsMap.end(); it++ ) {
		ids4t& ids = (*it).second;
		ids[1] = ids[2] = -1;
		ids[3] = ids[0];
	}
    return;
}

bool BaseNeuralNet::isIsolated( Cluster* c ) const {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to isIsolato! This operation will return false" ;
        return false;
    }
#endif
    return ( inLinks.count( c ) == 0 && outLinks.count( c ) == 0 );
}

const ClusterVec& BaseNeuralNet::clusters() const {
    return clustersv;
}

const ClusterVec& BaseNeuralNet::inputClusters() const {
    return inclusters;
}

const ClusterVec& BaseNeuralNet::outputClusters() const {
    return outclusters;
}

const ClusterVec& BaseNeuralNet::hiddenClusters() const {
    return hidclusters;
}

void BaseNeuralNet::addLinker( Linker* l ) {
#ifdef NNFW_DEBUG
    if ( !l ) {
        nError() << "Null Pointer passed to addLinker! This operation will be ignored" ;
        return;
    }
#endif
    // Check if the Linker is already added
    if ( find( l ) ) {
#ifdef NNFW_DEBUG
        nError() << "Linker already added! addLinker will be ignored" ;
#endif
        return;
    }
#ifdef NNFW_DEBUG
    // --- Check: Are There in this net the Clusters that linker l connects ???
    if ( ! find( l->getFrom() ) ) {
        nError() << "The linker that you want add links clusters that doesn't exist in this net! \
                                  This operation will be ignored" ;
        return;
    }
    if ( ! find( l->getTo() ) ) {
        nError() << "The linker that you want add links clusters that doesn't exist in this net! \
                                  This operation will be ignored" ;
        return;
    }
#endif
	ids4t& ids = lksIdsMap[l];
	ids[0] = linkersv.size();
    linkersv.push_back( l );
    // Adding information in outLinks map
	ids[1] = outLinks[ l->getFrom() ].size();
    outLinks[ l->getFrom() ].push_back( l );
    // Adding information in inLinks map
	ids[2] = outLinks[ l->getTo() ].size();
    inLinks[ l->getTo() ].push_back( l );

	lksMap[l->name()] = l;
    return;
}

bool BaseNeuralNet::removeLinker( Linker* l ) {
#ifdef NNFW_DEBUG
    if ( !l ) {
        nError() << "Null Pointer passed to removeLinker! This operation will return false" ;
        return false;
    }
#endif
	if ( !find(l) ) {
		return false;
	}
	ids4t& ids = lksIdsMap[l];
	linkersv.erase( ids[0] );
	outLinks[ l->getFrom() ].erase( ids[1] );
	inLinks[ l->getTo() ].erase( ids[2] );
	lksMap.erase( l->name() );
	lksIdsMap.erase( l );
    return true;
}

const LinkerVec& BaseNeuralNet::linkers() const {
    return linkersv;
}

const LinkerVec& BaseNeuralNet::linkers( Cluster* c, bool out ) const {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to linkers! This operation will return an empty LinkerGroup" ;
        return emptyLinkerVec;
    }
#endif
    if ( out ) {
        // Return outgoing linkers
        if ( outLinks.count( c ) > 0 ) {
            return outLinks.find( c )->second;
        }
    } else {
        // Return incoming linkers
        if ( inLinks.count( c ) > 0 ) {
            return inLinks.find( c )->second;
        }
    }
    return emptyLinkerVec;
}

void BaseNeuralNet::setOrder( Updatable* u[], u_int dim ) {
    ups.clear();
    for( u_int i = 0; i<dim; i++ ) {
        if ( find( u[i] ) ) {
            ups.push_back( u[i] );
        }
    }
    dimUps = ups.size();
    return;
}

void BaseNeuralNet::setOrder( const UpdatableVec& u ) {
    ups.clear();
    u_int dim = u.size();
    for( u_int i = 0; i<dim; i++ ) {
        if ( find( u[i] ) ) {
            ups.push_back( u[i] );
        }
    }
    dimUps = ups.size();
    return;
}

void BaseNeuralNet::randomize( Real min, Real max ) {
	int dim = clustersv.size();
	for( int i=0; i<dim; i++ ) {
		clustersv[i]->randomize( min, max );
	}
	for( int i=0; i<dim; i++ ) {
		linkersv[i]->randomize( min, max );
	}
}

Updatable* BaseNeuralNet::getByName( const char* name ) {
	if ( clsMap.find( name ) != clsMap.end() ) {
		return clsMap[name];
	}
	if ( lksMap.find( name ) != lksMap.end() ) {
		return lksMap[name];
	}
#ifdef NNFW_DEBUG
    nWarning() << "Updatable not present in BaseNeuralNet!!!";
#endif
    return NULL;
}

bool BaseNeuralNet::find( const Cluster* cl ) const {
	return ( clsIdsMap.count( (Cluster*)cl ) > 0 );
}

bool BaseNeuralNet::find( const Linker* l ) const {
	return ( lksIdsMap.count( (Linker*)l ) > 0 );
}

bool BaseNeuralNet::find( const Updatable* u ) const {
	return (
		( clsIdsMap.count( (Cluster*)u ) > 0 ) ||
		( lksIdsMap.count( (Linker*)u ) > 0 )
	);
}

BaseNeuralNet* BaseNeuralNet::clone() const {
	BaseNeuralNet* clone = new BaseNeuralNet();
	for( int i=0; i<(int)inputClusters().size(); i++ ) {
		clone->addInputCluster( inputClusters()[i]->clone() );
	}
	for( int i=0; i<(int)outputClusters().size(); i++ ) {
		clone->addOutputCluster( outputClusters()[i]->clone() );
	}
	// --- not efficient way to add 'hidden clusters'
	for( int i=0; i<(int)clusters().size(); i++ ) {
		if ( clone->getByName( clusters()[i]->name() ) == 0 ) {
			clone->addCluster( clusters()[i]->clone() );
		}
	}
	// --- putting linkers
	for( int i=0; i<(int)linkers().size(); i++ ) {
		Linker* lk = linkers()[i];
		PropertySettings prop;
		lk->propertySettings( prop );
		// --- change the neural network of belong
		prop["baseneuralnet"] = Variant( clone );
		// --- set from and to using the string version
		prop["from"] = lk->from()->name();
		prop["to"] = lk->to()->name();
		// --- create a new clone according to PropertySettings retrieved
		Linker* nl = Factory::createLinker( lk->getTypename().getString(), prop );
		clone->addLinker( nl );
	}
	// --- copy the order -- not-efficient
	UpdatableVec ord;
	for( int i=0; i<(int)order().size(); i++ ) {
		ord << clone->getByName( order()[i]->name() );
	}
	clone->setOrder( ord );
	return clone;
}

}
