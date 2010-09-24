/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>                *
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

#ifndef UPDATABLE_H
#define UPDATABLE_H

/*! \file
 */

#include "types.h"
#include <factory/parametersettable.h>

namespace nnfw {

/*! \brief Updatables objects
 *
 *  The Updatable objects has a name.
 */
class NNFW_API Updatable : public ParameterSettableInConstructor {
public:
	/*! \name Constructors */
	//@{
	/*! Constructor */
	Updatable( QString name = "unnamed" );
	/*! Destructor */
	virtual ~Updatable();
	//@}
	/*! \name Interface */
	//@{
	/*! Update the object */
	virtual void update() = 0;
	/*! Set the name of Updatable */
	void setName( QString newname );
	/*! Return its name */
	QString name() const;
	/*! Return its typename */
	virtual QString typeName() = 0;
	//@}
protected:
	QString namev;
};

}

#endif
