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

#include "ionnfw.h"
#include "messages.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

std::ostream& operator<<(std::ostream& stream, const RealVec& v) {
    for ( u_int i = 0; i < v.size(); i++ ) {
        stream << v[i] << "\t";
    }
    return stream;
}

std::istream& operator>>(std::istream& stream, RealVec& v) {
    for ( u_int i = 0; i < v.size(); i++ ) {
        stream >> (v[i]);
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const RealMat& m) {
    for( u_int i=0; i<m.rows(); i++ ) {
        stream << m[i] << std::endl;
    }
    return stream;
}

std::istream& operator>>(std::istream& stream, RealMat& ) {
    nnfwMessage( NNFW_ERROR, "Reading Variant not yet implemented" );
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Variant::types t) {
    stream << ( Variant::typeName( t ) );
    return stream;
}

std::istream& operator>>(std::istream& stream, Variant::types& t) {
    stream >> t;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Variant var) {
    switch( var.type() ) {
    case Variant::t_null: stream << "Null"; break;
    case Variant::t_real: stream << var.getReal(); break;
    case Variant::t_int: stream << var.getInt(); break;
    case Variant::t_uint: stream << var.getUInt(); break;
    case Variant::t_char: stream << var.getChar(); break;
    case Variant::t_uchar: stream << var.getUChar(); break;
    case Variant::t_bool: stream << var.getBool(); break;
    case Variant::t_realvec: stream << *(var.getRealVec()); break;
    case Variant::t_realmat: stream << *(var.getRealMat()); break;
    case Variant::t_propertized: stream << *(var.getPropertized()); break;
    }
    return stream;
}

std::istream& operator>>(std::istream& stream, Variant& ) {
    nnfwMessage( NNFW_ERROR, "Reading Variant not yet implemented" );
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Propertized& p) {
    PropertyAccessVec& ps = p.properties();
    for( u_int i=0; i<ps.size(); i++ ) {
        stream << "Name: " << ps[i]->name();
        stream << "\tType: " << ps[i]->type();
        stream << "\tValue: " << ps[i]->get();
        stream << std::endl;
    }
    return stream;    
}

std::istream& operator>>(std::istream& stream, Propertized& ) {
    nnfwMessage( NNFW_ERROR, "Reading Variant not yet implemented" );
    return stream;
}

}
