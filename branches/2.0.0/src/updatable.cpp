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

#include "updatable.h"

namespace nnfw {

#warning Ho Dovuto mettere una variabile statica relativo ad un configurationParameters vuoto perché il construttore non mi accetta ParameterSettableInConstructor(ConfigurationParameters(), "") e da errore
static ConfigurationParameters dummyParams;
Updatable::Updatable( QString name )
//	: ParameterSettableInConstructor(ConfigurationParameters(), QString(""))
	: ParameterSettableInConstructor(dummyParams, "")
{
	setName( name );
}

Updatable::Updatable( ConfigurationParameters& params, QString prefix ) :
	ParameterSettableInConstructor(params, prefix)
{
	setName( prefix );
}

Updatable::~Updatable() {
}

void Updatable::setName( QString newname ) {
	namev = newname;
}

QString Updatable::name() const {
	return namev;
}

void Updatable::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "Updatable", this);
}

}
