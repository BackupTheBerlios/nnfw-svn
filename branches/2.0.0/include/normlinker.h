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

#ifndef NORMLINKER_H
#define NORMLINKER_H

/*! \file
 */

#include "types.h"
#include "matrixlinker.h"

namespace nnfw {

/*! \brief NormLinker Class
 *
 */
class NNFW_API NormLinker : public MatrixLinker {
public:
	/*! \name Constructors */
	//@{
	/*! Connect clusters with a complete connections */
	NormLinker( Cluster* from, Cluster* to, QString name = "unnamed" );
	/*! Constructor */
	NormLinker( ConfigurationParameters& params, QString prefix );
	/*! Destructor */
	virtual ~NormLinker();
	//@}
	/*! \name Interface */
	//@{
	/*! Performs the euclidean norm calculation */
	void update();
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
protected:
	// temp data
	DoubleVector temp;
};

}

#endif
