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

#ifndef LIBPERIODICFUNCTIONS_H
#define LIBPERIODICFUNCTIONS_H

#include "types.h"

/*! \file
 *  \brief Library of Periodic OutputFunction
 *
 */

#include "outputfunction.h"

namespace nnfw {

/*! \brief PeriodicFunction
 *
 */
class NNFW_API PeriodicFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Construct */
	PeriodicFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~PeriodicFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Implement the Periodic function */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs ) = 0;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * This sets parameters for the base class, remember to call it in the
	 * configure function of child classes.
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
	 * This saves parameters for the base class, remember to call it in the
	 * configure function of child classes
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
	/*! \name Parameters */
	//@{
	double phase;
	double span;
	double amplitude;
	//@}
};

/*! \brief SawtoothFunction
 *
 */
class NNFW_API SawtoothFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Construct */
	SawtoothFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~SawtoothFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Implement the Sawtooth function */
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
};

/*! \brief TriangleFunction
 *
 */
class NNFW_API TriangleFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Construct */
	TriangleFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~TriangleFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Implement the Triangle function */
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
};

/*! \brief SinFunction
 *
 */
class NNFW_API SinFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Construct */
	SinFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~SinFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Return the frequency of the sinusoidal wave
	 *  \param frequency is equal to 2*pi_greco/span
	 */
	double frequency();
	/*! Implement the Sin function */
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
};

/*! \brief PseudoGaussFunction
 *
 */
class NNFW_API PseudoGaussFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Construct */
	PseudoGaussFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~PseudoGaussFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Implement the Triangle function */
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
};

}

#endif

