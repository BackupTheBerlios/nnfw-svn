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

#ifndef IONNFW_H
#define IONNFW_H

#include "types.h"
#include "propertized.h"
#include <iostream>

/*! \file
 *  \brief Function for I/O operations
 *
 *  \todo XML I/O functions doesn't support name with spaces
 *  \todo there's no support for UTF-8 coding or LOCALE string representation
 *
 */



namespace nnfw {

/*! \name XML load/save */
//@{

class BaseNeuralNet;
/*! Load the net from an XML file, and return a BaseNeuralNet */
NNFW_API BaseNeuralNet* loadXML( const char* filename );
/*! Save the BaseNeuralNet passed into an XML file; return true on success */
NNFW_API bool saveXML( const char* filename, BaseNeuralNet* );

//@}

/*! \name Ouput Stream Operator */
//@{

/*! Operator << with RealVec */
NNFW_API std::ostream& operator<<(std::ostream& stream, const RealVec& v);

/*! Operator << with RealMat */
NNFW_API std::ostream& operator<<(std::ostream& stream, const RealMat& m);

/*! Operator << with Variant::types */
NNFW_API std::ostream& operator<<(std::ostream& stream, const Variant::types t);

/*! Operator << with Variant */
NNFW_API std::ostream& operator<<(std::ostream& stream, const Variant var);

/*! Operator << with Propertized */
NNFW_API std::ostream& operator<<(std::ostream& stream, const Propertized& p);

//@}

}

#endif
