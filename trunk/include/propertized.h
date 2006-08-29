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

#ifndef PROPERTIZED_H
#define PROPERTIZED_H

#include "types.h"
#include <map>
#include <string>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Incapsulate various type in a unified way (like union)
 */
class PolyType {
public:
    //! type of data registrable
    typedef enum { t_real=0, t_int, t_uint, t_char, t_uchar, t_bool,
                t_realvec, t_realmat, t_propertized } types;

    //! \brief Constructor
    PolyType( Real& d ) {
        dtype = t_real;
        data.dreal = d;
    };

    //! \brief Constructor
    PolyType( int& d ) {
        dtype = t_int;
        data.dint = d;
    };

    //! \brief Copy-Constructor
    PolyType( const PolyType& src ) {
        dtype = src.dtype;
        switch( dtype ) {
        case t_real: data.dreal = src.data.dreal; break;
        case t_int: data.dint = src.data.dint; break;
        case t_uint: data.duint = src.data.duint; break;
        case t_char: data.dchar = src.data.dchar; break;
        case t_uchar: data.duchar = src.data.duchar; break;
        case t_bool: data.dbool = src.data.dbool; break;
        case t_realvec: data.drealvec = src.data.drealvec; break;
        case t_realmat: data.drealmat = src.data.drealmat; break;
        case t_propertized: data.dprop = src.data.dprop; break;
        }
    };

    /*! \brief Type of data
     */
    types type() const {
        return dtype;
    };

    //! return the reference to Real
    Real& getReal() {
        checkType( t_real );
        return data.dreal;
    };

private:
    //! type registered
    types dtype;
    //! union
    union {
        Real&    dreal,
        int&     dint,
        u_int&   duint,
        char&    dchar,
        unsigned char&   duchar,
        bool&            dbool,
        RealVec&         drealvec,
        RealMat&         drealmat,
        Propertized&     dprop
    } data;

    //! Check type
    void checkType( types t ) {
        char* typen[] = { "Real", "int", "unsigned int", "char", "unsigned char", "bool", "RealVec", "RealMat", "Propertized" };
        if ( t != dtype ) {
            char msg[100];
            sprintf( msg, "Attempt to access a %s type instead of the right type %s", typen[t], typen[dtype] );
            nnfwMessage( NNFW_ERROR, msg );
        }
        return;
    };
};

/*! \brief Implements the capability to access internal data via properties
 *
 *  \par Motivation
 *  This class allow to access data of very different type of Cluster, Linker, etc by a unified interface
 *  via the concept of 'property'. A property is an attribute of a class identified by a string and accessibile
 *  by methods set/getProperty.
 *  \par Description
 *  \par Warnings
 */
class Propertized {
public:
    //! \brief Constructor
    Propertized();

    /*! \brief add a property
     *
     *  \par Warnings:
     *  this method doesn't check if a property with name name already exist, so pay attention or
     *  previous setting may be overwritten
     */
    void addProperty( const char* name, PolyType& data );

    /*! \brief return the property setted
     */
    PolyType& property( const char* name );

private:
    //! mapping name -> PolyType
    std::map< std::string, PolyType > props;
};

}

#endif
