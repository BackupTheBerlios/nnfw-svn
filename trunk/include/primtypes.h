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

// --- You can't include it directly
#ifndef TYPES_INCLUDES
#error "You can't include primtypes.h directly; Instead, You have to include types.h"
// --- follow define avoid to get a lot of understandable error !
#define PRIMTYPES_H
#endif

#ifndef PRIMTYPES_H
#define PRIMTYPES_H

/*! \file
 *  \brief This file contains definition of primitive types ; Don't include this file directly, instead include types.h
 */


namespace nnfw {

/*! Unsigned integer */
typedef  unsigned int u_int;

/*! Abstraction on the type of real numbers */
#ifndef NNFW_DOUBLE_PRECISION
typedef float Real;
#else
typedef double Real;
#endif

}

#endif

