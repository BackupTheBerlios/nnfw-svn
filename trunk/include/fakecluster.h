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

#ifndef FAKECLUSTER_H
#define FAKECLUSTER_H

#include "cluster.h"

/*! \brief FakeCluster Class. The FakeCluster is a Cluster without neurons, it's like an array ! :-)
 *
 *  The FakeCluster implemented the Cluster Interface so it can be inserted inside a BaseNeuralNet, but it doesn't
 *  contains neurons, but it contains a simple vector with values and inputs and outputs are alias name for this vector of data. <br>
 *  It's very useful for inputs layers that don't needs true nuerons but only a vector where registry the sensors values.
 *
 */
class FakeCluster : public Cluster
{
public:
    /*! \brief Construct a FakeCluster
     *
     *  Details...
     */
    FakeCluster( u_int size, const char* name = "unnamed" );

    /*! \brief Destructor
     *
     * Details...
     */
    virtual ~FakeCluster();

    /*! \brief Return the size of the data vector
     *
     * Details...
     */
    u_int inputSize() const;

    /*! \brief Return the size of the data vector
     *
     * Details...
     */
    u_int outputSize() const;

    /*! \brief Update ;-) It's do nothing !!
     *
     * Details
     */
    void update();

    /*! \brief This function is ignored !! ;-)
     *
     * Details...
     */
    void setUpdater( ClusterUpdater* up );

    /*! \brief This function is ignored !! ;-)
     *
     * Details...
     */
    void setUpdater( ClusterUpdater* up, u_int numNeuron );

    /*! \brief It's always return DummyUpdater ;-)
     *
     */
    const ClusterUpdater* getUpdater( u_int numNeuron ) const;

    /*! \brief Set the n-th position of vector
     *
     * Details...
     */
    void setInput( u_int n, nnfwReal value );

    /*! \brief Set all the inputs with the same value
     *
     * Details...
     */
    void setAllInputs( nnfwReal value );

    /*! \brief Reset the data of vector ( set all inputs to zero )
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

    /*! \brief Same as setInput ;-)
     *
     * Details...
     */
    void setOutput( u_int neuron, nnfwReal value );

    /*! \brief Get the output of neuron ( same as getInput ) ;-)
     *
     * Details...
     */
    nnfwReal getOutput( u_int neuron ) const;

    /*! \brief Get the array of inputs
     *
     *  Return the array of outputs, not a copy of outputs; Then you can change outputs by the pointer returned !!!
     */
    nnfwReal* getOutputs( );

    /*! \brief Randomize ?!?! What means 'randomize a FakeCluster' ???
     *
     * Details...
     */
    void randomize( nnfwReal min, nnfwReal max );

private:
    u_int dim;
    nnfwReal* data;
    ClusterUpdater updater;
};

#endif
