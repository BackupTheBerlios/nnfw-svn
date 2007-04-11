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

#ifndef LIBOUTPUTFUNCTIONS_H
#define LIBOUTPUTFUNCTIONS_H

#include "types.h"

/*! \file
 *  \brief Library of Common OutputFunction
 *
 *  Details...
 *
 *  \todo CompositeFunction, a subclass of OutputFunction that allow to combine two or more OutputFunctions;
 *        example: combining a SigmoidFunction with LinearFunction it's possibile realize an OutputFunction that
 *        first truncate the input in a range [mix,max] and the apply the Sigmoid: Sigmoid( Linear( x ) )
 *  \todo ComboLinearFunction, a subclass of OutputFunction that constructs a linear combination of OutputFunctions:
 *        example: 4*Sigmoid(x)+2*Linear(x)
 */

#include "outputfunction.h"
#include "derivableoutputfunction.h"


namespace nnfw {

/*! \brief IdentityFunction
 *
 * IdentityFunction copyies the inputs to the outputs
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   </table>
 */
class NNFW_API IdentityFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct
    IdentityFunction();

    //! Construct
    IdentityFunction( PropertySettings& prop );

    //! Destructor
    virtual ~IdentityFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    //! Implement the identity function
    virtual void apply( RealVec& inputs, RealVec& outputs );

    //! return always 1 (an explain of why will be coming soon)
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! \brief Clone this object
     */
    virtual IdentityFunction* clone() const;

    //@}
};

/*! \brief Sigmoid Function
 *
 * Details..
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>lambda</td> <td>Real</td> <td>read/write</td> <td> function's slope </td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API SigmoidFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a sigmoid updater with parameter l
    SigmoidFunction( Real l = 1.0 );

    //! Construct
    SigmoidFunction( PropertySettings& prop );

    //! Destructor
    virtual ~SigmoidFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Set the slope of SigmoidFunction
     */
    bool setLambda( const Variant& v );

    /*! \brief Return the slope of SigmoidFunction
     */
    Variant getLambda();

    /*! \brief Implement the updating method
     *
     * Details...
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    //! return the approximation commonly used in backpropagation learning: x(1-x)
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! \brief Clone this object
     */
    virtual SigmoidFunction* clone() const;

    //@}

    //! lambda is the slope of the curve
    Real lambda;
};

/*! \brief Fake Sigmoid Function !! Is a linear approximation of sigmoid function
 *
 * Details..
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>lambda</td> <td>Real</td> <td>read/write</td> <td> function's slope </td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API FakeSigmoidFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a sigmoid updater with parameter l
    FakeSigmoidFunction( Real l = 1.0 );

    //! Construct
    FakeSigmoidFunction( PropertySettings& prop );

    //! Destructor
    virtual ~FakeSigmoidFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Set the slope of FakeSigmoidFunction
     */
    bool setLambda( const Variant& v );

    /*! \brief Return the slope of FakeSigmoidFunction
     */
    Variant getLambda();

    /*! \brief Implement the updating method
     *
     * Details...
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    //! return the approximation commonly used in backpropagation learning: x(1-x)
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! \brief Clone this object
     */
    virtual FakeSigmoidFunction* clone() const;

    //@}

    //! lambda is the slope of the curve
    Real lambda;
};

/*! \brief ScaledSigmoid Function
 *
 * lambda is the slope of the curve<br>
 * min is the y value when x -> -infinite <br>
 * max is the y value when x -> +infinite <br>
 * (max-min) is the y value when x == 0
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>lambda</td> <td>Real</td> <td>read/write</td> <td> function's slope </td> <td>this</td> </tr>
 *   <tr><td>min</td> <td>Real</td> <td>read/write</td> <td> function's minimun value </td> <td>this</td> </tr>
 *   <tr><td>max</td> <td>Real</td> <td>read/write</td> <td> function's maximun value </td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API ScaledSigmoidFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a scaled sigmoid updater with parameter l
    ScaledSigmoidFunction( Real l = 1.0, Real min = -1.0, Real max = +1.0 );

    //! Construct
    ScaledSigmoidFunction( PropertySettings& prop );

    //! Destructor
    virtual ~ScaledSigmoidFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Set the slope of ScaledSigmoidFunction
     */
    bool setLambda( const Variant& v );

    /*! \brief Return the slope of ScaledSigmoidFunction
     */
    Variant getLambda();

    /*! \brief Set the minimum value of ScaledSigmoidFunction
     */
    bool setMin( const Variant& v );

    /*! \brief Return the minimum value of ScaledSigmoidFunction
     */
    Variant getMin();

    /*! \brief Set the maximum value of ScaledSigmoidFunction
     */
    bool setMax( const Variant& v );

    /*! \brief Return the maximum value of ScaledSigmoidFunction
     */
    Variant getMax();

    /*! \brief Implement the updating method
     *
     * Details...
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    //! return the approximation commonly used in backpropagation learning: x(1-x)
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! \brief Clone this object
     */
    virtual ScaledSigmoidFunction* clone() const;

    //@}

    //! lambda is the slope of the curve
    Real lambda;
    //! min is the y value when x -> -infinite
    Real min;
    //! max is the y value when x -> +infinite
    Real max;
};

