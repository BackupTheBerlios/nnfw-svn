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

#ifndef TYPES_H
#define TYPES_H

/*! \file
 *  \brief This file contains the common type defitions used on the whole framework
 *
 *  Details...
 *
 *
 */

#ifdef WIN32
	#define _CRT_SECURE_NO_DEPRECATE
	#ifdef NNFW_BUILDING_DLL
		#define NNFW_API __declspec(dllexport)
		#define NNFW_TEMPLATE __declspec(dllexport)
	#else
		#define NNFW_API __declspec(dllimport)
		#define NNFW_TEMPLATE 
	#endif
	// used for: create the static library, and for hide symbols when generating DLL
	#ifdef NNFW_DONT_EXPORT
		#define NNFW_API 
		#define NNFW_TEMPLATE 
	#endif
	#define NNFW_INTERNAL 
#else
	#ifdef NNFW_DONT_EXPORT
		#define NNFW_API __attribute__ ((visibility ("hidden")))
		#define NNFW_TEMPLATE __attribute__ ((visibility ("hidden")))
	#else
		#define NNFW_API
		#define NNFW_TEMPLATE 
	#endif
	#define NNFW_INTERNAL __attribute__ ((visibility ("hidden")))
#endif

#define NNFW_VERSION 20000

#include <QList>
#include <QVector>
#include <QMap>
#include <QString>
#include <QDebug>

#include "vectors.h"
#include "matrices.h"
#include "algebra.h"

namespace nnfw {

class Updatable;
/*! Array of Updatable */
typedef QList<Updatable*> UpdatableList;

class Cluster;
/*! Array of Clusters */
typedef QList<Cluster*> ClusterList;

class Linker;
/*! Array of Linkers */
typedef QList<Linker*> LinkerList;

}

#endif
