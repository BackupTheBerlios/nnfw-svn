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

Updatable::Updatable( QString name )
	: ParameterSettableInConstructor()
{
	setName( name );
}

Updatable::Updatable( ConfigurationParameters& params, QString prefix ) :
	ParameterSettableInConstructor(params, prefix)
{
	QString upname = prefix;
	// remove from the name the GroupSeparator at the start and at the end
	// because they create problem when referencing the object using NeuralNet::byName
	if ( upname.startsWith( ConfigurationParameters::GroupSeparator ) ) {
		upname.remove( 0, 1 );
	}
	if ( upname.endsWith( ConfigurationParameters::GroupSeparator ) ) {
		upname.chop( 1 );
	}
	setName( upname );
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
