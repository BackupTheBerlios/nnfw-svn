/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>                *
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

#warning THERE ARE WARNINGS HERE ABOUT RETURNING REFERENCES TO TEMPORARIES (BOTH BECAUSE LinkerList() IS RETURNED AND BECAUSE THE const VERSION OF operator[] OF QMap RETURNS A COPY INSTEAD OF A CONST REFERENCE). TALK WITH GIANLUCA ABOUT WHAT TO DO HERE.

namespace nnfw {

BaseNeuralNet::BaseNeuralNet() {
	dimUps = 0;
}

BaseNeuralNet::~BaseNeuralNet() {
}

void BaseNeuralNet::addCluster( Cluster* c, bool isInput, bool isOutput ) {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to addCluster! This operation will be ignored" ;
		return;
	}
#endif
	// Check if the Cluster is already added
	if ( find( c ) ) {
#ifdef NNFW_DEBUG
		qWarning() << "Cluster already added! addCluster will be ignored" ;
#endif
		return;
	}
	clustersv.append( c );
	if ( isInput ) {
		inclusters.append( c );
	}
	if ( isOutput ) {
		outclusters.append( c );
	}
	if ( !isInput && !isOutput ) {
		hidclusters.append( c );
	}
	clsMap[c->name()] = c;
	return;
}

bool BaseNeuralNet::removeCluster( Cluster* c ) {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to removeCluster! This operation will return false" ;
		return false;
	}
#endif
	if ( !find( c ) ) {
		return false;
	}
	clustersv.removeOne( c );
	inclusters.removeOne( c );
	outclusters.removeOne( c );
	hidclusters.removeOne( c );
	clsMap.remove( c->name() );
	return true;
}

void BaseNeuralNet::markAsInput( Cluster* c ) {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to addCluster! This operation will be ignored" ;
		return;
	}
#endif
	// Check if the Cluster exists
	if ( !find( c ) ) {
#ifdef NNFW_DEBUG
		qWarning() << "attempt to mark a Cluster not present in this net!" ;
#endif
		return;
	}
	if ( inclusters.contains( c ) ) {
		return;
	}
	inclusters.append( c );
}

void BaseNeuralNet::markAsOutput( Cluster* c ) {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to addCluster! This operation will be ignored" ;
		return;
	}
#endif
	// Check if the Cluster exists
	if ( !find( c ) ) {
#ifdef NNFW_DEBUG
		qWarning() << "attempt to mark a Cluster not present in this net!" ;
#endif
		return;
	}
	if ( outclusters.contains( c ) ) {
		return;
	}
	outclusters.append( c );
}

void BaseNeuralNet::unmark( Cluster* c ) {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to addCluster! This operation will be ignored" ;
		return;
	}
#endif
	// Check if the Cluster exists
	if ( !find( c ) ) {
		return;
	}
	inclusters.removeOne( c );
	outclusters.removeOne( c );
	hidclusters.append( c );
	return;
}

void BaseNeuralNet::unmarkAll( ) {
	inclusters.clear();
	outclusters.clear();
	hidclusters = clustersv;
	return;
}

bool BaseNeuralNet::isIsolated( Cluster* c ) const {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to isIsolato! This operation will return false" ;
		return false;
	}
#endif
	return ( inLinks.count( c ) == 0 && outLinks.count( c ) == 0 );
}

const ClusterList& BaseNeuralNet::clusters() const {
	return clustersv;
}

const ClusterList& BaseNeuralNet::inputClusters() const {
	return inclusters;
}

const ClusterList& BaseNeuralNet::outputClusters() const {
	return outclusters;
}

const ClusterList& BaseNeuralNet::hiddenClusters() const {
	return hidclusters;
}

void BaseNeuralNet::addLinker( Linker* l ) {
#ifdef NNFW_DEBUG
	if ( !l ) {
		qWarning() << "Null Pointer passed to addLinker! This operation will be ignored" ;
		return;
	}
#endif
	// Check if the Linker is already added
	if ( find( l ) ) {
#ifdef NNFW_DEBUG
		qWarning() << "Linker already added! addLinker will be ignored" ;
#endif
		return;
	}
#ifdef NNFW_DEBUG
	// --- Check: Are There in this net the Clusters that linker l connects ???
	if ( ! find( l->from() ) ) {
		qWarning() << "The linker that you want add links clusters that doesn't exist in this net! \
									This operation will be ignored" ;
		return;
	}
	if ( ! find( l->to() ) ) {
		qWarning() << "The linker that you want add links clusters that doesn't exist in this net! \
									This operation will be ignored" ;
		return;
	}
#endif
	linkersv.append( l );
	// Adding information in outLinks map
	outLinks[ l->from() ].append( l );
	// Adding information in inLinks map
	inLinks[ l->to() ].append( l );
	lksMap[l->name()] = l;
	return;
}

bool BaseNeuralNet::removeLinker( Linker* l ) {
#ifdef NNFW_DEBUG
	if ( !l ) {
		qWarning() << "Null Pointer passed to removeLinker! This operation will return false" ;
		return false;
	}
#endif
	if ( !find(l) ) {
		return false;
	}
	linkersv.removeOne( l );
	outLinks[ l->from() ].removeOne( l );
	inLinks[ l->to() ].removeOne( l );
	lksMap.remove( l->name() );
	return true;
}

const LinkerList& BaseNeuralNet::linkers() const {
	return linkersv;
}

const LinkerList& BaseNeuralNet::linkers( Cluster* c, bool out ) const {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to linkers! This operation will return an empty LinkerGroup" ;
		return LinkerList();
	}
#endif
	if ( out ) {
		// Return outgoing linkers
		if ( outLinks.contains( c ) ) {
			return outLinks[c];
		}
	} else {
		// Return incoming linkers
		if ( inLinks.contains( c ) ) {
			return inLinks[c];
		}
	}
	return LinkerList();
}

void BaseNeuralNet::setOrder( Updatable* u[], unsigned int dim ) {
	ups.clear();
	for( unsigned int i = 0; i<dim; i++ ) {
		if ( find( u[i] ) ) {
			ups.append( u[i] );
		}
	}
	dimUps = ups.size();
	return;
}

void BaseNeuralNet::setOrder( const UpdatableList& u ) {
	ups.clear();
	unsigned int dim = u.size();
	for( unsigned int i = 0; i<dim; i++ ) {
		if ( find( u[i] ) ) {
			ups.append( u[i] );
		}
	}
	dimUps = ups.size();
	return;
}

void BaseNeuralNet::randomize( double min, double max ) {
	int dim = clustersv.size();
	for( int i=0; i<dim; i++ ) {
		clustersv[i]->randomize( min, max );
	}
	dim = linkersv.size();
	for( int i=0; i<dim; i++ ) {
		linkersv[i]->randomize( min, max );
	}
}

Updatable* BaseNeuralNet::getByName( QString name ) {
	if ( clsMap.contains( name ) ) {
		return clsMap[name];
	}
	if ( lksMap.find( name ) != lksMap.end() ) {
		return lksMap[name];
	}
#ifdef NNFW_DEBUG
	qWarning() << "Updatable not present in BaseNeuralNet!!!";
#endif
	return NULL;
}

bool BaseNeuralNet::find( const Cluster* cl ) const {
	return clustersv.contains( (Cluster*)cl );
}

bool BaseNeuralNet::find( const Linker* l ) const {
	return linkersv.contains( (Linker*)l );
}

bool BaseNeuralNet::find( const Updatable* u ) const {
	return (
		clustersv.contains( (Cluster*)u ) ||
		linkersv.contains( (Linker*)u )
	);
}

}
