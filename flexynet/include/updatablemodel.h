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

#ifndef UPDATABLEMODEL_H
#define UPDATABLEMODEL_H
#include "types.h"

#include "propertizedmodel.h"
#include "nnfw/updatable.h"
#include <QString>

/*! \brief Model of Updatable class
 *
 * \par Description
 * \par Warnings
 */
class FLEX_API UpdatableModel : public PropertizedModel {
	Q_OBJECT
public:
	/*! create a Model for the Updatable passed */
	UpdatableModel( nnfw::Updatable* up );
	/*! return the name of it */
	QString name();
	/*! return the Updatable */
	nnfw::Updatable* nnUpdatable();

public slots:
	/*! set the name */
	void setName( QString name );
	/*! update it */
	void update();

signals:
	/*! emitted when it's updated */
	void updated();

private:
	nnfw::Updatable* updatable;
};

#endif
