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
 *
 */

// Definition a macro for exporting/importing data and types to/from .dll (it's only useful for windows)
#ifdef WIN32
    #ifdef DLL_CREATING
        #define NNFW_API __declspec(dllexport)
    #else
        #define NNFW_API __declspec(dllimport)
    #endif
#else
    #define NNFW_API
#endif

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
typedef NNFW_API unsigned int u_int;

//! Abstraction on the type of real numbers
typedef NNFW_API double Real;

//! Vector of Real
class NNFW_API RealVec : public Vector<Real> {
public:
    //! Constructor
    RealVec( u_int dim = 0 ) : Vector<Real>(dim, 0.0f) { /* Nothing to do */ };
    //! Construct by const Real* vector
    RealVec( const Real* r, u_int dim ) {
        for( u_int i=0; i<dim; i++ ) {
            this->push_back( r[i] );
        }
    };
    //! Construct a vector contains one element
    RealVec( const Real v ) {
        this->push_back( v );
    };
    // ****************************
    // *** UNARY OPERATORS
    //! Operator +
    const RealVec& operator+() const {
        return (*this);
    };
    //! Operator -
    const RealVec operator-() {
        u_int dim = this->size();
        for( u_int i=0; i<dim; i++ ) {
            (*this)[i] = -((*this)[i]);
        }
        return (*this);
    };
    // ****************************
    // *** BINARY OPERATORS
    //! Operator + with RealVec
    const RealVec operator+( const RealVec& r ) const {
        if( this->size() != r.size() ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return (*this);
        }
        u_int dim = this->size();
        RealVec rop( dim );
        for( u_int i=0; i<dim; i++ ) {
            rop[i] = (*this)[i] + r[i];
        }
        return rop;
    };
    //! Operator += with RealVec
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
    //! Operator + with Real
    const RealVec operator+( const Real& r ) const {
        u_int dim = this->size();
        RealVec rop( dim );
        for( u_int i=0; i<dim; i++ ) {
            rop[i] = (*this)[i] + r;
        }
        return rop;
    };
    //! Operator += with Real
    RealVec& operator+=(const Real& r ) {
        u_int dim = this->size();
        for( u_int i=0; i<dim; i++ ) {
            (*this)[i] += r;
        }
        return (*this);
    };
    //! Operator - with RealVec
    const RealVec operator-( const RealVec& r ) const {
        if( this->size() != r.size() ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return (*this);
        }
        u_int dim = this->size();
        RealVec rop( dim );
        for( u_int i=0; i<dim; i++ ) {
            rop[i] = (*this)[i] - r[i];
        }
        return rop;
    };
    //! Operator -= with RealVec
    RealVec& operator-=(const RealVec& r ) {
        if( this->size() != r.size() ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return *this;
        }
        u_int dim = this->size();
        for( u_int i=0; i<dim; i++ ) {
            (*this)[i] -= r[i];
        }
        return (*this);
    };
    //! Operator - with Real
    const RealVec operator-( const Real& r ) const {
        u_int dim = this->size();
        RealVec rop( dim );
        for( u_int i=0; i<dim; i++ ) {
            rop[i] = (*this)[i] - r;
        }
        return rop;
    };
    //! Operator -= with Real
    RealVec& operator-=(const Real& r ) {
        u_int dim = this->size();
        for( u_int i=0; i<dim; i++ ) {
            (*this)[i] -= r;
        }
        return (*this);
    };
    //! Operator - WARNING: this operator doesn't check the dimension
    const RealVec operator-(const Real* r) const {
        u_int dim = this->size();
        RealVec out( dim );
        for( u_int i=0; i<dim; i++ ) {
            out[i] = (*this)[i] - r[i];
        }
        return out;
    };
    //! Operator * with RealVec
    const RealVec operator*( const RealVec& r ) const {
        if( this->size() != r.size() ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return (*this);
        }
        u_int dim = this->size();
        RealVec rop( dim );
        for( u_int i=0; i<dim; i++ ) {
            rop[i] = (*this)[i] * r[i];
        }
        return rop;
    };
    //! Operator *= with RealVec
    RealVec& operator*=(const RealVec& r ) {
        if( this->size() != r.size() ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return *this;
        }
        u_int dim = this->size();
        for( u_int i=0; i<dim; i++ ) {
            (*this)[i] *= r[i];
        }
        return (*this);
    };
    //! Operator * with Real
    const RealVec operator*( const Real& r ) const {
        u_int dim = this->size();
        RealVec rop( dim );
        for( u_int i=0; i<dim; i++ ) {
            rop[i] = (*this)[i] * r;
        }
        return rop;
    };
    //! Operator *= with Real
    RealVec& operator*=(const Real& r ) {
        u_int dim = this->size();
        for( u_int i=0; i<dim; i++ ) {
            (*this)[i] *= r;
        }
        return (*this);
    };
    //! Operator / with RealVec
    const RealVec operator/( const RealVec& r ) const {
        if( this->size() != r.size() ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return (*this);
        }
        u_int dim = this->size();
        RealVec rop( dim );
        for( u_int i=0; i<dim; i++ ) {
            rop[i] = (*this)[i] / r[i];
        }
        return rop;
    };
    //! Operator /= with RealVec
    RealVec& operator/=(const RealVec& r ) {
        if( this->size() != r.size() ) {
            nnfwMessage( NNFW_ERROR, "Different numbers of element" );
            return *this;
        }
        u_int dim = this->size();
        for( u_int i=0; i<dim; i++ ) {
            (*this)[i] /= r[i];
        }
        return (*this);
    };
    //! Operator / with Real
    const RealVec operator/( const Real& r ) const {
        u_int dim = this->size();
        RealVec rop( dim );
        for( u_int i=0; i<dim; i++ ) {
            rop[i] = (*this)[i] / r;
        }
        return rop;
    };
    //! Operator /= with Real
    RealVec& operator/=(const Real& r ) {
        u_int dim = this->size();
        for( u_int i=0; i<dim; i++ ) {
            (*this)[i] /= r;
        }
        return (*this);
    };
};
//! Operator <<
inline RealVec& operator<<( RealVec& vec, const Real v ) {
    vec.push_back( v );
    return vec;
};
//! Operator +
inline RealVec operator+( const Real v, const RealVec& vec ) {
    return vec+v;
};
//! Operator -
inline RealVec operator-( const Real v, const RealVec& vec ) {
    return vec-v;
};
//! Operator *
inline RealVec operator*( const Real v, const RealVec& vec ) {
    return vec*v;
};
//! Operator /
inline RealVec operator/( const Real v, const RealVec& vec ) {
    u_int dim = vec.size();
    RealVec rop( dim );
    for( u_int i; i<dim; i++ ) {
        rop[i] = v/vec[i];    
    }
    return rop;
};

