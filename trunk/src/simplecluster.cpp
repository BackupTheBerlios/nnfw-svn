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

#include "simplecluster.h"
#include "liboutputfunctions.h"
#include "random.h"
#include <cstdio>
#include <cstring>


namespace nnfw {

/**********************************************
 *  Implementation of SimpleCluster Class     *
 **********************************************/

SimpleCluster::SimpleCluster( u_int numNeurons, const char* name )
    : Cluster( numNeurons, name) {
    setTypename( "SimpleCluster" );
}

SimpleCluster::SimpleCluster( PropertySettings& prop )
    : Cluster( prop ) {
    setTypename( "SimpleCluster" );
}

SimpleCluster::~SimpleCluster() {
}

void SimpleCluster::update() {
    getFunction()->apply( inputs(), outputs() );
    setNeedReset( true );
}

SimpleCluster* SimpleCluster::clone() const {
	SimpleCluster* newclone = new SimpleCluster( numNeurons(), name() );
	newclone->setAccumulate( this->isAccumulate() );
	newclone->inputs().assign( this->inputs() );
	newclone->outputs().assign( this->outputs() );
	newclone->setFunction( *(this->getFunction()) );
	return newclone;
}

}
