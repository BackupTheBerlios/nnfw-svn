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

#include <vector>
#include <map>
#include <string>
#include "messages.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

//! Generic Vector
#define Vector std::vector

//! Generic Map
#define Map std::map

//! Generic Pair
#define Pair std::pair

//! Unsigned integer
typedef  unsigned int u_int;

//! Abstraction on the type of real numbers
typedef double Real;

}

#include "realvec.h"

namespace nnfw {

//! Array of Unsigned Integer
typedef  Vector<u_int> U_IntVec;
//! Operator <<
inline U_IntVec& operator<<(U_IntVec& vec, u_int v ) {
    vec.push_back( v );
    return vec;
};

class Updatable;
//! Array of Updatable
typedef  Vector<Updatable*> UpdatableVec;
//! Operator <<
inline UpdatableVec& operator<<(UpdatableVec& vec, Updatable* v ) {
    vec.push_back( v );
    return vec;
};

class Cluster;
//! Array of Clusters
typedef  Vector<Cluster*> ClusterVec;
//! Operator <<
inline ClusterVec& operator<<(ClusterVec& vec, Cluster* v ) {
    vec.push_back( v );
    return vec;
};

class Linker;
//! Array of Linkers
typedef  Vector<Linker*> LinkerVec;
//! Operator <<
inline LinkerVec& operator<<(LinkerVec& vec, Linker* v ) {
    vec.push_back( v );
    return vec;
};

//! Empty LinkerVec constant
 extern const LinkerVec emptyLinkerVec;

//! Empty ClusterVec constant
 extern const ClusterVec emptyClusterVec;

class BaseTeachBlock;
//! Array of Updatable
typedef  Vector<BaseTeachBlock*> TeachBlockVec;
//! Operator <<
inline TeachBlockVec& operator<<(TeachBlockVec& vec, BaseTeachBlock* v ) {
    vec.push_back( v );
    return vec;
};

}

#endif
