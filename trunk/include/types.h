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
	#else
		#define NNFW_API __declspec(dllimport)
	#endif
#else
	#define NNFW_API
#endif

#include <vector>
#include <string>

// --- Define NNFW_DOUBLE_PRECISION for defining Real as double
//#define  NNFW_DOUBLE_PRECISION

#define TYPES_INCLUDES

#include "messages.h"
#include "primtypes.h"
#include "observ.h"
#include "vectordata.h"
#include "matrixdata.h"

namespace nnfw {
    class RealMat;
}
#include "realvec.h"
#include "realmat.h"

#undef TYPES_INCLUDES


namespace nnfw {

//! Array of Boolean
typedef VectorData<bool> BoolVec;

//! Array of Unsigned Integer
typedef VectorData<u_int> U_IntVec;

class Updatable;
//! Array of Updatable
typedef VectorData<Updatable*> UpdatableVec;

class Cluster;
//! Array of Clusters
typedef VectorData<Cluster*> ClusterVec;

class Linker;
//! Array of Linkers
typedef VectorData<Linker*> LinkerVec;

//! Empty LinkerVec constant
extern const LinkerVec emptyLinkerVec;

//! Empty ClusterVec constant
extern const ClusterVec emptyClusterVec;

class BaseTeachBlock;
//! Array of Updatable
typedef VectorData<BaseTeachBlock*> TeachBlockVec;

}

#endif
