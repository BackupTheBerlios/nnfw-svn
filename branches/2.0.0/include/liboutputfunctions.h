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

#ifndef LIBOUTPUTFUNCTIONS_H
#define LIBOUTPUTFUNCTIONS_H

#include "types.h"

/*! \file
 *  \brief Library of Common OutputFunction
 *
 *  \todo ComboLinearFunction, a subclass of OutputFunction that constructs a linear combination of OutputFunctions:
 *        example: 4*Sigmoid(x)+2*Linear(x)
 */

#include "outputfunction.h"
#include "derivableoutputfunction.h"

namespace nnfw {

/*! \brief IdentityFunction
 *
 * IdentityFunction copyies the inputs to the outputs
 */
class NNFW_API IdentityFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct */
    IdentityFunction();
    /*! Destructor */
    virtual ~IdentityFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Implement the identity function */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! return always 1 (an explain of why will be coming soon) */
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! Clone this object
     */
    virtual IdentityFunction* clone() const;

    //@}
};

/*! \brief ScaleFunction
 *
 * ScaleFunction scales the inputs vector to the outputs
 */
class NNFW_API ScaleFunction : public OutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct */
    ScaleFunction( double rate = 1.0 );
    /*! Destructor */
    virtual ~ScaleFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Set the rate of Scaling function */
    bool setRate( double v );

    /*! Return the rate of Scaling function */
    double getRate();

    /*! Implement the identity function */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! Clone this object */
    virtual ScaleFunction* clone() const;

    //@}
private:
	double rate;
};

/*! \brief GainFunction
 *
 * GainFunction add a constan value to inputs
 */
class NNFW_API GainFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	GainFunction( double gain = 1.0 );
	/*! Destructor */
	virtual ~GainFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Set the Gain */
	bool setGain( double v );
	
	/*! Return the Gain function */
	double gain();
	
	/*! Implement the Gain function */
	virtual void apply( RealVec& inputs, RealVec& outputs );
	
	/*! Clone this object */
	virtual GainFunction* clone() const;
	
	//@}
private:
	double gainv;
};

/*! \brief Sigmoid Function
 *
 * Details..
 */
class NNFW_API SigmoidFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a sigmoid updater with parameter l */
    SigmoidFunction( double l = 1.0 );
    /*! Destructor */
    virtual ~SigmoidFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Set the slope of SigmoidFunction */
    bool setLambda( double v );

    /*! Return the slope of SigmoidFunction */
    double getLambda();

    /*! Implement the updating method */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! return the approximation commonly used in backpropagation learning: x(1-x) */
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! Clone this object */
    virtual SigmoidFunction* clone() const;

    //@}
private:
    /*! lambda is the slope of the curve */
    double lambda;
};

/*! \brief Fake Sigmoid Function !! Is a linear approximation of sigmoid function
 *
 */
class NNFW_API FakeSigmoidFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a sigmoid updater with parameter l */
    FakeSigmoidFunction( double l = 1.0 );
    /*! Destructor */
    virtual ~FakeSigmoidFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Set the slope of FakeSigmoidFunction */
    bool setLambda( double v );

    /*! Return the slope of FakeSigmoidFunction */
    double getLambda();

    /*! Implement the updating method */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! return the approximation commonly used in backpropagation learning: x(1-x) */
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! Clone this object */
    virtual FakeSigmoidFunction* clone() const;

    //@}

    /*! lambda is the slope of the curve */
    double lambda;
};

/*! \brief ScaledSigmoid Function
 *
 * lambda is the slope of the curve<br>
 * min is the y value when x -> -infinite <br>
 * max is the y value when x -> +infinite <br>
 * (max-min) is the y value when x == 0
 *
 */
class NNFW_API ScaledSigmoidFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a scaled sigmoid updater with parameter l */
    ScaledSigmoidFunction( double l = 1.0, double min = -1.0, double max = +1.0 );
    /*! Destructor */
    virtual ~ScaledSigmoidFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Set the slope of ScaledSigmoidFunction */
    bool setLambda( double v );

    /*! Return the slope of ScaledSigmoidFunction */
    double getLambda();

    /*! Set the minimum value of ScaledSigmoidFunction */
    bool setMin( double v );

    /*! Return the minimum value of ScaledSigmoidFunction */
    double getMin();

    /*! Set the maximum value of ScaledSigmoidFunction */
    bool setMax( double v );

    /*! Return the maximum value of ScaledSigmoidFunction */
    double getMax();

    /*! Implement the updating method */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! return the approximation commonly used in backpropagation learning: x(1-x) */
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! Clone this object */
    virtual ScaledSigmoidFunction* clone() const;

    //@}

    /*! lambda is the slope of the curve */
    double lambda;
    /*! min is the y value when x -> -infinite */
    double min;
    /*! max is the y value when x -> +infinite */
    double max;
};

