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

#include "copylinker.h"
#include "random.h"
#include <cstdio>


namespace nnfw {

/**********************************************
 *  Implementation of CopyLinker Class        *
 **********************************************/

CopyLinker::CopyLinker( Cluster* from, Cluster* to, CopyMode mode, const char* name )
    : Linker(from, to, name), dataFrom(), dataTo() {
    if ( from->numNeurons() < to->numNeurons() ) {
        dimData = from->numNeurons();
    } else {
        dimData = to->numNeurons();
    }
    // Follow initialization force setMode method to setting datas, otherwise strange automatic compile-time
    // initialization may results in unpredictable behaviour
    this->mode = (CopyMode)-1;
    setMode( mode );

    addProperty( "mode", Variant::t_string, this, &CopyLinker::getModeP, &CopyLinker::setMode );
    setTypename( "CopyLinker" );
}

CopyLinker::CopyLinker( PropertySettings& prop )
    : Linker( prop ) {
    Variant& v = prop["mode"];
    if ( from()->numNeurons() < to()->numNeurons() ) {
        dimData = from()->numNeurons();
    } else {
        dimData = to()->numNeurons();
    }
    this->mode = (CopyMode)-1;
    if ( v.isNull() ) {
        setMode( Out2In );
    } else {
        setMode( v );
    };
    addProperty( "mode", Variant::t_string, this, &CopyLinker::getModeP, &CopyLinker::setMode );
    setTypename( "CopyLinker" );
}

CopyLinker::~CopyLinker() {
}

void CopyLinker::setMode( CopyMode cm ) {
    if ( this->mode == cm ) return;

    this->mode = cm;
    switch( mode ) {
    case In2In:
        dataFrom.convertToView( from()->inputs(), 0, dimData );
        dataTo.convertToView( to()->inputs(), 0, dimData );
        break;
    case In2Out:
        dataFrom.convertToView( from()->inputs(), 0, dimData );
        dataTo.convertToView( to()->outputs(), 0, dimData );
        break;
    case Out2In:
        dataFrom.convertToView( from()->outputs(), 0, dimData );
        dataTo.convertToView( to()->inputs(), 0, dimData );
        break;
    case Out2Out:
        dataFrom.convertToView( from()->outputs(), 0, dimData );
        dataTo.convertToView( to()->outputs(), 0, dimData );
        break;
    }
    return;
}

bool CopyLinker::setMode( const Variant& v ) {
	nnfwString str = v.getString();
	if ( str == "In2In" ) {
		setMode( In2In );
	} else if ( str == "In2Out" ) {
		setMode( In2Out );
	} else if ( str == "Out2In" ) {
		setMode( Out2In );
	} else if ( str == "Out2Out" ) {
		setMode( Out2Out );
	} else {
		nWarning() << "mode accept exactly only one of 'In2In', 'In2Out', 'Out2In', 'Out2Out'";
		setMode( Out2In );
	}
    return true;
}

CopyLinker::CopyMode CopyLinker::getMode() const {
    return mode;
}

Variant CopyLinker::getModeP() {
    switch( mode ) {
    case In2In:
		return Variant( "In2In" );
        break;
    case In2Out:
		return Variant( "In2Out" );
        break;
    case Out2In:
		return Variant( "Out2In" );
        break;
    case Out2Out:
		return Variant( "Out2Out" );
        break;
    }
    return Variant( "In2In" );
}

void CopyLinker::update() {
    // check if cluster 'To' needs a reset
    if ( to()->needReset() ) {
        dataTo.assign( dataFrom );
    } else {
        dataTo += dataFrom;
    }
    return;
}

u_int CopyLinker::size() const {
    return dimData;
}

CopyLinker* CopyLinker::clone() const {
	CopyLinker* newclone = new CopyLinker( this->from(), this->to(), this->getMode(), name() );
	return newclone;
}

}
