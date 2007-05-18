/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
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

#ifndef BACKPROPAGATIONALGO_H
#define BACKPROPAGATIONALGO_H

/*! \file
 */

#include "types.h"
#include "learningalgorithm.h"

namespace nnfw {

/*! \brief Back-Propagation Algorithm implementation
 *
 */
class NNFW_API BackPropagationAlgo : public LearningAlgorithm {
public:
	/*! \name Constructors */
	//@{

    //! Constructor
    BackPropagationAlgo( BaseNeuralNet* );
    //! Destructor
    virtual ~BackPropagationAlgo();

	//@}
	/*! \name Interface */
	//@{

    //! Modify the object
    virtual void learn();

	//@}
};

}

#endif

