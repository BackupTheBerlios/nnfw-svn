/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                     *
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
#include "propertized.h"

namespace nnfw {

/*! \brief Updatables objects
 *
 *  The Updatable objects has a name.
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>name</td> <td>string</td> <td>read/write</td> <td> name of the object </td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API Updatable : public Propertized {
public:
    /*! \name Constructors */
    //@{

    /*! Constructor */
    Updatable( const char* name = "unnamed" );

    /*! Constructor with PropertySettings */
    Updatable( PropertySettings& prop );

    /*! Destructor */
    virtual ~Updatable();

    //@}
    /*! \name Interface */
    //@{

    /*! Update the object */
    virtual void update() = 0;
    /*! Set the name of Updatable */
    void setName( const char* newname );
    /*! Set the name of Updatable (Varian version) */
    bool setName( const Variant& nv );
	/*! Return its name */
	const char* name() const;
    /*! Return the name (version that use Variant for property) */
    Variant getNameV();

    //@}

protected:
    char* namev;
};

}

#endif
