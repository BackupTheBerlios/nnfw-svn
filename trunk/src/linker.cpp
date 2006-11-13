/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2006 Gianluca Massera <emmegian@yahoo.it>                     *
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
    Variant& v = prop["from"];
    if ( v.isNull() ) {
        nnfwMessage( NNFW_ERROR, "You can't construct a Linker wihout specifying a Cluster From" );
    } else {
        fromc = (Cluster*)( v.getCluster() );
    }
    v = prop["to"];
    if ( v.isNull() ) {
        nnfwMessage( NNFW_ERROR, "You can't construct a Linker wihout specifying a Cluster To" );
    } else {
        toc = (Cluster*)( v.getCluster() );
    }
    addProperty( "from", Variant::t_cluster, this, &Linker::fromP );
    addProperty( "to", Variant::t_cluster, this, &Linker::toP );
    // setTypename( "Linker" ); --- it's no instanciable
}

}

