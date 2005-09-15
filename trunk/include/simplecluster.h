/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005 Gianluca Massera <emmegian@yahoo.it>                     *
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

#include "cluster.h"
#include "clusterupdater.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief SimpleCluster Class. In this cluster a neuron have an input, a output and a bias value.
 *
 *  The SimpleCluster update the neurons contained using an object of type ClusterUpdater. The ClusterUpdater calculate the outputs
 *  of neuron as function of inputs of the neurons. <br>
 *  <em> I know... I know that the name ClusterUpdater is confusing... but for now I haven't a better idea !! </em>
 *  Further Details coming soon ... :-)
 */
class SimpleCluster : public Cluster
{
public:
    /*! \brief Construct a Cluster that contains numNeurons neuron
     *
     *  Details...
     */
    SimpleCluster( u_int numNeurons, const char* name = "unnamed" );

    /*! \brief Destructor
     *
     * Details..
     */
    virtual ~SimpleCluster();

    /*! \brief Return the number of the neurons
     *
     * Details...
     */
    u_int size() const;

    /*! \brief Update the outputs of neurons
     *
     * Details
     */
    void update();

    /*! \brief Set the update function
     *
     * ATTENTION: This function delete the previous updater class registered !!! <br>
     * Details...
     */
    void setUpdater( ClusterUpdater* up );

    /*! \brief Set the update function for the neuron specified
     *
     * Details...
     */
    void setUpdater( ClusterUpdater* up, u_int neuron );

    /*! \brief Get the update function
     *
     * Details...
     */
    const ClusterUpdater* getUpdater( u_int neuron ) const;

    /*! \brief Set the input of neuron
     *
     * Details...
     */
    void setInput( u_int neuron, nnfwReal value );

    /*! \brief Set all the inputs with the same value
     *
     * Details...
     */
    void setAllInputs( nnfwReal value );

    /*! \brief It will sets the inputs at the value of biases
     *
     * Details...
     */
    void resetInputs();

    /*! \brief Get the input of neuron
     *
     * Details...
     */
    nnfwReal getInput( u_int neuron ) const;

    /*! \brief Get the array of inputs
     *
     *  Return the array of inputs, not a copy of inputs; Then you can change inputs by the pointer returned !!!
     */
    nnfwReal* getInputs( );

    /*! \brief Force the output of the neuron at value specified
     *
     * Details...
     */
    void setOutput( u_int neuron, nnfwReal value );

    /*! \brief Get the output of neuron
     *
     * Details...
     */
    nnfwReal getOutput( u_int neuron ) const;

    /*! \brief Get the array of inputs
     *
     *  Return the array of outputs, not a copy of outputs; Then you can change outputs by the pointer returned !!!
     */
    nnfwReal* getOutputs( );

    /*! \brief Set the bias of the neuron
     *
     * Details ...
     */
    void setBias( u_int neuron, nnfwReal bias );

    /*! \brief Get bias of the neuron
     *
     * Details...
     */
    nnfwReal getBias( u_int neuron );

    /*! \brief Randomize the biases of SimpleCluster
     *
     * Details...
     */
    void randomize( nnfwReal min, nnfwReal max );

private:
    u_int numNeurons;
    nnfwReal* inputdata;
    nnfwReal* outputdata;
    nnfwReal* biases;
    //! Updaters Object
    ClusterUpdater* singleUpdater;
    ClusterUpdater** poolUpdater;
    bool singleUpd;
    //! temporary data
    nnfwReal* tmpdata;
};

}

#endif
