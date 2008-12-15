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

#include "ionnfw.h"
#include "outputfunction.h"
#include "cluster.h"
#include "linker.h"


namespace nnfw {

std::ostream& operator<<(std::ostream& stream, const RealVec& v) {
    for ( unsigned int i = 0; i < v.size(); i++ ) {
        stream << v[i] << "\t";
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const RealMat& m) {
    for( unsigned int i=0; i<m.rows(); i++ ) {
        stream << m[i] << std::endl;
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Variant::types t) {
    stream << ( Variant::typeName( t ) );
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Variant var) {
    bool b;
    Propertized* p;
    switch( var.type() ) {
    case Variant::t_null: stream << "Null"; break;
    case Variant::t_real: stream << var.getdouble(); break;
    case Variant::t_int: stream << var.getInt(); break;
    case Variant::t_uint: stream << var.getUInt(); break;
    case Variant::t_char: stream << var.getChar(); break;
    case Variant::t_uchar: stream << var.getUChar(); break;
    case Variant::t_bool:
        b = var.getBool();
        stream << (b ? "TRUE" : "FALSE" );
        break;
    case Variant::t_string: stream << var.getString(); break;
    case Variant::t_realvec: stream << *(var.getRealVec()); break;
    case Variant::t_realmat: stream << *(var.getRealMat()); break;
    case Variant::t_outfunction:
        p = (Propertized*)( var.getOutputFunction() );
        stream << *p;
        break;
    case Variant::t_cluster:
        p = (Propertized*)(var.getCluster());
        stream << *p;
        break;
    case Variant::t_linker:
        p = (Propertized*)(var.getLinker());
        stream << *p;
        break;
    case Variant::t_propertized: stream << *(var.getPropertized()); break;
	case Variant::t_dataptr: stream << "Generic data pointer"; break;
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Propertized& p) {
    static int indent = -1;
    PropertyAccessVec& ps = p.properties();
    indent++;
    for( unsigned int i=0; i<ps.size(); i++ ) {
        // --- indentation
        for( int k=0; k<indent; k++ ) {
            stream << "  ";
        }
        stream << ps[i]->name() << ": ";
        if ( ps[i]->isVector() ) {
            stream << "Printing of Vector Property not yet implemented" << std::endl;
        } else {
            Variant v = ps[i]->get();
            if ( v.type() == Variant::t_propertized ||
                v.type() == Variant::t_outfunction ||
                v.type() == Variant::t_cluster ||
                v.type() == Variant::t_linker ) {
                stream << std::endl;
            }
            stream << v;
        }
        if ( i < (ps.size()-1) ) {
            stream << std::endl;
        }
    }
    if ( indent == 0 ) {
        stream << std::endl;
    }
    indent--;
    return stream;    
}

}
