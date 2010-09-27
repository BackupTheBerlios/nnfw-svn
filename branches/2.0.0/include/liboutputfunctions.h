/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>                *
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
 */

#include "outputfunction.h"
#include <configuration/configurationparameters.h>
#include <memory>

namespace nnfw {

/*! \brief IdentityFunction
 *
 * IdentityFunction copyies the inputs to the outputs
 */
class NNFW_API IdentityFunction : public OutputFunction {
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
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! return always 1 (an explain of why will be coming soon) */
	virtual bool derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
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
	/*! Implement the identity function */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! return always the rate (an explain of why will be coming soon) */
	virtual bool derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	double rate;
	//@}
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
	/*! Implement the Gain function */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! return always 1 (an explain of why will be coming soon) */
	virtual bool derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	double gainv;
	//@}
};

/*! \brief Sigmoid Function
 *
 * Details..
 */
class NNFW_API SigmoidFunction : public OutputFunction {
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
	/*! Implement the updating method */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! return the approximation commonly used in backpropagation learning: y*(1-y) */
	virtual bool derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	/*! lambda is the slope of the curve */
	double lambda;
	//@}
};

/*! \brief Fake Sigmoid Function !! Is a linear approximation of sigmoid function
 *
 */
class NNFW_API FakeSigmoidFunction : public OutputFunction {
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
	/*! Implement the updating method */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! return the approximation commonly used in backpropagation learning: x(1-x) */
	virtual bool derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	/*! lambda is the slope of the curve */
	double lambda;
	//@}
};

/*! \brief ScaledSigmoid Function
 *
 * lambda is the slope of the curve<br>
 * min is the y value when x -> -infinite <br>
 * max is the y value when x -> +infinite <br>
 * (max-min) is the y value when x == 0
 *
 */
class NNFW_API ScaledSigmoidFunction : public OutputFunction {
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
	/*! Implement the updating method */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! return the approximation commonly used in backpropagation learning: x(1-x) */
	virtual bool derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	/*! lambda is the slope of the curve */
	double lambda;
	/*! min is the y value when x -> -infinite */
	double min;
	/*! max is the y value when x -> +infinite */
	double max;
	//@}
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
class NNFW_API RampFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Default constructor*/
	RampFunction();
	/*! Construct a linear updater */
	RampFunction( double minX, double maxX, double minY = -1, double maxY = +1 );
	/*! Destructor */
	virtual ~RampFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Implement the updating method */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! return the m coefficient if x is in [minX, maxX] and x(1-x) otherwise */
	virtual bool derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	/*! minX */
	double min_x;
	/*! maxX */
	double max_x;
	/*! minY */
	double min_y;
	/*! maxY */
	double max_y;
	//@}
};

/*! \brief Linear equation Function
 *  Implements a linear equation y = m*x + b
 *
 *  \warning The old name (before version 1.1.0) of RampFunction was LinearFunction while now
 *  LinearFunction express a simple linear equation y=mx+b
 */
class NNFW_API LinearFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Default constructor */
	LinearFunction();
	/*! Construct a linear updater */
	LinearFunction( double m, double b );
	/*! Destructor */
	virtual ~LinearFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Implement the updating method */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! return the m coefficient */
	virtual bool derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	/*! m */
	double m;
	/*! b */
	double b;
	//@}
};

/*! \brief Step Function
 *
 * Further Details coming soon ;-)
 *
 */
class NNFW_API StepFunction : public OutputFunction {
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
	/*! Implement the updating method */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! Using the derivate of the sigmoid function!!!  */
	virtual bool derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	/*! Minimum value  */
	double min;
	/*! Maximum value */
	double max;
	/*! Threshold */
	double threshold;
	//@}
};

/*! \brief LeakyIntegrator Function !! 
 *
 */
