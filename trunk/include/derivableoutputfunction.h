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

#ifndef DERIVABLEOUTPUTFUNCTION_H
#define DERIVABLEOUTPUTFUNCTION_H

/*! \file
 *  \brief This file contains the declaration of the abstract DerivableOutputFunction Class
 *
 *  Details...
 *
 */

#include "types.h"
#include "outputfunction.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief DerivableOutputFunction Class
 *
 *  Details...
 */
class  DerivableOutputFunction : public OutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Constructor
    DerivableOutputFunction() : OutputFunction() { /* Nothing to do */ };
    //! Destructor
    virtual ~DerivableOutputFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Compute the derivate of the function represent
     *
     *  Given the input of neurons and the corresponding output of neurons, it calculate
     *  the derivate
     */
    virtual void derivate( const RealVec& inputs, const RealVec&, RealVec& derivates ) const {
        derivates.assign( inputs );
    };

    //@}
	
};

}

#endif