/*! \brief Ramp Function
 *
 *  \warning The old name (before version 1.1.0) of RampFunction was LinearFunction while now 
 *  LinearFunction express a simple linear equation y=mx+b
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
 */
class NNFW_API RampFunction : public DerivableOutputFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct a linear updater */
	RampFunction( double minX, double maxX, double minY = -1, double maxY = +1 );
	/*! Destructor */
	virtual ~RampFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Set the minimum value of Xs RampFunction */
	bool setMinX( double v );
	
	/*! Return the minimum value of Xs RampFunction */
	double minX();
	
	/*! Set the maximum value of Xs RampFunction */
	bool setMaxX( double v );
	
	/*! Return the maximum value of Xs RampFunction */
	double maxX();
	
	/*! Set the minimum value of Ys RampFunction */
	bool setMinY( double v );
	
	/*! Return the minimum value of Ys RampFunction */
	double minY();
	
	/*! Set the maximum value of Ys RampFunction */
	bool setMaxY( double v );
	
	/*! Return the maximum value of Ys RampFunction */
	double maxY();
	
	/*! Implement the updating method */
	virtual void apply( RealVec& inputs, RealVec& outputs );
	
	/*! return the m coefficient if x is in [minX, maxX] and x(1-x) otherwise */
	virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;
	
	/*! Clone this object */
	virtual RampFunction* clone() const;
	
	//@}
	
	/*! minX */
	double min_x;
	/*! maxX */
	double max_x;
	/*! minY */
	double min_y;
	/*! maxY */
	double max_y;
};

/*! \brief Linear equation Function
 *
 *  \warning The old name (before version 1.1.0) of RampFunction was LinearFunction while now 
 *  LinearFunction express a simple linear equation y=mx+b
 *
 *  Implements a linear equation y = m*x + b
 *
 */
class NNFW_API LinearFunction : public DerivableOutputFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct a linear updater */
	LinearFunction( double m, double b );
	/*! Destructor */
	virtual ~LinearFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Set the m coeffiecient of LinearFunction */
	bool setM( double v );
	
	/*! Return the m coeffiecient of LinearFunction */
	double m();
	
	/*! Set the b coeffiecient of LinearFunction */
	bool setB( double v );
	
	/*! Return the b coeffiecient of Linear Function */
	double b();
	
	/*! Implement the updating method */
	virtual void apply( RealVec& inputs, RealVec& outputs );
	
	/*! return the m coefficient */
	virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;
	
	/*! Clone this object */
	virtual LinearFunction* clone() const;
	
	//@}
	
	/*! m */
	double mv;
	/*! b */
	double bv;
};

/*! \brief Step Function
 *
 * Further Details coming soon ;-)
 *
 */
class NNFW_API StepFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a step updater */
    StepFunction( double min = 0.0f, double max = 1.0f, double threshold = 0.0f );
    /*! Destructor */
    virtual ~StepFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Set the minimum value of StepFunction */
    bool setMin( double v );

    /*! Return the minimum value of StepFunction */
    double getMin();

    /*! Set the maximum value of StepFunction */
    bool setMax( double v );

    /*! Return the maximum value of StepFunction */
    double getMax();

    /*! Set the threshold of StepFunction */
    bool setThreshold( double v );

    /*! Return the threshold of StepFunction */
    double getThreshold();

    /*! Implement the updating method */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! Using the derivate of the sigmoid function!!!  */
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! Clone this object */
    virtual StepFunction* clone() const;

    //@}

    /*! Minimum value  */
    double min;
    /*! Maximum value */
    double max;
    /*! Threshold */
    double threshold;
};

/*! \brief LeakyIntegrator Function !! 
 *
 */
class NNFW_API LeakyIntegratorFunction : public OutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a LeakyIntegrator with deltas specified */
    LeakyIntegratorFunction( const RealVec& d );
    /*! Destructor */
    virtual ~LeakyIntegratorFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Return the i-th Delta setted */
    RealVec& getDelta() {
		return delta;
	};

    /*! Set the i-th Delta */
    void setDelta( const RealVec& v ) {
		delta.assign( v );
	};

    /*! Zeroing the status */
	void zeroingStatus();

    /*! Implement the updating method<br>
     * it computes: y(t) <- delta * y(t-1) + (1.0-delta) * inputs
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! Clone this object */
    virtual LeakyIntegratorFunction* clone() const;

    /*! resize itself to fit the size of Cluster */
    virtual void setCluster( Cluster* );

    //@}

    /*! delta is the leak rate of the function */
    RealVec delta;
	/*! previous outputs */
	RealVec outprev;
};

/*! \brief LogLike Function !! 
 *
 *  It compute the following equation:
 *  \f[
 *     y = \frac{x}{ 1+Ax+B }
 *  \f]
 *
 */
