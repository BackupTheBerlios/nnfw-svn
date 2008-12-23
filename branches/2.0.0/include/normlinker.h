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

    /*! Destructor */
    virtual ~NormLinker();

    //@}
    /*! \name Interface */
    //@{

    /*! Performs the euclidean norm calculation
     */
    void update();

	/*! Clone this NormLinker */
	virtual NormLinker* clone() const;
	/*! Return its typename */
	virtual QString typeName() {
		return "NormLinker";
	};

    //@}

protected:
    // temp data
    RealVec temp;
};

}

#endif
