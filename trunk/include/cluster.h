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
#include "outputfunction.h"


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
 *    appling the function provided by OutputFunction.<br>
 *    The number of neuron returned by size() method is also the dimension of inputs and outputs arrays<br>
 *    You can sets one subclasses of OutputFunction by setUpdater methods. If you don't specify an index when
 *    set a OutputFunction then this OutputFunction will be used to update the output of all neurons. Otherwise,
 *    you can specifiy different OutputFunction for different neuron.
 *    \code
 * // create a SimpleCluster, a specialized subclass of Cluster
 * SimpleCluster* simple = new SimpleCluster( 10 ); // this cluster contains 10 neurons
 * // set the SigmoidUpdater for all neurons
 * simple->setUpdater( new SigmoidUpdater( 1.0 ) );
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
 * \nosubgrouping
 */
class  Cluster : public Updatable {
public:
    /*! \name Constructors */
    //@{

    //! Construct a Cluster
    Cluster( u_int numNeurons, const char* name = "unnamed" );

    //! Construct a Cluster with PropertySettings
    Cluster( PropertySettings& prop );

    //! Destructor
    virtual ~Cluster();

    //@}
    /*! \name Methods affecting whole Cluster */
    //@{

    /*! \brief Return the number of neurons (the length of input and output arrays)
     * Details...
     */
    u_int size() const {
        return numNeurons;
    };

    /*! \brief Return true if inputs needs a reset
     */
    bool needReset() {
        return needRst;
    };

    /*! \brief Enable/Disable accumulation mode
     *  if accumulation is enabled (true) then linkers attached to this Cluster will never resetInput and accumulates data,
     *  otherwise the inputs will be resetted at each step of neural network
     */
    void accumulate( bool mode ) {
        accOff = !mode;
    };

    /*! \brief return true if the Cluster will accumulates inputs
     */
    bool isAccumulate() {
        return !accOff;
    };

    /*! \brief Randomize the parameters of the Cluster
     * The parameters randomized by this method will be specified by sub-classes
     */
    virtual void randomize( Real min, Real max ) = 0;

    /*! \brief Read Access to property 'size'
     */
    Variant sizeP() {
        return Variant( size() );
    };

    /*! \brief Read Access to property 'accumulate'
     */
    Variant accumP() {
        return Variant( isAccumulate() );
    };

    /*! \brief Write Access to property 'accumulate'
     */
    bool setAccumP( const Variant& b ) {
        accumulate( b.getBool() );
        return true;
    };

    //@}
    /*! \name Operations on Input's vector */
    //@{

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

    /*! \brief Get the array of inputs
     *  Return the array of inputs, not a copy of inputs; Then you can change inputs by the pointer returned !!!
     */
    RealVec& inputs() {
        return inputdata;
    };

    //! For property 'inputs'
    Variant inputsP() {
        return Variant( &inputdata );
    };

    //! setting the property 'inputs'
    bool setInputsP( const Variant& v ) {
        inputdata.assign( *(v.getRealVec()) );
        return true;
    };

    //@}
    /*! \name Operations on Output's vector */
    //@{

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

    /*! \brief Get the array of outputs
     *
     *  Return the array of outputs, not a copy of outputs; Then you can change outputs by the pointer returned !!!
     */
    RealVec& outputs() {
        return outputdata;
    };

    //! For property 'outputs'
    Variant outputsP() {
        return Variant( &outputdata );
    };

    //! setting the property 'outputs'
    bool setOutputsP( const Variant& v ) {
        outputdata.assign( *(v.getRealVec()) );
        return true;
    };

    //@}
    /*! \name Operations on OutputFunction */
    //@{

    /*! \brief Set the output function for all neurons contained
     *
     *  This method create an internal copy of the OutputFunction passed <br>
     *  ATTENTION: This function delete the previous updater class registered !!! <br>
     */
    void setFunction( const OutputFunction& up );

    /*! \brief Get the Output function
     */
    OutputFunction* const getFunction() {
        return updater;
    };

    //! read property 'outfunction'
    Variant getFunctionP() {
        return Variant( updater );
    };

    //! sets the property 'outfunction'
    bool setFunction( const Variant& outf ) {
        setFunction( *(outf.getOutputFunction()) );
        return true;
    };

    //@}

protected:
    /*! \brief Set the state of 'needReset'
     *  Used by subclassed into update implementation
     */
    void setNeedReset( bool b ) {
        needRst = accOff && b;
    };

private:
    //! Number of neurons
    u_int numNeurons;
    //! Input of neurons
    RealVec inputdata;
    //! Output of neurons
    RealVec outputdata;
    //! OutputFunction Object
    OutputFunction* updater;

    //! True if the inputs needs a reset
    bool needRst;
    /*! In Accumulated mode the needRst is always false, and then linkers attached to this will never resetInputs
     *  --- Warns for developers --- accOff == true means NO-ACCUMULATION
     */
    bool accOff;

    //! define properties
    void propdefs();
};

}

#endif

