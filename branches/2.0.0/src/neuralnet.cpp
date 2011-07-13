/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2011 Gianluca Massera <emmegian@yahoo.it>                *
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

namespace nnfw {

NeuralNet::NeuralNet() {
	dimUps = 0;
}

NeuralNet::~NeuralNet() {
}

void NeuralNet::addCluster( Cluster* c, bool isInput, bool isOutput ) {
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

bool NeuralNet::removeCluster( Cluster* c ) {
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

void NeuralNet::markAsInput( Cluster* c ) {
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
	hidclusters.removeOne( c );
}

void NeuralNet::markAsOutput( Cluster* c ) {
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
	hidclusters.removeOne( c );
}

void NeuralNet::unmark( Cluster* c ) {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to addCluster! This operation will be ignored" ;
		return;
	}
#endif
	// Check if the Cluster exists
	if ( !find( c ) ) {
#ifdef NNFW_DEBUG
		qWarning() << "attempt to unmark a Cluster not present in this net!" ;
#endif
		return;
	}
	inclusters.removeOne( c );
	outclusters.removeOne( c );
	hidclusters.append( c );
	return;
}

void NeuralNet::unmarkAll( ) {
	inclusters.clear();
	outclusters.clear();
	hidclusters = clustersv;
	return;
}

bool NeuralNet::isIsolated( Cluster* c ) const {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to isIsolato! This operation will return false" ;
		return false;
	}
#endif
	return ( inLinks.count( c ) == 0 && outLinks.count( c ) == 0 );
}

ClusterList NeuralNet::clusters() const {
	return clustersv;
}

ClusterList NeuralNet::inputClusters() const {
	return inclusters;
}

ClusterList NeuralNet::outputClusters() const {
	return outclusters;
}

ClusterList NeuralNet::hiddenClusters() const {
	return hidclusters;
}

void NeuralNet::addLinker( Linker* l ) {
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
		qWarning() << "The linker that you want add links cluster" << l->from()->name() << "that doesn't exist in this net! This operation will be ignored" ;
		return;
	}
	if ( ! find( l->to() ) ) {
		qWarning() << "The linker that you want add links cluster" << l->to()->name() << "that doesn't exist in this net! This operation will be ignored" ;
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

bool NeuralNet::removeLinker( Linker* l ) {
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

LinkerList NeuralNet::linkers() const {
	return linkersv;
}

LinkerList NeuralNet::linkers( Cluster* c, bool out ) const {
#ifdef NNFW_DEBUG
	if ( !c ) {
		qWarning() << "Null Pointer passed to linkers! This operation will return an empty LinkerList" ;
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

void NeuralNet::setOrder( Updatable* u[], unsigned int dim ) {
	ups.clear();
	for( unsigned int i = 0; i<dim; i++ ) {
		if ( find( u[i] ) ) {
			ups.append( u[i] );
		}
#ifdef NNFW_DEBUG
		else {
			qWarning() << "In the Updatable order list passed there are some Clusters and/or Linkers not present in this NeuralNet";
		}
#endif
	}
	dimUps = ups.size();
	return;
}

void NeuralNet::setOrder( const UpdatableList& u ) {
	ups.clear();
	unsigned int dim = u.size();
	for( unsigned int i = 0; i<dim; i++ ) {
		if ( find( u[i] ) ) {
			ups.append( u[i] );
		}
#ifdef NNFW_DEBUG
		else {
			qWarning() << "In the Updatable order list passed there are some Clusters and/or Linkers not present in this NeuralNet";
		}
#endif
	}
	dimUps = ups.size();
	return;
}

void NeuralNet::randomize( double min, double max ) {
	int dim = clustersv.size();
	for( int i=0; i<dim; i++ ) {
		clustersv[i]->randomize( min, max );
	}
	dim = linkersv.size();
	for( int i=0; i<dim; i++ ) {
		linkersv[i]->randomize( min, max );
	}
}

Updatable* NeuralNet::getByName( QString name ) {
	if ( clsMap.contains( name ) ) {
		return clsMap[name];
	}
	if ( lksMap.contains( name ) ) {
		return lksMap[name];
	}
#ifdef NNFW_DEBUG
	qWarning() << "getByName: Updatable not present in BaseNeuralNet";
#endif
	return NULL;
}

bool NeuralNet::find( const Cluster* cl ) const {
	return clustersv.contains( (Cluster*)cl );
}

bool NeuralNet::find( const Linker* l ) const {
	return linkersv.contains( (Linker*)l );
}

bool NeuralNet::find( const Updatable* u ) const {
	return (
		clustersv.contains( (Cluster*)u ) ||
		linkersv.contains( (Linker*)u )
	);
}

void NeuralNet::configure(ConfigurationParameters& params, QString prefix) {
	params.startRememberingGroupObjectAssociations();
	// the name will correspond to the group where the declaration of NeuralNet is
	QString myname = prefix;
	if ( myname.startsWith( ConfigurationParameters::GroupSeparator ) ) {
		myname.remove( 0, 1 );
	}
	if ( myname.endsWith( ConfigurationParameters::GroupSeparator ) ) {
		myname.chop( 1 );
	}
	setName( myname );
	
	// All Cluster has to be added before the creation of linkers, that's why the insertion is done after
	ClusterList clsToAdd;
	ClusterList clsInput;
	ClusterList clsOutput;
	LinkerList lnsToAdd;

	//--- get all subgroups, merge this list with clustersList and linkersList
	QStringList subgroups;
	foreach( QString sub, params.getGroupsList( prefix ) ) {
		// append the full path
		subgroups << (prefix+sub);
	}
	//--- clustersList and linkersList are supposed to be full path
	QString str = params.getValue(prefix + "clustersList");
	if (!str.isNull()) {
		subgroups << str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	}
	str = params.getValue(prefix + "linkersList");
	if (!str.isNull()) {
		subgroups << str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
	}
	foreach( QString sub, subgroups ) {
		Updatable* up = params.getObjectFromGroup<Updatable>( sub, true );
		//--- check if is a Cluster
		Cluster* cl = dynamic_cast<Cluster*>(up);
		if ( cl && !clsToAdd.contains( cl ) ) clsToAdd << cl;
		//--- check if is a Linker
		Linker* ln = dynamic_cast<Linker*>(up);
		if ( ln && !lnsToAdd.contains( ln ) ) lnsToAdd << ln;
	}
	//--- parse the parameter inputClusters
	str = params.getValue(prefix + "inputClusters");
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		foreach( QString sub, list ) {
			Cluster* cl = params.getObjectFromGroup<Cluster>( sub, true );
			if ( !clsToAdd.contains( cl ) ) clsToAdd << cl;
			clsInput << cl;
		}
	}
	//--- parse the parameter outputClusters
	str = params.getValue(prefix + "outputClusters");
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		foreach( QString sub, list ) {
			Cluster* cl = params.getObjectFromGroup<Cluster>( sub, true );
			if ( !clsToAdd.contains( cl ) ) clsToAdd << cl;
			clsOutput << cl;
		}
	}
	//--- parse the parameter spreadOrder
	str = params.getValue(prefix + "spreadOrder");
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		ups.clear();
		foreach( QString sub, list ) {
			Updatable* up = params.getObjectFromGroup<Updatable>( sub, true );
			// add it to the list of objects to add
			Cluster* cl = dynamic_cast<Cluster*>(up);
			if ( cl && !clsToAdd.contains( cl ) ) clsToAdd << cl;
			//--- check if is a Linker
			Linker* ln = dynamic_cast<Linker*>(up);
			if ( ln && !lnsToAdd.contains( ln ) ) { qDebug() << ln->name(); lnsToAdd << ln; }
			ups.append( up );
		}
		dimUps = ups.size();
	}
	foreach( Cluster* cl, clsToAdd ) {
		addCluster( cl );
	}
	foreach( Cluster* cl, clsInput ) {
		markAsInput( cl );
	}
	foreach( Cluster* cl, clsOutput ) {
		markAsOutput( cl );
	}
	foreach( Linker* ln, lnsToAdd ) {
		addLinker( ln );
	}
	params.stopRememberingGroupObjectAssociations();
}

void NeuralNet::save(ConfigurationParameters& params, QString prefix) {
	params.startObjectParameters( prefix, "NeuralNet", this );
	//--- save all Clusters and generate parameter clustersList
	QStringList list;
	list.clear();
	foreach( Cluster* cl, clustersv ) {
		params.createGroup( cl->name() );
		cl->save( params, cl->name() );
		list << cl->name();
	}
	params.createParameter( prefix, "clustersList", list.join(" ") );
	//--- save all Linkers and generate parameter linkersList
	list.clear();
	foreach( Linker* ln, linkersv ) {
		params.createGroup( ln->name() );
		ln->save( params, ln->name() );
		list << ln->name();
	}
	params.createParameter( prefix, "linkersList", list.join(" ") );
	//--- save parameter inputClusters
	list.clear();
	foreach( Cluster* cl, inclusters ) {
		list << cl->name();
	}
	params.createParameter( prefix, "inputClusters", list.join(" ") );
	//--- save parameter outputClusters
	list.clear();
	foreach( Cluster* cl, outclusters ) {
		list << cl->name();
	}
	params.createParameter( prefix, "outputClusters", list.join(" ") );
	//--- save parameter spreadOrder
	list.clear();
	foreach( Updatable* up, ups ) {
		list << up->name();
	}
	params.createParameter( prefix, "spreadOrder", list.join(" ") );
}

}
