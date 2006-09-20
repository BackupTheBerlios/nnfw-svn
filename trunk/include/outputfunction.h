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

#ifndef OUTPUTFUNCTION_H
#define OUTPUTFUNCTION_H

/*! \file
 *  \brief This file contains the declaration of the abstract OutputFunction Class
 *
 *  Details...
 *
 */

#include "types.h"
#include "clonable.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief OutputFunction Class
 *
 *  Details...
 */
class OutputFunction : public Clonable {
public:
    /*! \name Constructors */
    //@{
    //! Constructor
    OutputFunction() : tmp1(1), tmp2(1) { /* Nothing else to do */ };

    //! Destructor
    virtual ~OutputFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Calculate the outputs of neurons by the net inputs given
     */
    virtual void apply( RealVec& inputs, RealVec& outputs );

    /*! \brief Calculate the outputs of a single neuron
     */
    Real apply( Real input ) {
        tmp1[0] = input;
        apply( tmp1, tmp2 );
        return tmp2[0];
    };

    /*! \brief Clone this object
     */
    virtual OutputFunction* clone() const;

    //@}

private:
    //! temporary RealVec for speed-up apply with a single value
    RealVec tmp1;
    //! temporary RealVec for speed-up apply with a single value
    RealVec tmp2;
};

}

#endif
