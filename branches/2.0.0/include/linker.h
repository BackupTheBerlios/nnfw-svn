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

#ifndef LINKERS_H
#define LINKERS_H

/*! \file
 *  \brief This file contains the declaration of the linkers implemented
 *
 */

#include "types.h"
#include "cluster.h"
#include "updatable.h"
#include <exception>

namespace nnfw {

/*! \brief Abstract Linker Class. This define the common interface among Linkers
 *
 */
class NNFW_API Linker : public Updatable {
public:
	/*! \name Constructors */
	//@{
	/*! Construct */
	Linker( Cluster* from, Cluster* to, QString name = "unnamed" );
	/*! Constructor */
	Linker( ConfigurationParameters& params, QString prefix );
	//@}
	/*! \name Exceptions throw by Linker */
	//@{
	/*! Thrown when a user attempt to create a Linker without specify the "from" or "to" */
	class NNFW_API ClusterFromOrToMissing : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "The Cluster 'from' or 'to' is missing - Check your configuration file\n";
		};
	};
	//@}
	/*! \name Interface */
	//@{
	/*! Return the Cluster From */
	Cluster* from() const {
		return fromc;
	};
	/*! Return the Cluster to */
	Cluster* to() const {
		return toc;
	};
	/*! Returns the 'size' of the linker<br>
	 * The actual means of 'size' will be specified by sub-classes that implementes concrete linkers
	 */
	virtual unsigned int size() const = 0;
	/*! Randomize the parameters of the Linker<br>
	 * The parameters randomized by this method will be specified by sub-classes
	 */
	virtual void randomize( double min, double max ) = 0;
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * This saves the name property, remember to call this in child classes
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
private:
	/*! incoming Cluster */
	Cluster* fromc;
	/*! outgoing Cluster */
	Cluster* toc;
};

}

#endif
