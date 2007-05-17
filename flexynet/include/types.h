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

#ifndef FLEXTYPES_H
#define FLEXTYPES_H

/*! \file
 *  \brief This file contains the common type defitions used on the whole framework
 *
 *  Details...
 *
 *
 */

#ifdef WIN32
	#define _CRT_SECURE_NO_DEPRECATE
	#ifdef FLEX_BUILDING_DLL
		#define FLEX_API __declspec(dllexport)
	#else
		#define FLEX_API __declspec(dllimport)
	#endif
#else
	#define FLEX_API
#endif

#define TYPES_INCLUDES
// --- for checking the privates includes
// --- see nnfw/vectordata.h
#undef TYPES_INCLUDES


#endif
