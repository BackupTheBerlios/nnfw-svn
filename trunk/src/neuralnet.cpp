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
    clustersv.push_back( c );
    if ( isInput ) {
        inclusters.push_back( c );
    }
    if ( isOutput ) {
        outclusters.push_back( c );
    }
    return;
}

bool BaseNeuralNet::removeCluster( Cluster* c ) {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to removeCluster! This operation will return false" ;
        return false;
    }
#endif
    ClusterVec::iterator it = std::find( clustersv.begin(), clustersv.end(), c );
    if ( it == clustersv.end() ) {
        return false;
    }
    clustersv.erase( it );
    it = std::find( inclusters.begin(), inclusters.end(), c );
    if ( it != inclusters.end() ) {
        inclusters.erase( it );
    }
    it = std::find( outclusters.begin(), outclusters.end(), c );
    if ( it != outclusters.end() ) {
        outclusters.erase( it );
    }
    return true;
}

void BaseNeuralNet::markAsInput( Cluster* c ) {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to addCluster! This operation will be ignored" ;
        return;
    }
    // Check if the Cluster is already added
    if ( !find( c ) ) {
        nError() << "attempt to mark a Cluster not present in this net!" ;
        return;
    }
#endif
    ClusterVec::iterator it = std::find( inclusters.begin(), inclusters.end(), c );
    if ( it != inclusters.end() ) {
        // --- already marked as Input
        return;
    }
    inclusters.push_back( c );
}

void BaseNeuralNet::markAsOutput( Cluster* c ) {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to addCluster! This operation will be ignored" ;
        return;
    }
    // Check if the Cluster is already added
    if ( !find( c ) ) {
        nError() << "attempt to mark a Cluster not present in this net!" ;
        return;
    }
#endif
    ClusterVec::iterator it = std::find( outclusters.begin(), outclusters.end(), c );
    if ( it != outclusters.end() ) {
        // --- already marked as Output
        return;
    }
    outclusters.push_back( c );
}

void BaseNeuralNet::unmark( Cluster* c ) {
#ifdef NNFW_DEBUG
    if ( !c ) {
        nError() << "Null Pointer passed to addCluster! This operation will be ignored" ;
        return;
    }
#endif
    ClusterVec::iterator it = std::find( inclusters.begin(), inclusters.end(), c );
    if ( it != inclusters.end() ) {
        inclusters.erase( it );
    }
    it = std::find( outclusters.begin(), outclusters.end(), c );
    if ( it != outclusters.end() ) {
        outclusters.erase( it );
    }
    return;
}

void BaseNeuralNet::unmarkAll( ) {
    inclusters.clear();
    outclusters.clear();
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
    linkersv.push_back( l );

    // Adding information in outLinks map
    outLinks[ l->getFrom() ].push_back( l );
    // Adding information in inLinks map
    inLinks[ l->getTo() ].push_back( l );
    return;
}

bool BaseNeuralNet::removeLinker( Linker* l ) {
#ifdef NNFW_DEBUG
    if ( !l ) {
        nError() << "Null Pointer passed to removeLinker! This operation will return false" ;
        return false;
    }
#endif
    LinkerVec::iterator it1 = std::find( linkersv.begin(), linkersv.end(), l );
    if ( it1 == linkersv.end() ) {
        return false;
    }
    // Removing the information in outLinks
    Cluster* c = l->getFrom();
    LinkerVec::iterator it2 = std::find( outLinks[c].begin(), outLinks[c].end(), l );
    outLinks[c].erase( it2 );
    if ( outLinks[c].size() == 0 ) {
        outLinks.erase( c );
    }
    // Removing the information in inLinks
    c = l->getTo();
    it2 = std::find( inLinks[c].begin(), inLinks[c].end(), l );
    inLinks[c].erase( it2 );
    if ( inLinks[c].size() == 0 ) {
        inLinks.erase( c );
    }
    // Remove the Linker
    linkersv.erase( it1 );
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
    for ( ClusterVec::iterator it = clustersv.begin(); it != clustersv.end(); ++it ) {
        (*it)->randomize( min, max );
    }
    for ( LinkerVec::iterator it2 = linkersv.begin(); it2 != linkersv.end(); ++it2 ) {
        (*it2)->randomize( min, max );
    }
}

Updatable* BaseNeuralNet::getByName( const char* name ) {
    for ( ClusterVec::iterator it = clustersv.begin(); it != clustersv.end(); ++it ) {
        if ( strcmp( (*it)->name(), name ) == 0 ) {
            return *it;
        }
    }
    for ( LinkerVec::iterator it2 = linkersv.begin(); it2 != linkersv.end(); ++it2 ) {
        if ( strcmp( (*it2)->name(), name ) == 0 ) {
            return *it2;
        }
    }
#ifdef NNFW_DEBUG
    nWarning() << "Updatable not present in BaseNeuralNet!!!";
#endif
    return NULL;
}

bool BaseNeuralNet::find( const Cluster* cl ) const {
    ClusterVec::const_iterator it = std::find( clustersv.begin(), clustersv.end(), cl );
    if ( it == clustersv.end() ) {
        return false;
    }
    return true;
}

bool BaseNeuralNet::find( const Linker* l ) const {
    LinkerVec::const_iterator it = std::find( linkersv.begin(), linkersv.end(), l );
    if ( it == linkersv.end() ) {
        return false;
    }
    return true;
}

bool BaseNeuralNet::find( const Updatable* u ) const {
    ClusterVec::const_iterator cit = std::find( clustersv.begin(), clustersv.end(), u );
    if ( cit != clustersv.end() ) {
        return true;
    }
    LinkerVec::const_iterator lit = std::find( linkersv.begin(), linkersv.end(), u );
    if ( lit != linkersv.end() ) {
        return true;
    }
    return false;
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
	for( int i=0; i<order().size(); i++ ) {
		ord << clone->getByName( order()[i]->name() );
	}
	clone->setOrder( ord );
	return clone;
}

}
