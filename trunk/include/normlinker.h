/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                     *
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

#ifndef NORMLINKER_H
#define NORMLINKER_H

/*! \file
 */

#include "types.h"
#include "matrixlinker.h"

namespace nnfw {

/*! \brief NormLinker Class
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>name</td> <td>string</td> <td>read/write</td> <td> name of the object </td> <td>Updatable</td> </tr>
 *   <tr><td>from</td> <td>Cluster</td> <td>read-only</td> <td> incoming Cluster </td> <td>Linker</td> </tr>
 *   <tr><td>to</td> <td>Cluster</td> <td>read-only</td> <td> outgoing Cluster </td> <td>Linker</td> </tr>
 *   <tr><td>weights</td> <td>RealMat</td> <td>read/write</td> <td> connections' weights </td> <td>MatrixLinker</td> </tr>
 *   </table>
 */
class NNFW_API NormLinker : public MatrixLinker {
public:
    /*! \name Constructors */
    //@{

    /*! Connect clusters with a complete connections
     */
    NormLinker( Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*! Construct by PropertySettings
     */
    NormLinker( PropertySettings& prop );

    /*! Destructor
     */
    virtual ~NormLinker();

    //@}
    /*! \name Interface */
    //@{

    /*! Performs the euclidean norm calculation
     */
    void update();

    //@}

protected:
    // temp data
    RealVec temp;
};

}

#endif
