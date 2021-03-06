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

#ifndef FAKECLUSTER_H
#define FAKECLUSTER_H

/*! \file
 */

#include "types.h"
#include "cluster.h"

namespace nnfw {

/*! \brief FakeCluster Class. The FakeCluster is a Cluster without neurons, it's like an array ! :-)
 *
 *  \par Motivation
 *
 *  \par Description
 *  The FakeCluster implemented the Cluster Interface so it can be inserted inside a BaseNeuralNet,
 *  but it doesn't contains neurons, but it contains a simple vector with values and inputs and outputs
 *  are alias name for this vector of data. <br>
 *  It's very useful for inputs layers that don't needs true neurons but only a vector where registry
 *  the sensors values.
 *
 *  \par Warnings
 *  Due to fact that outputs() are simple an alias, or more precisly a VectorData view, of inputs(),
 *  this means that a call to resetInputs() method also resets the outputs!! In general, all methods
 *  that modify inputs also modify outputs and viceversa.
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>name</td> <td>string</td> <td>read/write</td> <td> name of the object </td> <td>Updatable</td> </tr>
 *   <tr><td>accumulate</td> <td>boolean</td> <td>read/write</td> <td> if inputs are accumulated </td> <td>Cluster</td> </tr>
 *   <tr><td>inputs</td> <td>RealVec</td> <td>read/write</td> <td> neuron's input </td> <td>Cluster</td> </tr>
 *   <tr><td>outfunction</td> <td>OutputFunction</td> <td>read/write</td> <td> ignored </td> <td>Cluster</td> </tr>
 *   <tr><td>outputs</td> <td>RealVec</td> <td>read/write</td> <td> neuron's output (same of inputs) </td> <td>Cluster</td> </tr>
 *   <tr><td>size</td> <td>unsigned int</td> <td>read-only</td> <td> number of neurons </td> <td>Cluster</td> </tr>
 *   </table>
 *
 */
class NNFW_API FakeCluster : public Cluster {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a FakeCluster
     */
    FakeCluster( u_int size, const char* name = "unnamed" );

    /*! Construct by PropertySettings
     */
    FakeCluster( PropertySettings& );

    /*! Destructor
     */
    virtual ~FakeCluster();

    //@}
    /*! \name Interface */
    //@{

    /*! Update ;-) It's do nothing !!
     */
    void update();

    /*! Randomize ?!?! it do nothings
     */
    void randomize( Real min, Real max );

	/*! Clone this FakeCluster */
	virtual FakeCluster* clone() const;

    //@}

};

}

#endif
