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

#include "updatable.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

Updatable::Updatable( const char* name )
    : Propertized() {
    this->name = 0;
    setName( name );
    addProperty( "name", Variant::t_string, this, &Updatable::getNameV, &Updatable::setName );
    // setTypename( "Updatable" ); --- it's no instanciable
}

Updatable::Updatable( PropertySettings& prop )
    : Propertized() {
    this->name = 0;
    setName( prop["name"].getString() );
    addProperty( "name", Variant::t_string, this, &Updatable::getNameV, &Updatable::setName );
    // setTypename( "Updatable" ); --- it's no instanciable
}

Updatable::~Updatable() {
    delete []name;
}

void Updatable::setName( const char* newname ) {
    if (name) {
        delete []name;
    }
    u_int size = strlen(newname);
    name = new char[size+1];
    strcpy( name, newname );
}

bool Updatable::setName( const Variant& nv ) {
    setName( nv.getString() );
    return true;
}

const char* Updatable::getName() {
    return name;
}

Variant Updatable::getNameV() {
    return Variant( name );
}

}