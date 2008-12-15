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

#ifndef SIMPLECLUSTER_H
#define SIMPLECLUSTER_H

/*! \file
 */

#include "types.h"
#include "cluster.h"
#include "outputfunction.h"

namespace nnfw {

/*! \brief SimpleCluster Class
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>name</td> <td>string</td> <td>read/write</td> <td> name of the object </td> <td>Updatable</td> </tr>
 *   <tr><td>accumulate</td> <td>boolean</td> <td>read/write</td> <td> if inputs are accumulated </td> <td>Cluster</td> </tr>
 *   <tr><td>inputs</td> <td>RealVec</td> <td>read/write</td> <td> neuron's input </td> <td>Cluster</td> </tr>
 *   <tr><td>outfunction</td> <td>OutputFunction</td> <td>read/write</td> <td> neuron's output function </td> <td>Cluster</td> </tr>
 *   <tr><td>outputs</td> <td>RealVec</td> <td>read/write</td> <td> neuron's output </td> <td>Cluster</td> </tr>
 *   <tr><td>size</td> <td>unsigned int</td> <td>read-only</td> <td> number of neurons </td> <td>Cluster</td> </tr>
 *   </table>
 *
 */
class NNFW_API SimpleCluster : public Cluster {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a Cluster that contains numNeurons neuron
     */
    SimpleCluster( unsigned int numNeurons, const char* name = "unnamed" );

    /*! Construct by a PropertySettings
     */
    SimpleCluster( PropertySettings& prop );

    /*! Destructor
     */
    virtual ~SimpleCluster();

    //@}
    /*! \name Interface */
    //@{

    /*! Update the outputs of neurons
     */
    void update();

    /*! Randomize Nothing ;-)
     */
    void randomize( double, double ) { /* Nothing To Do */ };

	/*! Clone this SimpleCluster */
	virtual SimpleCluster* clone() const;

    //@}
};

}

#endif
