/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2006 Gianluca Massera <emmegian@yahoo.it>                     *
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

#ifndef BIASEDCLUSTER_H
#define BIASEDCLUSTER_H

#include "cluster.h"
#include "clusterupdater.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief BiasedCluster Class. In this cluster a neuron have an input, a output and a bias value.
 *
 *  The BiasedCluster update the neurons contained using an object of type ClusterUpdater. The ClusterUpdater calculate the outputs
 *  of neuron as function of inputs of the neurons. <br>
 *  <em> I know... I know that the name ClusterUpdater is confusing... but for now I haven't a better idea !! </em>
 *  Further Details coming soon ... :-)
 */
class  BiasedCluster : public Cluster {
public:
    /*! \brief Construct a Cluster that contains numNeurons neuron
     *
     *  Details...
     */
    BiasedCluster( u_int numNeurons, const char* name = "unnamed" );

    /*! \brief Destructor
     *
     * Details..
     */
    virtual ~BiasedCluster();

    /*! \brief Update the outputs of neurons
     */
    void update();

    /*! \brief Set the bias of the neuron
     */
    void setBias( u_int neuron, Real bias );

    /*! \brief Set all biases with the same value
     */
    void setAllBiases( Real bias );

    /*! \brief Set the biases from the vector given
     */
    void setBiases( const RealVec& biases );

    /*! \brief Get bias of the neuron
     */
    Real getBias( u_int neuron );

	/*! \brief Get the array of biases
     *  Return the array of biases, not a copy of biases; Then you can change biases by the pointer returned !!!
     */
    RealVec& biases() {
        return biasesdata;
    };

    /*! \brief Randomize the biases of BiasedCluster
     */
    void randomize( Real min, Real max );

private:
    RealVec biasesdata;
    //! temporary data
    RealVec tmpdata;
};

}

#endif
