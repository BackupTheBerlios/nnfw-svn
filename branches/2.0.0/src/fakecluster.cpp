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

#include "fakecluster.h"
#include "liboutputfunctions.h"

namespace nnfw {

FakeCluster::FakeCluster( unsigned int size, QString name )
	: Cluster( size, name) {
	// Outputs will point to same data of Inputs
	outputdataptr = inputdataptr;
}

FakeCluster::FakeCluster( ConfigurationParameters& params, QString prefix )
	: Cluster( params, prefix ) {
	// there is no extra parameters to configure
}

FakeCluster::~FakeCluster() {
}

void FakeCluster::update() {
	setNeedReset( true );
	return;
}

void FakeCluster::randomize( double , double ) {
	return;
}

void FakeCluster::save(ConfigurationParameters& params, QString prefix) {
	Cluster::save( params, prefix );
	params.startObjectParameters( prefix, "FakeCluster", this );
	// there is no extra parameters to save
}

}
