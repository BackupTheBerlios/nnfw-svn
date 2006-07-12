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

#include "types.h"
#include "messages.h"

#include <cmath>

#ifdef NNFW_USE_MKL
#include <mkl_vml.h>
#include <mkl_cblas.h>
#endif

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

#ifdef NNFW_USE_MKL
int inutile = vmlSetMode( VML_LA );
#endif

RealVec::RealVec( u_int size )
    : viewers(0) {
    vsize = size;
    allocated = size;
	data = new Real[vsize];
	for(u_int i = 0; i<size; i++) {
	   data[i] = 0.0;
    }
    // --- view attribute
    view = false;
    viewed = 0;
    idstart = 0;
    idend = 0;
}

RealVec::RealVec( u_int size, Real value )
    : viewers(0) {
	vsize = size;
    allocated = size;
	data = new Real[vsize];
	for(u_int i = 0; i<size; i++) {
		data[i] = value;
    }
    // --- view attribute
    view = false;
    viewed = 0;
    idstart = 0;
    idend = 0;
}


RealVec::RealVec()
    : viewers(0) {
    data = 0;
    vsize = 0;
    allocated = 0;
    // --- view attribute
    view = false;
    viewed = 0;
    idstart = 0;
    idend = 0;
}

RealVec::RealVec( const Real* r, u_int dim )
    : viewers(0) {
    data = new Real[dim];
    vsize = dim;
    allocated = dim;
    memcpy( data, r, sizeof(Real)*dim );
    // --- view attribute
    view = false;
    viewed = 0;
    idstart = 0;
    idend = 0;
}

RealVec::RealVec( RealVec& src, u_int idStart, u_int idEnd )
    : viewers(0) {
    if ( idStart > src.vsize || idEnd > src.vsize || idStart >= idEnd ) {
        nnfwMessage( NNFW_ERROR, "Wrongs indexes specified in RealVec constructor; using 0 and src.size()" );
        idstart = 0;
        idend = src.size();
    } else {
        idstart = idStart;
        idend = idEnd;
    }
    data = (src.data) + idstart;
    vsize = idend - idstart;
    allocated = 0;
    view = true;
    src.viewers.push_back( this );
    viewed = &src;
}

RealVec::~RealVec() {
    // ---- QUESTO DELETE DA PROBLEMI ... PERCHE' ????
    //delete []data;
    if ( view ) {
        Vector<RealVec*>::iterator it = std::find( viewed->viewers.begin(), viewed->viewers.end(), this );
        if ( it != viewed->viewers.end() ) {
            viewed->viewers.erase( it );
        }
    }
}

void RealVec::setView( u_int idStart, u_int idEnd ) {
    if ( !view ) {
        nnfwMessage( NNFW_ERROR, "setView can be called only if RealVec is a view" );
        return;
    }
    if ( idStart > viewed->vsize || idEnd > viewed->vsize || idStart >= idEnd ) {
        nnfwMessage( NNFW_ERROR, "Wrongs indexes specified in RealVec constructor; using 0 and viewed->size()" );
        idstart = 0;
        idend = viewed->size();
    }
    idstart = idStart;
    idend = idEnd;
    data = (viewed->data) + idstart;
    vsize = idend - idstart;
    // --- Propagate Notify to sub-viewers
    for( u_int i=0; i<viewers.size(); i++ ) {
        viewers[i]->datachanged();
    }
}

void RealVec::resize( u_int newsize ) {
    if ( view ) {
        nnfwMessage( NNFW_ERROR, "It's not possible resize RealVec views" );
        return;
    }
    if ( allocated < newsize ) {
        allocated = newsize+20;
        Real* tmp = new Real[allocated];
        memcpy( tmp, data, sizeof(Real)*(vsize) );
        delete []data;
        data = tmp;
    }
    for( u_int i=vsize; i<newsize; i++ ) {
        data[i] = 0.0f;
    }
    vsize = newsize;
    // --- Notify the viewers
    for( u_int i=0; i<viewers.size(); i++ ) {
        viewers[i]->datachanged();
    }
}

