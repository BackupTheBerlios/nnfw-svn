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

#include "clustermodel.h"
#include "outputfunctionmodel.h"
#include "modelmaps.h"
using namespace nnfw;

ClusterModel::ClusterModel( nnfw::Cluster* cl )
	: UpdatableModel( cl ) {
	ModelMaps* maps = ModelMaps::instance();
	if ( maps->modelOf( cl ) ) {
		//--- errore molto grave
		qFatal( "Is not allowed to create more that one model for the same Cluster" );
	}
	maps->addEntry( this, cl );
	cluster = cl;
	outfunc = maps->modelOf( cl->getFunction() );
	if ( !outfunc ) {
		outfunc = new OutputFunctionModel( cl->getFunction() );
	} 
}

ClusterModel::~ClusterModel() {
	ModelMaps::instance()->delEntry( cluster );
	delete cluster;
}

int ClusterModel::numNeurons() {
	return cluster->numNeurons();
}

bool ClusterModel::isAccumulate() {
	return cluster->isAccumulate();
}

const RealVec& ClusterModel::inputs() {
	return cluster->inputs();
}

const RealVec& ClusterModel::outputs() {
	return cluster->outputs();
}

OutputFunctionModel* ClusterModel::getFunction() {
	return outfunc;
}

nnfw::Cluster* ClusterModel::nnCluster() {
	return cluster;
}

void ClusterModel::randomize( nnfw::Real min, nnfw::Real max ) {
	cluster->randomize( min, max );
	emit randomized();
}

void ClusterModel::setAccumulate( bool b ) {
	cluster->setAccumulate( b );
	emit changedAccumulate( b );
}

void ClusterModel::setInput( int i, nnfw::Real value ) {
	cluster->setInput( i, value );
	emit changedInput( i, value );
}

void ClusterModel::setInputs( const nnfw::RealVec& values ) {
	cluster->setInputs( values );
	emit changedInputs( values );
}

void ClusterModel::resetInputs() {
	cluster->resetInputs();
	emit resetted();
}

void ClusterModel::setOutput( int i, nnfw::Real value ) {
	cluster->setOutput( i, value );
	emit changedOutput( i, value );
}

void ClusterModel::setOutputs( const nnfw::RealVec& values ) {
	cluster->setOutputs( values );
	emit changedOutputs( values );
}

void ClusterModel::setFunction( OutputFunctionModel* func ) {
	delete outfunc;
	cluster->setFunction( *(func->nnFunction()) );
	outfunc = ModelMaps::instance()->modelOf( cluster->getFunction() );
	if ( !outfunc ) {
		outfunc = new OutputFunctionModel( cluster->getFunction() );
	}
	emit changedFunction( outfunc );
}

ClusterModel* ClusterModel::clone() {
	return new ClusterModel( cluster->clone() );
}
