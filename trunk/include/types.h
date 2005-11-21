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
 *
 *  \todo operator << to Vector template that inserts element as push_back() method;
 *  \todo operators: + - / % += -= /= %= to RealVec class
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
typedef unsigned int u_int;

//! Abstraction on the type of real numbers
typedef float Real;

//! Vector of Real
class RealVec : public Vector<Real> {
public:
    //! Constructor
    RealVec( u_int dim = 0 ) : Vector<Real>(dim) { /* Nothing to do */ };
    //! Operator +
    const RealVec operator+(const RealVec& r) {
        if( this->size() != r.size() ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return *this;
        }
        u_int dim = this->size();
        RealVec sum( dim );
        for( u_int i=0; i<dim; i++ ) {
            sum[i] = (*this)[i] + r[i];
        }
        return sum;
    };
    //! Operator +=
    RealVec& operator+=(const RealVec& r ) {
        if( this->size() != r.size() ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return *this;
        }
        u_int dim = this->size();
        for( u_int i=0; i<dim; i++ ) {
            (*this)[i] += r[i];
        }
        return (*this);
    };
    //! Operator - WARNING: this operator doesn't check the dimension
    const RealVec operator-(const Real* r) {
        u_int dim = this->size();
        RealVec out( dim );
        for( u_int i=0; i<dim; i++ ) {
            out[i] = (*this)[i] - r[i];
        }
        return out;
    };
};
//! Operator <<
inline RealVec& operator<<( RealVec& vec, const Real v ) {
    vec.push_back( v );
    return vec;
};
//typedef Vector<Real> RealVec;

class Updatable;
//! Array of Updatable
typedef Vector<Updatable*> UpdatableVec;
//! Operator <<
inline UpdatableVec& operator<<(UpdatableVec& vec, Updatable* v ) {
    vec.push_back( v );
    return vec;
};

class Cluster;
//! Array of Clusters
typedef Vector<Cluster*> ClusterVec;

class Linker;
//! Array of Linkers
typedef Vector<Linker*> LinkerVec;

//! Empty LinkerVec constant
extern LinkerVec emptyLinkerVec;

//! Empty ClusterVec constant
extern ClusterVec emptyClusterVec;

}

#endif
