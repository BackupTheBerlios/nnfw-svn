/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2011 Gianluca Massera <emmegian@yahoo.it>                *
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

#ifndef LIBCOMPETITIVEFUNCTIONS_H
#define LIBCOMPETITIVEFUNCTIONS_H

#include "types.h"

/*! \file
 *  \brief Library of Competitive OutputFunction
 *
 */

#include "outputfunction.h"
#include <configuration/configurationparameters.h>

namespace nnfw {

/*! \brief WinnerTakeAllFunction
 *
 */
class NNFW_API WinnerTakeAllFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Construct */
	WinnerTakeAllFunction( double value = 1.0 );
	/*! Destructor */
	virtual ~WinnerTakeAllFunction() { /* Nothing to do */ };
	//@}
	/*! \name Interface */
	//@{
	/*! Set the value */
	bool setValue( double v );
	/*! Return the value */
	double value();
	/*! Implement the WinnerTakeAllFunction function */
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

private:
	double valuev;
};

}

#endif

