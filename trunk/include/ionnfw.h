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

#ifndef IONNFW_H
#define IONNFW_H

#include "types.h"
#include "propertized.h"
#include <iostream>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

//! Operator << with RealVec
std::ostream& operator<<(std::ostream& stream, const RealVec& v);
//! Operator >> with RealVec
std::istream& operator>>(std::istream& stream, RealVec& v);

//! Operator << with RealMat
std::ostream& operator<<(std::ostream& stream, const RealMat& m);
//! Operator >> with RealMat
std::istream& operator>>(std::istream& stream, RealMat& m);

//! Operator << with Variant::types
std::ostream& operator<<(std::ostream& stream, const Variant::types t);
//! Operator >> with Variant::types
std::istream& operator>>(std::istream& stream, Variant::types& t);

//! Operator << with Variant (print the value)
std::ostream& operator<<(std::ostream& stream, const Variant var);
//! Operator >> with Variant (read the value)
std::istream& operator>>(std::istream& stream, Variant& var);

//! Operator << with Propertized
std::ostream& operator<<(std::ostream& stream, const Propertized& p);
//! Operator >> with Propertized
std::istream& operator>>(std::istream& stream, Propertized& p);

}

#endif