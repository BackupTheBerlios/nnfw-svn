/******************************************************************************
 *  Neural Network Framework.                                                 *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>              *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the Free Software               *
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA *
 *****************************************************************************/

#ifndef BIASEDCLUSTER_H
#define BIASEDCLUSTER_H

/*! \file
 *  \brief This file contains the declaration of BiasedCluster class
 *
 *  Details...
 */

#include "types.h"
#include "cluster.h"
#include "outputfunction.h"

namespace nnfw {

/*! \brief In this cluster a neuron have an input, an output and a bias value.
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
 *   <tr><td>biases</td> <td>RealVec</td> <td>read/write</td> <td> neuron's bias </td> <td>this</td> </tr>
 *   </table>
 *
 */
class NNFW_API BiasedCluster : public Cluster {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a Cluster that contains numNeurons neuron */
    BiasedCluster( unsigned int numNeurons, const char* name = "unnamed" );

    /*! Construct by a PropertySettings */
    BiasedCluster( PropertySettings& prop );

    /*! Destructor */
    virtual ~BiasedCluster();

    //@}
    /*! \name Interface */
    //@{

    /*! Update the outputs of neurons */
    void update();

    /*! Set the bias of the neuron */
    void setBias( unsigned int neuron, double bias );

    /*! Set all biases with the same value */
    void setAllBiases( double bias );

    /*! Set the biases from the vector given */
    void setBiases( const RealVec& biases );

    /*! Get bias of the neuron */
    double getBias( unsigned int neuron );

	/*! Get the array of biases
     *  Return the array of biases, not a copy of biases; Then you can change biases by the pointer returned !!!
     */
    RealVec& biases() {
        return biasesdata;
    };

    /*! read property 'biases' */
    Variant getBiases() {
        return Variant( &biasesdata );
    };

    /*! set property 'biases' */
    bool setBiases( const Variant& v ) {
        setBiases( *( v.getRealVec() ) );
        return true;
    };

    /*! Randomize the biases of BiasedCluster */
    void randomize( double min, double max );

	/*! Clone this BiasedCluster */
	virtual BiasedCluster* clone() const;

    //@}

private:
    RealVec biasesdata;
    RealVec tempdata;

    /*! define properties */
    void propdefs();
};

}

#endif
