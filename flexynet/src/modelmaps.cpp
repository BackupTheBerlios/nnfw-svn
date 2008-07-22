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

#include "modelmaps.h"
#include "clustermodel.h"
#include "linkermodel.h"
#include "outputfunctionmodel.h"
using namespace nnfw;

bool ModelMaps::inited = false;
ModelMaps* ModelMaps::instancev = 0;
ModelMaps* ModelMaps::instance() {
	if ( !inited ) {
		instancev = new ModelMaps();
		inited = true;
	}
	return instancev;
}

ModelMaps::ModelMaps() : mapcluster(), maplinker(), mapfunction() {
}

void ModelMaps::addEntry( ClusterModel* model, nnfw::Cluster* cl ) {
	if ( mapcluster.contains( cl ) ) {
		return;
	}
	mapcluster[cl] = model;
}

void ModelMaps::addEntry( LinkerModel* model, nnfw::Linker* cl ) {
	if ( maplinker.contains( cl ) ) {
		return;
	}
	maplinker[cl] = model;
}

void ModelMaps::addEntry( OutputFunctionModel* model, nnfw::OutputFunction* cl ) {
	if ( mapfunction.contains( cl ) ) {
		return;
	}
	mapfunction[cl] = model;
}

void ModelMaps::delEntry( nnfw::Cluster* cl ) {
	mapcluster.take( cl );
}

void ModelMaps::delEntry( nnfw::Linker* cl ) {
	maplinker.take( cl );
}

void ModelMaps::delEntry( nnfw::OutputFunction* cl ) {
	mapfunction.take( cl );
}

ClusterModel* ModelMaps::modelOf( nnfw::Cluster* cl ) {
	if ( mapcluster.contains( cl ) ) {
		return mapcluster[cl];
	}
	return 0;
}

LinkerModel* ModelMaps::modelOf( nnfw::Linker* cl ) {
	if ( maplinker.contains( cl ) ) {
		return maplinker[cl];
	}
	return 0;
}

UpdatableModel* ModelMaps::modelOf( nnfw::Updatable* cl ) {
	if ( dynamic_cast<Cluster*>( cl ) ) {
		return modelOf( (Cluster*)cl );
	}
	if ( dynamic_cast<Linker*>( cl ) ) {
		return modelOf( (Linker*)cl );
	}
	return 0;
}

OutputFunctionModel* ModelMaps::modelOf( nnfw::OutputFunction* cl ) {
	if ( mapfunction.contains( cl ) ) {
		return mapfunction[cl];
	}
	return 0;
}