//! Array of Unsigned Integer
typedef NNFW_API Vector<u_int> U_IntVec;
//! Operator <<
inline U_IntVec& operator<<(U_IntVec& vec, u_int v ) {
    vec.push_back( v );
    return vec;
};

class Updatable;
//! Array of Updatable
typedef NNFW_API Vector<Updatable*> UpdatableVec;
//! Operator <<
inline UpdatableVec& operator<<(UpdatableVec& vec, Updatable* v ) {
    vec.push_back( v );
    return vec;
};

class Cluster;
//! Array of Clusters
typedef NNFW_API Vector<Cluster*> ClusterVec;
//! Operator <<
inline ClusterVec& operator<<(ClusterVec& vec, Cluster* v ) {
    vec.push_back( v );
    return vec;
};

class Linker;
//! Array of Linkers
typedef NNFW_API Vector<Linker*> LinkerVec;
//! Operator <<
inline LinkerVec& operator<<(LinkerVec& vec, Linker* v ) {
    vec.push_back( v );
    return vec;
};

//! Empty LinkerVec constant
NNFW_API extern const LinkerVec emptyLinkerVec;

//! Empty ClusterVec constant
NNFW_API extern const ClusterVec emptyClusterVec;

class BaseTeachBlock;
//! Array of Updatable
typedef NNFW_API Vector<BaseTeachBlock*> TeachBlockVec;
//! Operator <<
inline TeachBlockVec& operator<<(TeachBlockVec& vec, BaseTeachBlock* v ) {
    vec.push_back( v );
    return vec;
};

}

#endif
