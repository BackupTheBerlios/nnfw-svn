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

#ifndef LIBRADIALFUNCTIONS_H
#define LIBRADIALFUNCTIONS_H

#include "types.h"

/*! \file
 *  \brief Library of Radial OutputFunction
 *
 */

#include "outputfunction.h"

namespace nnfw {

/*! \brief GaussFunction
 *
 */
class NNFW_API GaussFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Construct */
	GaussFunction( double centre = 0.0, double variance = 1.0, double maxvaule = 1.0 );
	/*! Destructor */
	virtual ~GaussFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Set the Variance */
	bool setVariance( double v );
	/*! Return the variance */
	double variance();
	/*! Implement the Gaussian function */
	virtual void apply( DoubleVector& inputs, DoubleVector& outputs );
	/*! derivate of Gauss function */
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
	/*! Centre of GaussFunction */
	double centre;
	/*! Maximum value of GaussFunction */
	double max;
	//@}
private:
	// variance
	double variancev;
	// minus squared-variance
	double msqrvar;
};

}

#endif