class NNFW_API LeakyIntegratorFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Default constructor */
	LeakyIntegratorFunction();
	/*! Construct a LeakyIntegrator with deltas specified */
	LeakyIntegratorFunction( const DoubleVector& deltas );
	/*! Destructor */
	virtual ~LeakyIntegratorFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Zeroing the status */
	void zeroingStatus();
	/*! Implement the updating method<br>
	 * it computes: y(t) <- delta * y(t-1) + (1.0-delta) * inputs
	 */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! resize itself to fit the size of Cluster */
	virtual void setCluster( Cluster* );
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	/*! delta is the leak rate of the function */
	DoubleVector delta;
	/*! previous outputs */
	DoubleVector outprev;
	//@}
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
	/*! Implement the updating method */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	/*! A coefficient */
	double A;
	/*! B coefficient */
	double B;
	//@}
};

/*! \brief Composite Function !!
 *
 * \note Component OutputFunction objects are destroyed by the CompositeFunction
 *       object to which they belong.
 */
class NNFW_API CompositeFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Default constructor */
	CompositeFunction();
	/*! Construct a Composite */
	CompositeFunction( OutputFunction *f, OutputFunction *g );
	/*! Destructor */
	virtual ~CompositeFunction();
	//@}
	/*! \name Interface */
	//@{
	/*! Set the first function of CompositeFunction */
	bool setFirstFunction( OutputFunction *f );
	/*! Return the first function of CompositeFunction */
	OutputFunction& getFirstFunction();
	/*! Set the second function of CompositeFunction */
	bool setSecondFunction( OutputFunction *g );
	/*! Return the second function of CompositeFunction */
	OutputFunction& getSecondFunction();
	/*! Implement the updating method <br>
	 * it computes: y <- second( first( input, mid ), outputs ) <br>
	 * where mid is a private vector that traces the outputs of first function
	 */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! recursive call setCluster on first and second function setted */
	virtual void setCluster( Cluster* );
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
private:
	/*! \name Parameters */
	//@{
	//--- functions
	std::auto_ptr<OutputFunction> first;
	std::auto_ptr<OutputFunction> second;
	//--- intermediate result
	DoubleVector mid;
	//--- Cluster
	Cluster* cl;
	// Copy constructor and copy operator (here to prevent usage)
	CompositeFunction(const CompositeFunction&);
	CompositeFunction& operator=(const CompositeFunction&);
	//@}
};

/*! \brief Linear Combination of Two Function !!
 *
 * \note Component OutputFunction objects are destroyed by the
 *       LinearComboFunction object to which they belong.
 *
 */
class NNFW_API LinearComboFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Standard constructor */
	LinearComboFunction();
	/*! Construct a Linear Combination of two functions */
	LinearComboFunction( double w1, OutputFunction *f, double w2, OutputFunction *g );
	/*! Destructor */
	virtual ~LinearComboFunction();
	//@}
	/*! \name Interface */
	//@{
	/*! Set the first function of LinearComboFunction */
	bool setFirstFunction( OutputFunction *f );
	/*! Return the first function of LinearComboFunction */
	OutputFunction& getFirstFunction();
	/*! Set the first weight of LinearComboFunction */
	bool setFirstWeight( double v );
	/*! Return the first weight of LinearComboFunction */
	double getFirstWeight();
	/*! Set the second function of CompositeFunction */
	bool setSecondFunction( OutputFunction *g );
	/*! Return the second function of CompositeFunction */
	OutputFunction& getSecondFunction();
	/*! Set the second weight of LinearComboFunction */
	bool setSecondWeight( double v );
	/*! Return the second weight of LinearComboFunction */
	double getSecondWeight();
	/*! Implement the updating method <br>
	 * it computes: y <- w1*first(input,output) + w2*second(input,outputs)
	 */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! recursive call setCluster on first and second function setted */
	virtual void setCluster( Cluster* );
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
private:
	/*! \name Parameters */
	//@{
	//--- functions
	std::auto_ptr<OutputFunction> first;
	std::auto_ptr<OutputFunction> second;
	//--- temporary result
	DoubleVector mid;
	//--- weights
	double w1, w2;
	//--- Cluster
	Cluster* cl;
	// Copy constructor and copy operator (here to prevent usage)
	LinearComboFunction(const LinearComboFunction&);
	LinearComboFunction& operator=(const LinearComboFunction&);
	//@}
};

}

#endif
