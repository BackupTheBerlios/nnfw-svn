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

#include "propertized.h"


namespace nnfw {

const char* Variant::typen[t_propertized+1] = { "Null", "Real", "int", "unsigned int", "char", "unsigned char", "bool",
    "String (const char*)", "RealVec*", "RealMat*", "OutputFunction*", "Cluster*", "Linker*", "Propertized*" };

Variant::Variant() {
    dtype = t_null;
}

Variant::Variant( const Variant& src ) {
    dtype = src.dtype;
    switch( dtype ) {
    case t_null: break;
    case t_real: dreal = src.dreal; break;
    case t_int: dint = src.dint; break;
    case t_uint: duint = src.duint; break;
    case t_char: dchar = src.dchar; break;
    case t_uchar: duchar = src.duchar; break;
    case t_bool: dbool = src.dbool; break;
    case t_string: dstring = src.dstring; break;
    case t_realvec: drealvec = src.drealvec; break;
    case t_realmat: drealmat = src.drealmat; break;
    case t_outfunction: doutfun = src.doutfun; break;
    case t_cluster: dcluster = src.dcluster; break;
    case t_linker: dlinker = src.dlinker; break;
    case t_propertized: dprop = src.dprop; break;
    }
}

Variant::Variant( Real d ) {
    dtype = t_real;
    dreal = d;
}

Variant::Variant( int d ) {
    dtype = t_int;
    dint = d;
}

Variant::Variant( u_int d ) {
    dtype = t_uint;
    duint = d;
}

Variant::Variant( char d ) {
    dtype = t_char;
    dchar = d;
}

Variant::Variant( unsigned char d ) {
    dtype = t_uchar;
    duchar = d;
}

Variant::Variant( bool d ) {
    dtype = t_bool;
    dbool = d;
}

Variant::Variant( const char* d ) {
    dtype = t_string;
    u_int size = strlen(d);
    dstring = new char[size+1];
    strcpy( dstring, d );
}

Variant::Variant( RealVec* d ) {
    dtype = t_realvec;
    drealvec = d;
}

Variant::Variant( RealMat* d ) {
    dtype = t_realmat;
    drealmat = d;
}

Variant::Variant( OutputFunction* d ) {
    dtype = t_outfunction;
    doutfun = d;
}

Variant::Variant( Cluster* d ) {
    dtype = t_cluster;
    dcluster = d;
}

Variant::Variant( Linker* d ) {
    dtype = t_linker;
    dlinker = d;
}

Variant::Variant( Propertized* d ) {
    dtype = t_propertized;
    dprop = d;
}

Variant& Variant::operator=( const Variant& src ) {
    dtype = src.dtype;
    switch( dtype ) {
    case t_null: break;
    case t_real: dreal = src.dreal; break;
    case t_int: dint = src.dint; break;
    case t_uint: duint = src.duint; break;
    case t_char: dchar = src.dchar; break;
    case t_uchar: duchar = src.duchar; break;
    case t_bool: dbool = src.dbool; break;
    case t_string: dstring = src.dstring; break;
    case t_realvec: drealvec = src.drealvec; break;
    case t_realmat: drealmat = src.drealmat; break;
    case t_outfunction: doutfun = src.doutfun; break;
    case t_cluster: dcluster = src.dcluster; break;
    case t_linker: dlinker = src.dlinker; break;
    case t_propertized: dprop = src.dprop; break;
    }
    return (*this);
}

Variant::types Variant::type() const {
    return dtype;
}

const char* Variant::typeName() const {
    return typen[dtype];
}

bool Variant::isNull() {
    return ( dtype == t_null );
}

Real Variant::getReal() const {
    checkType( t_real );
    return dreal;
}

int Variant::getInt() const {
    checkType( t_int );
    return dint;
}

u_int Variant::getUInt() const {
    checkType( t_uint );
    return duint;
}

char Variant::getChar() const {
    checkType( t_char );
    return dchar;
}

unsigned char Variant::getUChar() const {
    checkType( t_uchar );
    return duchar;
}

bool Variant::getBool() const {
    checkType( t_bool );
    return dbool;
}

const char* Variant::getString() const {
    checkType( t_string );
    return dstring;
}

RealVec* Variant::getRealVec() const {
    checkType( t_realvec );
    return drealvec;
}

RealMat* Variant::getRealMat() const {
    checkType( t_realmat );
    return drealmat;
}

OutputFunction* Variant::getOutputFunction() const {
    checkType( t_outfunction );
    return doutfun;
}

Cluster* Variant::getCluster() const {
    checkType( t_cluster );
    return dcluster;
}

Linker* Variant::getLinker() const {
    checkType( t_linker );
    return dlinker;
}

Propertized* Variant::getPropertized() const {
    checkType( t_propertized );
    return dprop;
}

const char* Variant::typeName( types t ) {
    return typen[t];
}

void Variant::checkType( types t ) const {
    if ( t != dtype ) {
        nError() << "Attempt to access a " << typen[t] << " type instead of the right type " << typen[dtype]
				 << " unpredictable result";
    }
    return;
}


Propertized::Propertized()
    : props(), vecProps(0) {
    vtypename = 0;
    setTypename( "Propertized" );
    addProperty( "typename", Variant::t_string, this, &Propertized::getTypename );
}

Propertized::~Propertized() {
    props.clear();
    for( u_int i=0; i<vecProps.size(); i++ ) {
        delete (vecProps[i]);
    }
    vecProps.clear();
    delete []vtypename;
}

void Propertized::setProperties( PropertySettings& prop ) {
    PropertySettings::iterator it = prop.begin();
    while( it != prop.end() ) {
        setProperty( (*it).first.data(), (*it).second );
        it++;
    }
}

void Propertized::setTypename( const char* type ) {
    if (vtypename) {
        delete []vtypename;
    }
    u_int size = strlen(type);
    vtypename = new char[size+1];
    strcpy( vtypename, type );
}

}
