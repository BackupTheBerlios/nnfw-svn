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

#ifndef CLUSTER_H
#define CLUSTER_H

/*! \file
 *  \brief This file contains the declarations of the Cluster class
 */

#include "types.h"
#include "updatable.h"
#include "outputfunction.h"

namespace nnfw {

/*! \brief Define the common interface among Clusters
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
 * simple->setUpdater( SigmoidUpdater( 1.0 ) );
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
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>name</td> <td>string</td> <td>read/write</td> <td> name of the object </td> <td>Updatable</td> </tr>
 *   <tr><td>accumulate</td> <td>boolean</td> <td>read/write</td> <td> if inputs are accumulated </td> <td>this</td> </tr>
 *   <tr><td>inputs</td> <td>RealVec</td> <td>read/write</td> <td> neuron's input </td> <td>this</td> </tr>
 *   <tr><td>outfunction</td> <td>OutputFunction</td> <td>read/write</td> <td> neuron's output function </td> <td>this</td> </tr>
 *   <tr><td>outputs</td> <td>RealVec</td> <td>read/write</td> <td> neuron's output </td> <td>this</td> </tr>
 *   <tr><td>numNeurons</td> <td>unsigned int</td> <td>read-only</td> <td> number of neurons </td> <td>this</td> </tr>
 *   </table>
 *
 */
class NNFW_API Cluster : public Updatable {
public:
    /*! \name Constructors */
    //@{
    /*! Construct a Cluster */
    Cluster( unsigned int numNeurons, const char* name = "unnamed" );
    /*! Construct a Cluster with PropertySettings */
    Cluster( PropertySettings& prop );
    /*! Destructor */
    virtual ~Cluster();

    //@}
    /*! \name Methods affecting whole Cluster */
    //@{

    /*! Return the number of neurons (the length of input and output arrays) */
    unsigned int numNeurons() const {
        return numneurons;
    };

    /*! Return true if inputs needs a reset */
    bool needReset() {
        return needRst;
    };

    /*! Enable/Disable accumulation mode<br>
     *  If accumulation is enabled (true) then linkers attached to this Cluster will never resetInput and accumulates data,
     *  otherwise the inputs will be resetted at each step of neural network
     */
    void setAccumulate( bool mode ) {
        accOff = !mode;
    };

    /*! return true if the Cluster will accumulates inputs */
    bool isAccumulate() const {
        return !accOff;
    };

    /*! Randomize the parameters of the Cluster<br>
     *  The parameters randomized by this method will be specified by sub-classes
     */
    virtual void randomize( double min, double max ) = 0;

    /*! Read Access to property 'size'
     */
    Variant numNeuronsP() {
        return Variant( numNeurons() );
    };

    /*! Read Access to property 'accumulate'
     */
    Variant accumP() {
        return Variant( isAccumulate() );
    };

    /*! Write Access to property 'accumulate'
     */
    bool setAccumP( const Variant& b ) {
        setAccumulate( b.getBool() );
        return true;
    };

    //@}
    /*! \name Operations on Input's vector */
    //@{

    /*! Set the input of neuron
     * Details...
     */
    virtual void setInput( unsigned int neuron, double value );

    /*! Set the inputs from the vector given
     */
    virtual void setInputs( const RealVec& inputs );

    /*! Set all the inputs with the same value
     * Details...
     */
    virtual void setAllInputs( Real value );

    /*! Reset the inputs of this cluster, typically this means that the inputs will be set to zero.
     * Details...
     */
    virtual void resetInputs();

    /*! Get the input of neuron
     */
    virtual double getInput( unsigned int neuron ) const;

    /*! Get the array of inputs<br>
     *  Return the array of inputs, not a copy of inputs; Then you can change inputs by the pointer returned !!!
     */
    RealVec& inputs() {
        return inputdata;
    };

	/*! const version of inputs() */
    const RealVec& inputs() const {
        return inputdata;
    };

    /*! For property 'inputs' */
    Variant inputsP() {
        return Variant( &inputdata );
    };

    /*! setting the property 'inputs' */
    bool setInputsP( const Variant& v ) {
        inputdata.assign( *(v.getRealVec()) );
        return true;
    };

    //@}
    /*! \name Operations on Output's vector */
    //@{

    /*! Force the output of the neuron at value specified
     */
    virtual void setOutput( unsigned int neuron, double value );

    /*! Set the outputs from the vector given
     */
    virtual void setOutputs( const RealVec& outputs );

    /*! Get the output of neuron
     */
    virtual double getOutput( unsigned int neuron ) const;

    /*! Get the array of outputs<br>
     *  Return the array of outputs, not a copy of outputs; Then you can change outputs by the pointer returned !!!
     */
    RealVec& outputs() {
		return outputdata;
    };

	/*! const version of outputs() */
	const RealVec& outputs() const {
		return outputdata;
	};

    /*! For property 'outputs' */
    Variant outputsP() {
        return Variant( &outputdata );
    };

    /*! setting the property 'outputs' */
    bool setOutputsP( const Variant& v ) {
        outputdata.assign( *(v.getRealVec()) );
        return true;
    };

    //@}
    /*! \name Operations on OutputFunction */
    //@{

    /*! Set the output function for all neurons contained<br>
     *  This method create an internal copy of the OutputFunction passed <br>
     *  \warning This function delete the previous updater class registered !!! <br>
     */
    void setFunction( const OutputFunction& up );

    /*! Get the Output function
     */
    OutputFunction* const getFunction() const {
        return updater;
    };

    /*! read property 'outfunction' */
    Variant getFunctionP() {
        return Variant( updater );
    };

    /*! sets the property 'outfunction' */
    bool setFunction( const Variant& outf ) {
        setFunction( *(outf.getOutputFunction()) );
        return true;
    };

	/*! Clone this Cluster */
	virtual Cluster* clone() const;

    //@}

protected:
    /*! Set the state of 'needReset'<br>
     *  Used by subclassed into update implementation
     */
    void setNeedReset( bool b ) {
        needRst = accOff && b;
    };

private:
    /*! Number of neurons */
    unsigned int numneurons;
    /*! Input of neurons */
    RealVec inputdata;
    /*! Output of neurons */
    RealVec outputdata;
    /*! OutputFunction Object */
    OutputFunction* updater;

    /*! True if the inputs needs a reset */
    bool needRst;
    /*! In Accumulated mode the needRst is always false, and then linkers attached to this will never resetInputs
     *  --- Warns for developers --- accOff == true means NO-ACCUMULATION
     */
    bool accOff;

    /*! define properties */
    void propdefs();
};

}

#endif