class NNFW_API LogLikeFunction : public OutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a LogLike with deltas specified */
    LogLikeFunction( double A=1.0, double B=5.0 );
    /*! Destructor */
    virtual ~LogLikeFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Return the A coefficient setted */
    double A() {
		return a;
	};

    /*! Set the A coefficient */
    void setA( double na ) {
		a = na;
	};

    /*! Return the B coefficient setted */
    double B() {
		return b;
	};

    /*! Set the B coefficient */
    void setB( double nb ) {
		b = nb;
	};

    /*! Implement the updating method */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! Clone this object */
    virtual LogLikeFunction* clone() const;

    //@}

private:
	/*! A coefficient */
	double a;
	/*! B coefficient */
	double b;
};

/*! \brief Pool of Functions
 *
 *  Further Details coming soon ;-)
 *  \par Warnings
 *  The dimension of PoolFunction must be at least one.
 *
 */
class NNFW_API PoolFunction : public OutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a PoolFunction contains dim elements all equal to prototype passed */
    PoolFunction( const OutputFunction& prototype, unsigned int dim );
    /*! Construct a PoolFunction with dimension dim, but with <b>unintialized</b> OutputFunction */
    PoolFunction( unsigned int dim = 1 );
    /*! Destructor */
    ~PoolFunction();

    //@}
    /*! \name Interface */
    //@{

    /*! Return the i-th OutputFunction setted */
    OutputFunction* getOutputFunction( unsigned int i );

    /*! Set the i-th element of this Pool to a OutputFunction of type prototype */
    void setOutputFunction( unsigned int i, const OutputFunction& prototype );

    /*! Return the dimension */
    unsigned int size();

    /*! Implement the updating method <br>
     *  Apply OutputFunctions setted by setOutputFunction to elements of RealVec inputs
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! Clone this object */
    virtual PoolFunction* clone() const;

    /*! when it's inserted into a Cluster, it will be resized to fit the size of Cluster */
    virtual void setCluster( Cluster* );

    //@}

private:
    /*! Vector of OutputFunction */
    QVector<OutputFunction*> ups;
};

/*! \brief Composite Function !! 
 *
 */
class NNFW_API CompositeFunction : public OutputFunction {
public:
    /*! \name Constructors */
    //@{
    /*! Construct a Composite */
    CompositeFunction( const OutputFunction& f, const OutputFunction& g );
    /*! Destructor */
    virtual ~CompositeFunction();

    //@}
    /*! \name Interface */
    //@{

    /*! Set the first function of CompositeFunction */
    bool setFirstFunction( const OutputFunction& f );

    /*! Return the first function of CompositeFunction */
    OutputFunction* getFirstFunction();

    /*! Set the second function of CompositeFunction */
    bool setSecondFunction( const OutputFunction& g );

    /*! Return the second function of CompositeFunction */
    OutputFunction* getSecondFunction();

    /*! Implement the updating method <br>
     * it computes: y(t) <- second( first( input, mid ), outputs ) <br>
	 * where mid is a private vector that traces the outputs of first function
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! Clone this object */
    virtual CompositeFunction* clone() const;

    /*! recursive call setCluster on first and second function setted */
    virtual void setCluster( Cluster* );

    //@}

	//--- functions
	OutputFunction* first;
	OutputFunction* second;
	//--- intermediate result
	RealVec mid;
	//--- Cluster
	Cluster* cl;
};

/*! \brief Linear Combination of Two Function !! 
 *
 */
class NNFW_API LinearComboFunction : public OutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct a Linear Combination of two functions */
    LinearComboFunction( double w1, const OutputFunction& f, double w2, const OutputFunction& g );
    /*! Destructor */
    virtual ~LinearComboFunction();

    //@}
    /*! \name Interface */
    //@{

    /*! Set the first function of LinearComboFunction */
    bool setFirstFunction( const OutputFunction& f );
    /*! Return the first function of LinearComboFunction */
    OutputFunction* getFirstFunction();
    /*! Set the first weight of LinearComboFunction */
    bool setFirstWeight( double v );
    /*! Return the first weight of LinearComboFunction */
    double getFirstWeight();

    /*! Set the second function of CompositeFunction */
    bool setSecondFunction( const OutputFunction& g );
    /*! Return the second function of CompositeFunction */
    OutputFunction* getSecondFunction();
    /*! Set the second weight of LinearComboFunction */
    bool setSecondWeight( double v );
    /*! Return the second weight of LinearComboFunction */
    double getSecondWeight();

    /*! Implement the updating method <br>
     * it computes: y(t) <- w1*first(input,output) + w2*second(input,outputs)
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! Clone this object */
    virtual LinearComboFunction* clone() const;
    /*! recursive call setCluster on first and second function setted */
    virtual void setCluster( Cluster* );

    //@}

	//--- functions
	OutputFunction* first;
	OutputFunction* second;
	//--- temporary result
	RealVec mid;
	//--- weights
	double w1, w2;
	//--- Cluster
	Cluster* cl;
};

}

#endif
