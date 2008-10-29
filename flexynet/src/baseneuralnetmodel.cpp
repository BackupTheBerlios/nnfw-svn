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

#include "baseneuralnetmodel.h"
#include "clustermodel.h"
#include "linkermodel.h"
#include "updatablemodel.h"
#include "modelmaps.h"
#include "nnfw/neuralnet.h"
#include "nnfw/ionnfw.h"
using namespace nnfw;

BaseNeuralNetModel::BaseNeuralNetModel() {
	net = new BaseNeuralNet();
	ischanged = false;
}

BaseNeuralNetModel::BaseNeuralNetModel( QString filename ) {
	net = loadXML( filename.toAscii().data() );
	ischanged = false;
}

bool BaseNeuralNetModel::saveTo( QString filename ) {
	if ( saveXML( filename.toAscii().data(), net ) ) {
		ischanged = false;
		return true;
	} else {
		//--- error on saving
		return false;
	}
	return false;
}

bool BaseNeuralNetModel::hasChanges() {
	return ischanged;
}

void BaseNeuralNetModel::addCluster( ClusterModel* c, bool isInput, bool isOutput ) {
	net->addCluster( c->nnCluster(), isInput, isOutput );
	emit clusterAdded( c, isInput, isOutput );
}

bool BaseNeuralNetModel::removeCluster( ClusterModel* c ) {
	bool ret = net->removeCluster( c->nnCluster() );
	emit clusterRemoved( c );
	return ret;
}

void BaseNeuralNetModel::markAsInput( ClusterModel* c ) {
	net->markAsInput( c->nnCluster() );
	emit markedAsInput( c );
}

void BaseNeuralNetModel::markAsOutput( ClusterModel* c ) {
	net->markAsOutput( c->nnCluster() );
	emit markedAsOutput( c );
}

void BaseNeuralNetModel::unmark( ClusterModel* c ) {
	net->unmark( c->nnCluster() );
	emit unmarked( c );
}

void BaseNeuralNetModel::unmarkAll() {
	net->unmarkAll();
	emit unmarkedAll();
}

bool BaseNeuralNetModel::isIsolated( ClusterModel* c ) const {
	return net->isIsolated( c->nnCluster() );
}

QVector<ClusterModel*> BaseNeuralNetModel::clusters() const {
	QVector<ClusterModel*> cls;
	ModelMaps* maps = ModelMaps::instance();
	for( unsigned int i=0; i<net->clusters().size(); i++ ) {
		cls << maps->modelOf( net->clusters()[i] );
	}
	return cls;
}

QVector<ClusterModel*> BaseNeuralNetModel::inputClusters() const {
	QVector<ClusterModel*> cls;
	ModelMaps* maps = ModelMaps::instance();
	for( unsigned int i=0; i<net->inputClusters().size(); i++ ) {
		cls << maps->modelOf( net->inputClusters()[i] );
	}
	return cls;
}

QVector<ClusterModel*> BaseNeuralNetModel::outputClusters() const {
	QVector<ClusterModel*> cls;
	ModelMaps* maps = ModelMaps::instance();
	for( unsigned int i=0; i<net->outputClusters().size(); i++ ) {
		cls << maps->modelOf( net->outputClusters()[i] );
	}
	return cls;
}

void BaseNeuralNetModel::addLinker( LinkerModel* l ) {
	net->addLinker( l->nnLinker() );
	emit linkerAdded( l );
}

bool BaseNeuralNetModel::removeLinker( LinkerModel* l ) {
	bool ret = net->removeLinker( l->nnLinker() );
	emit linkerRemoved( l );
	return ret;
}

QVector<LinkerModel*> BaseNeuralNetModel::linkers() const {
	QVector<LinkerModel*> ls;
	ModelMaps* maps = ModelMaps::instance();
	for( unsigned int i=0; i<net->linkers().size(); i++ ) {
		ls << maps->modelOf( net->linkers()[i] );
	}
	return ls;
}

QVector<LinkerModel*> BaseNeuralNetModel::linkers( ClusterModel* c, bool out ) const {
	const LinkerVec& nls = net->linkers( c->nnCluster(), out );
	QVector<LinkerModel*> ls;
	ModelMaps* maps = ModelMaps::instance();
	for( unsigned int i=0; i<nls.size(); i++ ) {
		ls << maps->modelOf( nls[i] );
	}
	return ls;
}

void BaseNeuralNetModel::setOrder( QStringList order ) {
	UpdatableVec ups;
	QStringList order2;
	foreach( QString up, order ) {
		Updatable* nup = net->getByName( up.toAscii().data() );
		if ( nup == 0 ) {
			//--- error
		} else {
			ups << nup;
			order2 << up;
		}
	}
	net->setOrder( ups );
	emit orderChanged( order2 );
}

QStringList BaseNeuralNetModel::order() const {
	QStringList ord;
	for( unsigned int i=0; i<net->order().size(); i++ ) {
		ord << QString( net->order()[i]->name() );
	}
	return ord;
}

void BaseNeuralNetModel::step() {
	net->step();
	emit stepped();
}

void BaseNeuralNetModel::randomize( nnfw::Real min, nnfw::Real max ) {
	net->randomize( min, max );
	emit randomized();
}

UpdatableModel* BaseNeuralNetModel::getByName( QString name ) {
	Updatable* nup = net->getByName( name.toAscii().data() );
	if ( nup ) {
		return ModelMaps::instance()->modelOf( nup );
	}
	return 0;
}

bool BaseNeuralNetModel::find( UpdatableModel* c) const {
	return net->find( c->nnUpdatable() );
}
