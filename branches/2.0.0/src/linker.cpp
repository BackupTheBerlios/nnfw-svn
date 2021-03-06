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

#include "linker.h"
#include "neuralnet.h"

namespace nnfw {

Linker::Linker( Cluster* from, Cluster* to, QString name )
	: Updatable(name) {
	this->fromc = from;
	this->toc = to;
}

Linker::Linker( ConfigurationParameters& params, QString prefix )
	: Updatable( params, prefix ) {
	fromc = params.getObjectFromParameter<Cluster>( prefix + "from", true );
	toc = params.getObjectFromParameter<Cluster>( prefix + "to", true );
	if ( !fromc || !toc ) throw ClusterFromOrToMissing();
}

void Linker::save(ConfigurationParameters& params, QString prefix)
{
	Updatable::save( params, prefix );
	params.startObjectParameters(prefix, "Linker", this);
	params.createParameter(prefix, "from", fromc );
	params.createParameter(prefix, "to", toc );
}

}

