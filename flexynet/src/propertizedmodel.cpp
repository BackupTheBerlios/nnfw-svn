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

#include "propertizedmodel.h"
using namespace nnfw;

PropertyAccessModel::PropertyAccessModel( nnfw::AbstractPropertyAccess* prop, PropertizedModel* parent ) {
	this->prop = prop;
	this->parent = parent;
}

bool PropertyAccessModel::set( const nnfw::Variant& data ) {
	if ( prop->isWritable() && !prop->isVector() ) {
		bool r = prop->set( data );
		if ( r ) {
			emit changeValue( data );
		}
		return r;
	} else {
		// --- a messagebox for notify the error to the user ?!?!
		return false;
	}
}

nnfw::Variant PropertyAccessModel::get() const {
	if ( !prop->isVector() ) {
		return prop->get();
	} else {
		// --- a messagebox for notify the error to the user ?!?!
		return Variant();
	}
}

bool PropertyAccessModel::set( unsigned int i, const Variant& data ) {
	if ( prop->isWritable() && prop->isVector() ) {
		bool r = prop->set( i, data );
		if ( r ) {
			emit changeValue( i, data );
		}
		return r;
	} else {
		// --- a messagebox for notify the error to the user ?!?!
		return false;
	}
}

nnfw::Variant PropertyAccessModel::get( unsigned int i ) const {
	if ( prop->isVector() ) {
		return prop->get(i);
	} else {
		// --- a messagebox for notify the error to the user ?!?!
		return Variant();
	}
}

QString PropertyAccessModel::name() const {
	return prop->name();
}

bool PropertyAccessModel::isWritable() const {
	return prop->isWritable();
}

bool PropertyAccessModel::isVector() const {
	return prop->isVector();
}

nnfw::Variant::types PropertyAccessModel::type() const {
	return prop->type();
}

PropertizedModel* PropertyAccessModel::object() {
	return parent;
}

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

PropertizedModel::PropertizedModel( nnfw::Propertized* proped ) : props() {
	this->proped = proped;
	PropertyAccessVec& ps = proped->properties();
	for( int i=0; i<(int)ps.size(); i++ ) {
		props.append( new PropertyAccessModel( ps[i], this ) );
	}
}

QString PropertizedModel::getTypename() {
	return proped->getTypename().getString();
}

nnfw::Variant PropertizedModel::nnProperty( QString name ) {
	return proped->property( name.toAscii().data() );
}

bool PropertizedModel::nnSetProperty( QString name, const nnfw::Variant& data ) {
	bool r = proped->setProperty( name.toAscii().data(), data );
	if ( r ) {
		emit changeValue( name, data );
	}
	return r;
}

bool PropertizedModel::nnSetVectorProperty( QString name, unsigned int i, const nnfw::Variant& data ) {
	bool r = proped->setVectorProperty( name.toAscii().data(), i, data );
	if ( r ) {
		emit changeValue( name, i, data );
	}
	return r;
}

QVector<PropertyAccessModel*> PropertizedModel::nnProperties() {
	return props;
}