/*! \brief Linear Function
 *
 * <pre>
 *  maxY            /---------------
 *                 /
 *                /
 *               /
 *  minY -------/
 *           minX   maxX
 * </pre>
 * Further Details coming soon ;-)
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>minX</td> <td>Real</td> <td>read/write</td> <td> function's X minimun value </td> <td>this</td> </tr>
 *   <tr><td>maxX</td> <td>Real</td> <td>read/write</td> <td> function's X maximun value </td> <td>this</td> </tr>
 *   <tr><td>minY</td> <td>Real</td> <td>read/write</td> <td> function's Y minimun value </td> <td>this</td> </tr>
 *   <tr><td>maxY</td> <td>Real</td> <td>read/write</td> <td> function's Y maximun value </td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API LinearFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a linear updater
    LinearFunction( Real minX, Real maxX, Real minY = -1, Real maxY = +1 );

    //! Construct
    LinearFunction( PropertySettings& prop );

    //! Destructor
    virtual ~LinearFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Set the minimum value of Xs LinearFunction
     */
    bool setMinX( const Variant& v );

    /*! \brief Return the minimum value of Xs LinearFunction
     */
    Variant getMinX();

    /*! \brief Set the maximum value of Xs LinearFunction
     */
    bool setMaxX( const Variant& v );

    /*! \brief Return the maximum value of Xs LinearFunction
     */
    Variant getMaxX();

    /*! \brief Set the minimum value of Ys LinearFunction
     */
    bool setMinY( const Variant& v );

    /*! \brief Return the minimum value of Ys LinearFunction
     */
    Variant getMinY();

    /*! \brief Set the maximum value of Ys LinearFunction
     */
    bool setMaxY( const Variant& v );

    /*! \brief Return the maximum value of Ys LinearFunction
     */
    Variant getMaxY();

    /*! \brief Implement the updating method
     *
     * Details...
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    //! return the m coefficient if x is in [minX, maxX] and x(1-x) otherwise
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! \brief Clone this object
     */
    virtual LinearFunction* clone() const;

    //@}

    //! minX
    Real minX;
    //! maxX
    Real maxX;
    //! minY
    Real minY;
    //! maxY
    Real maxY;
};

/*! \brief Step Function
 *
 * Further Details coming soon ;-)
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>min</td> <td>Real</td> <td>read/write</td> <td> function's minimun value </td> <td>this</td> </tr>
 *   <tr><td>max</td> <td>Real</td> <td>read/write</td> <td> function's maximun value </td> <td>this</td> </tr>
 *   <tr><td>threshold</td> <td>Real</td> <td>read/write</td> <td> X position of step</td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API StepFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a step updater
    StepFunction( Real min = 0.0f, Real max = 1.0f, Real threshold = 0.0f );

    //! Construct
    StepFunction( PropertySettings& prop );

    //! Destructor
    virtual ~StepFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Set the minimum value of StepFunction
     */
    bool setMin( const Variant& v );

    /*! \brief Return the minimum value of StepFunction
     */
    Variant getMin();

    /*! \brief Set the maximum value of StepFunction
     */
    bool setMax( const Variant& v );

    /*! \brief Return the maximum value of StepFunction
     */
    Variant getMax();

    /*! \brief Set the threshold of StepFunction
     */
    bool setThreshold( const Variant& v );

    /*! \brief Return the threshold of StepFunction
     */
    Variant getThreshold();

    /*! \brief Implement the updating method
     *
     * Details...
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    //! Using the derivate of the sigmoid function!!! 
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! \brief Clone this object
     */
    virtual StepFunction* clone() const;

    //@}

    //! Minimum value 
    Real min;
    //! Maximum value
    Real max;
    //! Threshold
    Real threshold;
};

/*! \brief Pool of Functions
 *
 *  Further Details coming soon ;-)
 *  \par Warnings
 *  The dimension of PoolFunction must be at least one.
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>size</td> <td>unsigned int</td> <td>read-only</td> <td>this's dimesion </td> <td>this</td> </tr>
 *   <tr><td>functions</td> <td>OutputFunction [V]</td> <td>read/write</td> <td>OutputFunction vector</td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API PoolFunction : public OutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! \brief Constructor
     *
     *  Construct a PoolFunction contains dim elements all equal to prototype passed
     */
    PoolFunction( const OutputFunction& prototype, u_int dim );

    //! Construct
    PoolFunction( PropertySettings& prop );

    /*! \brief Constructor
     *
     *  Construct a PoolFunction with dimension dim, but with <b>unintialized</b> OutputFunction
     */
    PoolFunction( u_int dim = 1 );

    /*! \brief Destructor
     */
    ~PoolFunction();

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Return the i-th OutputFunction setted
     */
    OutputFunction* getOutputFunction( u_int i );

    /*! \brief Set the i-th element of this Pool to a OutputFunction of type prototype
     */
    void setOutputFunction( u_int i, const OutputFunction& prototype );

    /*! \brief Return the i-th OutputFunction setted (Variant version)
     */
    Variant getOutputFunctionV( u_int i );

    /*! \brief Set the i-th element (Variant version)
     */
    bool setOutputFunction( u_int i, const Variant& prototype );

    /*! \brief Return the dimension
     */
    unsigned int size();

    /*! \brief Return the dimension (Variant version)
     */
    Variant sizeV();

    /*! \brief Implement the updating method
     *
     *  Apply OutputFunctions setted by setOutputFunction to elements of RealVec inputs
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! \brief Clone this object
     */
    virtual PoolFunction* clone() const;

    /*! \brief when it's inserted into a Cluster, it will be resized to fit the size of Cluster
     */
    virtual void setCluster( Cluster* );

    //@}

private:
    //! Vector of OutputFunction
    VectorData<OutputFunction*> ups;
};

}

#endif