void RealVec::append( const Real value ) {
    if ( view ) {
        nnfwMessage( NNFW_ERROR, "It's not possible append a value to RealVec views" );
        return;
    }
    if ( allocated < vsize+1 ) {
        allocated += 21;
        Real* tmp = new Real[allocated];
        memcpy( tmp, data, sizeof(Real)*(vsize) );
        delete []data;
        data = tmp;
    }
    data[vsize] = value;
    vsize += 1;
    // --- Notify the viewers
    for( u_int i=0; i<viewers.size(); i++ ) {
        viewers[i]->datachanged();
    }
}

void RealVec::datachanged() {
    if ( idstart > viewed->vsize || idend > viewed->vsize ) {
        nnfwMessage( NNFW_ERROR, "Indexes become invalid after data changing; using 0 and viewed->size()" );
        idstart = 0;
        idend = viewed->size();
    }
    data = (viewed->data) + idstart;
    vsize = idend - idstart;
    allocated = 0;
    // --- Propagate Notify to sub-viewers
    for( u_int i=0; i<viewers.size(); i++ ) {
        viewers[i]->datachanged();
    }
}

RealVec& RealVec::exp() {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    vsExp( vsize, data, data );
#else
    vdExp( vsize, data, data );
#endif
#else
    for( u_int i=0; i<vsize; i++ ) {
        data[i] = std::exp( data[i] );
    };
#endif
    return (*this);
}

RealVec& RealVec::inv() {
#ifdef NNFW_USE_MKL
#ifndef NNFW_DOUBLE_PRECISION
    vsInv( vsize, data, data );
#else
    vdInv( vsize, data, data );
#endif
#else
    for( u_int i=0; i<vsize; i++ ) {
        data[i] = 1.0/data[i];
    };
#endif
    return (*this);
}

RealVec::RealVec( const RealVec& ) {
/*    vsize = orig.vsize;
    allocated = orig.allocated;
    data = new Real[allocated];
    memcpy( data, orig.data, sizeof(Real)*vsize );*/
}

RealVec& RealVec::operator=( const RealVec& ) {
    // delete []data;  <-- DA PROBLEMI... Perche??? Non capisco !! :-(
/*    vsize = src.vsize;
    allocated = src.allocated;
    data = new Real[allocated];
    memcpy( data, src.data, sizeof(Real)*vsize );
*/
    return (*this);
}

void RealVec::createAllBinaries( RealVec* v, unsigned long int pats, u_int dims ) {
//#ifdef NNFW_DEBUG
	unsigned long int totPat = 1;
	for (u_int x = 0; x < dims; x++)
		totPat *= 2;
	if ( totPat != pats ) {
		nnfwMessage( NNFW_ERROR, "Error in createAllBinaries: total patterns are not how many you think they are" );
		return;
	}
//#endif	
	//Create the first string of all 0s
	u_int i;
	for (i = 0; i < dims; i++)
		v[0][i] = 0.;
	for (unsigned long int number = 1; number < pats; number++) {
		//String n = string n - 1
		for (i = 0; i < dims; i++)
			v[ number ][i] = v[ number - 1 ][i];
		//Add one to string n - 1
		i = 0;
		while ( v[ number ][i] == 1. ) {
			v[ number ][i] = 0.;
			i++;
		}
		v[ number ][i] = 1.;
	}	
};


Real RealVec::mse( const RealVec& target, const RealVec& actual ) {
#ifdef NNFW_DEBUG
	if ( target.size() != actual.size() ) {
		nnfwMessage( NNFW_ERROR, "Error in mse: target and actual vectors have different size" );
		return 0.0;
	}
#endif
	RealVec error( target.size() );
	error.assign(target);
	error -= actual;
	error.square();
	return error.mean();
};


std::ostream& operator<<(std::ostream& stream, const RealVec& v) {
	for ( u_int i = 0; i < v.size(); i++ ) {
		stream << v[i] << "\t";
	}
	stream << std::endl;
	return stream;
}


std::istream& operator>>(std::istream& stream, RealVec& v) {
	for ( u_int i = 0; i < v.size(); i++ ) {
		stream >> (v[i]);
	}
	return stream;
}


}

