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

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief IdentityFunction
 *
 * IdentityFunction copyies the inputs to the outputs
 */
class  IdentityFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct
    IdentityFunction() : DerivableOutputFunction() { /* Nothing To Do */ };

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
 */
class  SigmoidFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a sigmoid updater with parameter l
    SigmoidFunction( Real l = 1.0 ) {
        lambda = l;
    };

    //! Destructor
    virtual ~SigmoidFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

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
 */
class  FakeSigmoidFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a sigmoid updater with parameter l
    FakeSigmoidFunction( Real l = 1.0 ) {
        lambda = l;
    };

    //! Destructor
    virtual ~FakeSigmoidFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

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
 */
class  ScaledSigmoidFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a scaled sigmoid updater with parameter l
    ScaledSigmoidFunction( Real l = 1.0,
                          Real min = -1.0, Real max = +1.0 ) {
        lambda = l;
        this->min = min;
        this->max = max;
    };

    //! Destructor
    virtual ~ScaledSigmoidFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

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
 */
class  LinearFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a linear updater
    LinearFunction( Real minX, Real maxX, Real minY = -1, Real maxY = +1 ) {
        this->minX = minX;
        this->maxX = maxX;
        this->minY = minY;
        this->maxY = maxY;
    };

    //! Destructor
    virtual ~LinearFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

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
 */
class  StepFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct a step updater
    StepFunction( Real min = 0.0f, Real max = 1.0f, Real threshold = 0.0f ) {
        this->min = min;
		this->max = max;
		this->threshold = threshold;
    };

    //! Destructor
    virtual ~StepFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

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
 */
class  PoolFunction : public OutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! \brief Constructor
     *
     *  Construct a PoolFunction contains dim elements all equal to prototype passed
     */
    PoolFunction( const OutputFunction& prototype, u_int dim );

    /*! \brief Constructor
     *
     *  Construct a PoolFunction with dimension dim, but with <b>unintialized</b> OutputFunction
     */
    PoolFunction( u_int dim );

    /*! \brief Destructor
     */
    ~PoolFunction();

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Set the i-th element of this Pool to a OutputFunction of type prototype
     */
    void setOutputFunction( u_int i, const OutputFunction& prototype );

    /*! \brief Implement the updating method
     *
     *  Apply OutputFunctions setted by setOutputFunction to elements of RealVec inputs
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! \brief Clone this object
     */
    virtual PoolFunction* clone() const;

    //@}

private:
    //! Vector of OutputFunction
    VectorData<OutputFunction*> ups;
};

}

#endif


