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

#include "neuralnet.h"
#include "messages.h"
#include <algorithm>
#include <functional>
#include <cstring>

//! Namespace that contains all classes of Neural Network Framework
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
    if ( !c ) {
        nnfwMessage( NNFW_ERROR, "Null Pointer passed to addCluster! This operation will be ignored" );
        return;
    }
    // Check if the Cluster is already added
    if ( find( c ) ) {
        nnfwMessage( NNFW_ERROR, "Cluster already added! addCluster will be ignored" );
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
    if ( !c ) {
        nnfwMessage( NNFW_ERROR, "Null Pointer passed to addCluster! This operation will return false" );
        return false;
    }
    unmark( c );
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
    if ( !c ) {
        nnfwMessage( NNFW_ERROR, "Null Pointer passed to addCluster! This operation will be ignored" );
        return;
    }
    // Check if the Cluster is already added
    if ( !find( c ) ) {
        nnfwMessage( NNFW_ERROR, "attempt to mark a Cluster not present in this net!" );
        return;
    }
    ClusterVec::iterator it = std::find( inclusters.begin(), inclusters.end(), c );
    if ( it != inclusters.end() ) {
        // --- already marked as Input
        return;
    }
    inclusters.push_back( c );
}

void BaseNeuralNet::markAsOutput( Cluster* c ) {
    if ( !c ) {
        nnfwMessage( NNFW_ERROR, "Null Pointer passed to addCluster! This operation will be ignored" );
        return;
    }
    // Check if the Cluster is already added
    if ( !find( c ) ) {
        nnfwMessage( NNFW_ERROR, "attempt to mark a Cluster not present in this net!" );
        return;
    }
    ClusterVec::iterator it = std::find( outclusters.begin(), outclusters.end(), c );
    if ( it != outclusters.end() ) {
        // --- already marked as Output
        return;
    }
    outclusters.push_back( c );
}

void BaseNeuralNet::unmark( Cluster* c ) {
    if ( !c ) {
        nnfwMessage( NNFW_ERROR, "Null Pointer passed to addCluster! This operation will be ignored" );
        return;
    }
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
    if ( !c ) {
        nnfwMessage( NNFW_ERROR, "Null Pointer passed to isIsolato! This operation will return false" );
        return false;
    }
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
    if ( !l ) {
        nnfwMessage( NNFW_ERROR, "Null Pointer passed to addLinker! This operation will be ignored" );
        return;
    }
    // Check if the Linker is already added
    if ( find( l ) ) {
        nnfwMessage( NNFW_ERROR, "Linker already added! addLinker will be ignored" );
        return;
    }
    // --- Check: Are There in this net the Clusters that linker l connects ???
    if ( ! find( l->getFrom() ) ) {
        nnfwMessage( NNFW_ERROR, "The linker that you want add links clusters that doesn't exist in this net! \
                                  This operation will be ignored" );
        return;
    }
    if ( ! find( l->getTo() ) ) {
        nnfwMessage( NNFW_ERROR, "The linker that you want add links clusters that doesn't exist in this net! \
                                  This operation will be ignored" );
        return;
    }
    linkersv.push_back( l );

    // Adding information in outLinks map
    outLinks[ l->getFrom() ].push_back( l );
    // Adding information in inLinks map
    inLinks[ l->getTo() ].push_back( l );
    return;
}

bool BaseNeuralNet::removeLinker( Linker* l ) {
    if ( !l ) {
        nnfwMessage( NNFW_ERROR, "Null Pointer passed to removeLinker! This operation will return false" );
        return false;
    }
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
    if ( !c ) {
        nnfwMessage( NNFW_ERROR, "Null Pointer passed to linkers! This operation will return an empty LinkerGroup" );
        return emptyLinkerVec;
    }
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

void BaseNeuralNet::setOrder( UpdatableVec& u ) {
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
        if ( strcmp( (*it)->getName(), name ) == 0 ) {
            return *it;
        }
    }
    for ( LinkerVec::iterator it2 = linkersv.begin(); it2 != linkersv.end(); ++it2 ) {
        if ( strcmp( (*it2)->getName(), name ) == 0 ) {
            return *it2;
        }
    }
    nnfwMessage( NNFW_WARNING, "Updatable not present in BaseNeuralNet!!!" );
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

}
