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

#include "linkermodel.h"
#include "clustermodel.h"
#include "modelmaps.h"
using namespace nnfw;

LinkerModel::LinkerModel( nnfw::Linker* link )
	: UpdatableModel(link) {
	ModelMaps* maps = ModelMaps::instance();
	if ( maps->modelOf( link ) ) {
		//--- errore molto grave
		qFatal( "Is not allowed to create more that one model for the same Linker" );
	}
	maps->addEntry( this, link );
	linker = link;
	fromm = maps->modelOf( linker->from() );
	if ( !fromm ) {
		fromm = new ClusterModel( linker->from() );
	}
	tom = maps->modelOf( linker->to() );
	if ( !tom ) {
		tom = new ClusterModel( linker->to() );
	}
}

LinkerModel::~LinkerModel() {
	ModelMaps::instance()->delEntry( linker );
	delete linker;
}

nnfw::Linker* LinkerModel::nnLinker() {
	return linker;
}

ClusterModel* LinkerModel::from() {
	return fromm;
}

ClusterModel* LinkerModel::to() {
	return tom;
}

void LinkerModel::randomize( nnfw::Real min, nnfw::Real max ) {
	linker->randomize( min, max );
	emit randomized();
}

LinkerModel* LinkerModel::clone() {
	return new LinkerModel( linker->clone() );
}
