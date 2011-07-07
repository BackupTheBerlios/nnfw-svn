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

#include "simplecluster.h"
#include "outputfunction.h"

namespace nnfw {

SimpleCluster::SimpleCluster( unsigned int numNeurons, QString name )
	: Cluster( numNeurons, name) {
}

SimpleCluster::SimpleCluster( ConfigurationParameters& params, QString prefix )
	: Cluster( params, prefix ) {
	// there is no extra parameters to configure
}

SimpleCluster::~SimpleCluster() {
}

void SimpleCluster::update() {
	outFunction()->apply( inputs(), outputs() );
	setNeedReset( true );
}

void SimpleCluster::save(ConfigurationParameters& params, QString prefix) {
	Cluster::save( params, prefix );
	params.startObjectParameters( prefix, "SimpleCluster", this );
	// there is no extra parameters to save
}

}
