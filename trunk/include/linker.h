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

#ifndef LINKERS_H
#define LINKERS_H

/*! \file
 *  \brief This file contains the declaration of the linkers implemented
 *
 */

#include "types.h"
#include "cluster.h"
#include "updatable.h"


namespace nnfw {

/*! \brief Abstract Linker Class. This define the common interface among Linkers
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>name</td> <td>string</td> <td>read/write</td> <td> name of the object </td> <td>Updatable</td> </tr>
 *   <tr><td>from</td> <td>Cluster</td> <td>read-only</td> <td> incoming Cluster </td> <td>this</td> </tr>
 *   <tr><td>to</td> <td>Cluster</td> <td>read-only</td> <td> outgoing Cluster </td> <td>this</td> </tr>
 *   </table>
 *
 */
class NNFW_API Linker : public Updatable {
public:
    /*! \name Constructors */
    //@{

    /*! Construct */
    Linker( Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*! Construct by PropertySettings
     */
    Linker( PropertySettings& );

    //@}
    /*! \name Deprecated Methods */
    //@{

    /*! Return the Cluster From -- Deprecated ( use from() )
     */
    Cluster* getFrom() const {
        return fromc;
    };

    /*! Return the Cluster to -- Deprecated ( use to() )
     */
    Cluster* getTo() const {
        return toc;
    };

    //@}
    /*! \name Interface */
    //@{

    /*! Return the Cluster From
     */
    Cluster* from() const {
        return fromc;
    };

    /*! Return the Cluster to
     */
    Cluster* to() const {
        return toc;
    };

    /*! Return Cluster from (Variant version)
     */
    Variant fromP() {
        return Variant( fromc );
    };

    /*! Return Cluster to (Variant version)
     */
    Variant toP() {
        return Variant( toc );
    };

    /*! Returns the 'size' of the linker<br>
     * The actual means of 'size' will be specified by sub-classes that implementes concrete linkers
     */
    virtual u_int size() const = 0;

    /*! Randomize the parameters of the Linker<br>
     * The parameters randomized by this method will be specified by sub-classes
     */
    virtual void randomize( Real min, Real max ) = 0;

	/*! Clone this Linker */
	virtual Linker* clone() const;

    //@}

private:
    /*! incoming Cluster */
    Cluster* fromc;
    /*! outgoing Cluster */
    Cluster* toc;
};

}

#endif
