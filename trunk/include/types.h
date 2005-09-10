/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005 Gianluca Massera <emmegian@yahoo.it>                     *
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
 */

#include <vector>
#include <map>
#include <string>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

//! Generic Vector
#define nnfwVector std::vector

//! Generic Map
#define nnfwMap std::map

//! Generic Pair
#define nnfwPair std::pair

//! Unsigned integer
typedef unsigned int u_int;

//! Abstraction on the type of real numbers
typedef float nnfwReal;
//! Vector of nnfwReal
typedef nnfwVector<nnfwReal> nnfwRealVec;

class Updatable;
//! Array of Updatable
typedef nnfwVector<Updatable*> UpdatableVec;

class Cluster;
//! Array of Clusters
typedef nnfwVector<Cluster*> ClusterVec;

class Linker;
//! Array of Linkers
typedef nnfwVector<Linker*> LinkerVec;

//! Empty LinkerVec constant
extern LinkerVec emptyLinkerVec;

//! Empty ClusterVec constant
extern ClusterVec emptyClusterVec;

}

#endif
