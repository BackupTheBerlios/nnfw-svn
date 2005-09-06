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
 *  \brief This file contains the declarations of the clusters implemented
 *
 *  Details...
 */

#include "types.h"
#include "updatable.h"
#include "clusterupdater.h"

/*! \brief Abstract Cluster Class. This define the common interface among Clusters
 *
 *  Cluster objects memorize the inputs and outputs into array of nnfwReal !! 
 *  ( ... ?!?! ... )
 */
class Cluster : public Updatable
{
public:
    //! Construct a Cluster
    Cluster( const char* name = "unnamed" ) : Updatable(name) {
        // nothing to do
    };

    /*! \brief Return the number of the inputs neuron
     *
     * Details...
     */
    virtual u_int inputSize() const = 0;

    /*! \brief Return the number of the outputs neuron
     *
     * Details...
     */
    virtual u_int outputSize() const = 0;

    /*! \brief Set the update function
     *
     * Details...
     */
    virtual void setUpdater( ClusterUpdater* up ) = 0;

    /*! \brief Get the update function
     *
     */
    virtual const ClusterUpdater* getUpdater() const = 0;

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

#endif

