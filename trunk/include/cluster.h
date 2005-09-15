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

#ifndef CLUSTER_H
#define CLUSTER_H

/*! \file
 *  \brief This file contains the declarations of the Cluster class
 */

#include "types.h"
#include "updatable.h"
#include "clusterupdater.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Abstract Cluster Class. This define the common interface among Clusters
 *
 *  \par Motivation
 *    The Cluster class define the common interface amog Cluster. The subclasses may extends this interface
 *    for specific purpose (ex. SimpleCluster), but the BaseNeuralNet, the Linker and other classes depends
 *    only by this class. This abstraction allow to isolate the specific implementation of various classes
 *  \par Description
 *    The Cluster class represent an abstract group of neurons. There isn't a neuron class. The Cluster
 *    class represent a group of neurons as two arrays: inputs and outputs. The inputs array represent the
 *    inputs of the neurons 'contained' in the cluster, and the outputs of this neurons are calculated by
 *    appling the function provided by ClusterUpdater.<br>
 *    The number of neuron returned by size() method is also the dimension of inputs and outputs arrays
 *    \par
 *    You can sets one subclasses of ClusterUpdater by setUpdater methods. If you don't specify an index when
 *    set a ClusterUpdater then this ClusterUpdater will be used to update the output of all neurons. Otherwise,
 *    you can specifiy different ClusterUpdater for different neuron.
 *    \code
 *         // create a SimpleCluster, a specialized subclass of Cluster
 *         SimpleCluster* simple = new SimpleCluster( 10 ); // this cluster contains 10 neurons
 *         // set the SigmoidUpdater for all neurons
 *         simple->setUpdater( new SigmoidUpdater( 1.0 ) );
 *         // If you want that neuron 2 will be updated by a Linear function then type:
 *         simple->setUpdater( new LinearUpdater(), 2 );
 *         // After this statement only neuron 2 will be updated by Linear function and the others
 *         //  will be updated with Sigmoidal function
 *    \endcode
 *  \par Warnings
 *    <b>For whose want to implement a subclass of Cluster: </b>
 *    The getInputs and getOutputs methods have to returns a valid array of internal data, and not simply a copy
 *    of the internal data. Look at the following code:
 *    \code
 *         nnfwReal* in = cluster->getInputs();
 *         in[2] = 3.0;   // This statement will be changes the inputs of third neuron.
 *         // the above statements must be equivalent with the following
 *         cluster->setInput( 2, 3.0 );
 *    \endcode
 *    The reasons behind this kind of behaviour its the efficiency!! When another class must do heavy calculation
 *    on all inputs of a Cluster (as MatrixLinker do), then its more efficient that it takes the array returned
 *    by getInputs (or getOutputs) and works over them.<br>
 *    This imply that <b>every</b> subclasses have to represents the input and output 
 *    of neurons as nnfwReal arrays ( C arrays not STL-classes )
 */
class Cluster : public Updatable
{
public:
    //! Construct a Cluster
    Cluster( const char* name = "unnamed" ) : Updatable(name) {
        // nothing to do
    };

    /*! \brief Return the number of neurons
     *
     * Details...
     */
    virtual u_int size() const = 0;

    /*! \brief Set the update function for all neurons contained
     *
     * Details...
     *
     */
    virtual void setUpdater( ClusterUpdater* up ) = 0;

    /*! \brief Set the update function for the neuron specified
     *
     * Details...
     */
    virtual void setUpdater( ClusterUpdater* up, u_int neuron ) = 0;

    /*! \brief Get the update function for the neuron specified
     *
     */
    virtual const ClusterUpdater* getUpdater( u_int neuron ) const = 0;

    /*! \brief Set the input of neuron
     *
     * Details...
     */
    virtual void setInput( u_int neuron, nnfwReal value ) = 0;

    /*! \brief Set all the inputs with the same value
     *
     * Details...
     */
    virtual void setAllInputs( nnfwReal value ) = 0;

    /*! \brief Reset the inputs of this cluster, typically this means that the inputs will be set to zero.
     *
     * Details...
     */
    virtual void resetInputs() = 0;

    /*! \brief Get the input of neuron
     *
     * Details...
     */
    virtual nnfwReal getInput( u_int neuron ) const = 0;

    /*! \brief Get the array of inputs
     *
     *  Return the array of inputs, not a copy of inputs; Then you can change inputs by the pointer returned !!!
     */
    virtual nnfwReal* getInputs( ) = 0;

    /*! \brief Force the output of the neuron at value specified
     *
     * Details...
     */
    virtual void setOutput( u_int neuron, nnfwReal value ) = 0;

    /*! \brief Get the output of neuron
     *
     * Details...
     */
    virtual nnfwReal getOutput( u_int neuron ) const = 0;

    /*! \brief Get the array of inputs
     *
     *  Return the array of outputs, not a copy of outputs; Then you can change outputs by the pointer returned !!!
     */
    virtual nnfwReal* getOutputs( ) = 0;

    /*! \brief Randomize the parameters of the Cluster
     *
     * The parameters randomized by this method will be specified by sub-classes
     */
    virtual void randomize( nnfwReal min, nnfwReal max ) = 0;
};

}

#endif

