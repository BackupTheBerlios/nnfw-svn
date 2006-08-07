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
 *    The number of neuron returned by size() method is also the dimension of inputs and outputs arrays<br>
 *    You can sets one subclasses of ClusterUpdater by setUpdater methods. If you don't specify an index when
 *    set a ClusterUpdater then this ClusterUpdater will be used to update the output of all neurons. Otherwise,
 *    you can specifiy different ClusterUpdater for different neuron.
 *    \code
 * // create a SimpleCluster, a specialized subclass of Cluster
 * SimpleCluster* simple = new SimpleCluster( 10 ); // this cluster contains 10 neurons
 * // set the SigmoidUpdater for all neurons
 * simple->setUpdater( new SigmoidUpdater( 1.0 ) );
 * // If you want that neuron 2 will be updated by a Linear function then type:
 * simple->setUpdater( new LinearUpdater(), 2 );
 * // After this statement only neuron 2 will be updated by Linear function and the others
 * //  will be updated with Sigmoidal function
 *    \endcode
 *  \par Warnings
 *    <b>For whose want to implement a subclass of Cluster: </b>
 *    This class allocate the memory for inputs, outputs and updaters array. So, a subclass have to implements only
 *    the update method.
 *    The getInputs and getOutputs returns a valid array of internal data, and not simply a copy
 *    of the internal data. Look at the following code:
 *    \code
 * RealVec& in = cluster->inputs();
 * in[2] = 3.0;   // This statement will be changes the inputs of third neuron.
 * // the above statements must be equivalent with the following
 * cluster->setInput( 2, 3.0 );
 *    \endcode
 *    The reasons behind this kind of behaviour its the efficiency!! When another class must do heavy calculation
 *    on all inputs of a Cluster (as MatrixLinker do), then its more efficient that it takes the array returned
 *    by inputs (or outputs) and works over them.
 */
class  Cluster : public Updatable
{
public:
    //! Construct a Cluster
    Cluster( u_int numNeurons, const char* name = "unnamed" );

    //! Destructor
    virtual ~Cluster();

    /*! \brief Return the number of neurons (the length of input and output arrays)
     * Details...
     */
    u_int size() const {
        return numNeurons;
    };

    /*! \brief Set the input of neuron
     * Details...
     */
    virtual void setInput( u_int neuron, Real value );

    /*! \brief Set the inputs from the vector given
     */
    virtual void setInputs( const RealVec& inputs );

    /*! \brief Set all the inputs with the same value
     * Details...
     */
    virtual void setAllInputs( Real value );

    /*! \brief Reset the inputs of this cluster, typically this means that the inputs will be set to zero.
     * Details...
     */
    virtual void resetInputs();

    /*! \brief Get the input of neuron
     * Details...
     */
    virtual Real getInput( u_int neuron ) const;

    /*! \brief Force the output of the neuron at value specified
     * Details...
     */
    virtual void setOutput( u_int neuron, Real value );

    /*! \brief Set the outputs from the vector given
     */
    virtual void setOutputs( const RealVec& outputs );

    /*! \brief Get the output of neuron
     * Details...
     */
    virtual Real getOutput( u_int neuron ) const;

    /*! \brief Randomize the parameters of the Cluster
     * The parameters randomized by this method will be specified by sub-classes
     */
    virtual void randomize( Real min, Real max ) = 0;

    /*! \brief Set the update function for all neurons contained
     * ATTENTION: This function delete the previous updater class registered !!! <br>
     */
    void setUpdater( ClusterUpdater* up );

    /*! \brief Set the update function for the neuron specified
     * Details...
     */
    void setUpdater( ClusterUpdater* up, u_int neuron );

    /*! \brief Get the update function for the neuron specified
     */
    const ClusterUpdater* getUpdater( u_int neuron ) const;

    /*! \brief Get the array of updaters
     *  It's a pointer to const data. You can't change ClusterUpdater via this pointer.
     *  Instead, use the setUpdater method()
     */
    ClusterUpdater** updaters() const {
        return poolUpdater;
    };

    /*! \brief Return true if all neuron share the same ClusterUpdater
     */
    bool isSingleUpdater() const {
        return singleUpd;
    };

    /*! \brief Get the array of inputs
     *  Return the array of inputs, not a copy of inputs; Then you can change inputs by the pointer returned !!!
     */
    RealVec& inputs() {
        return inputdata;
    };

    /*! \brief Get the array of outputs
     *
     *  Return the array of outputs, not a copy of outputs; Then you can change outputs by the pointer returned !!!
     */
    RealVec& outputs() {
        return outputdata;
    };

    /*! \brief Return true if inputs needs a reset
     */
    bool needReset() {
        return needRst;
    };

protected:
    /*! \brief Set the state of 'needReset'
     *  Used by subclassed into update implementation
     */
    void setNeedReset( bool b ) {
        needRst = b;
    };

    /*! \brief Only for special needs during subclass implementation
     *  See the FakeCluster implemetation for an example
     */
//     void changeInputPointerData( Real* newPointer ) {
//         inputdata = newPointer;
//     };

    /*! \brief Only for special needs during subclass implementation
     *  See the FakeCluster implemetation for an example
     */
//     void changeOutputPointerData( Real* newPointer ) {
//         outputdata = newPointer;
//     };

private:
    u_int numNeurons;
    RealVec inputdata;
    RealVec outputdata;
    //! Updaters Object
    ClusterUpdater* singleUpdater;
    ClusterUpdater** poolUpdater;
    bool singleUpd;
    //! True if the inputs needs a reset
    bool needRst;
};

}

#endif

