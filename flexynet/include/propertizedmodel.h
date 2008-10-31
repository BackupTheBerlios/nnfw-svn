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

#ifndef PROPERTIZEDMODEL_H
#define PROPERTIZEDMODEL_H
#include "types.h"

#include "nnfw/propertized.h"
#include <QObject>
#include <QString>
#include <QVector>

class PropertizedModel;

/*! \brief Model of AbstractPropertyAccess and its hierachy classes
 *
 * \par Description
 * \par Warnings
 */
class FLEX_API PropertyAccessModel : public QObject {
	Q_OBJECT
public:
	/*! create a PropertyAccessModel */
	PropertyAccessModel( nnfw::AbstractPropertyAccess* prop, PropertizedModel* parent );
	/*! set the property */
	bool set( const nnfw::Variant& data );
	/*! Return the value of property */
	nnfw::Variant get() const;
	/*! Set the i-th value of Vector property */
	bool set( unsigned int i, const nnfw::Variant& data );
	/*! Return the i-th value of Vector property */
	nnfw::Variant get( unsigned int i ) const;
	/*! Return the name of property */
	QString name() const;
	/*! Return true if the property is writable */
	bool isWritable() const;
	/*! Return true if the property is a Vector value */
	bool isVector() const;
	/*! Return the type of property */
	nnfw::Variant::types type() const;
	/*! Return the object whom this PropertyAccessModel is refered to */
	PropertizedModel* object();
signals:
	/*! emitted when the property value change after call to set */
	void changeValue( const nnfw::Variant& newdata );
	/*! for vector properties */
	void changeValue( unsigned int i, const nnfw::Variant& newdata );
private:
	/*! the property */
	nnfw::AbstractPropertyAccess* prop;
	/*! model whom this object is refered to */
	PropertizedModel* parent;
};

/*! \brief Model of Propertized class
 *
 * \par Description
 * \par Warnings
 */
class FLEX_API PropertizedModel : public QObject {
	Q_OBJECT
public:
	/*! create a Model for the propertized passed */
	PropertizedModel( nnfw::Propertized* proped );
	/*! return the typename */
	QString getTypename();
	/*! return the value of property */
	nnfw::Variant nnProperty( QString name );
	/*! set the property */
	bool nnSetProperty( QString name, const nnfw::Variant& data );
	/*! set i-th value of the vector property */
	bool nnSetVectorProperty( QString name, unsigned int i, const nnfw::Variant& data );
	/*! return a vector of the Properties of this object */
	QVector<PropertyAccessModel*> nnProperties();
signals:
	/*! emitted when a property value change after call to set */
	void changeValue( const QString& name, const nnfw::Variant& newdata );
	/*! for vector properties */
	void changeValue( const QString& name, unsigned int i, const nnfw::Variant& newdata );
private:
	nnfw::Propertized* proped;
	QVector<PropertyAccessModel*> props;
};

#endif
