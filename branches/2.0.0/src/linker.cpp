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

#include "linker.h"
#include "neuralnet.h"

namespace nnfw {

Linker::Linker( Cluster* from, Cluster* to, const char* name )
    : Updatable(name) {
    this->fromc = from;
    this->toc = to;
    addProperty( "from", Variant::t_cluster, this, &Linker::fromP );
    addProperty( "to", Variant::t_cluster, this, &Linker::toP );
    // setTypename( "Linker" ); --- it's no instanciable
}

Linker::Linker( PropertySettings& prop )
    : Updatable( prop ) {
	Variant& v = prop["baseneuralnet"];
	if ( v.isNull() ) {
		//--- suppose you pass directly Cluster pointers into "from" and "to"
		fromc = prop["from"].getCluster();
		toc = prop["to"].getCluster();
		addProperty( "from", Variant::t_cluster, this, &Linker::fromP );
		addProperty( "to", Variant::t_cluster, this, &Linker::toP );
		// setTypename( "Linker" ); --- it's no instanciable
		return;
	}
	BaseNeuralNet* net = v.getDataPtr<BaseNeuralNet>();
	//--- suppose you pass the name of Clusters "from" and "to"
	//--- it will check if they exists on the "baseneuralnet" passed
	v = prop["from"];
    if ( v.isNull() ) {
        nFatal() << "You can't construct a Linker wihout specifying a Cluster From";
		exit(1);
    } else {
		fromc = (Cluster*)( net->getByName( v.getString() ) );
    	if ( !fromc ) {
        	nFatal() << "the 'from' Cluster doesn't exist; impossible create linker " << name();
        	exit(1);
		}
    }
    v = prop["to"];
    if ( v.isNull() ) {
        nFatal() << "You can't construct a Linker wihout specifying a Cluster To" ;
		exit(1);
    } else {
		toc = (Cluster*)( net->getByName( v.getString() ) );
    	if ( !toc ) {
        	nFatal() << "the 'to' Cluster doesn't exist; impossible create linker " << name();
        	exit(1);
		}
    }
    addProperty( "from", Variant::t_cluster, this, &Linker::fromP );
    addProperty( "to", Variant::t_cluster, this, &Linker::toP );
    // setTypename( "Linker" ); --- it's no instanciable
}

Linker* Linker::clone() const {
	nError() << "The clone() method has to implemented by subclasses";
	return 0;
}

}

