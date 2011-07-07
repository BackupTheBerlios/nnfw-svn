/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2011 Gianluca Massera <emmegian@yahoo.it>                *
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

namespace nnfw {

CopyLinker::CopyLinker( Cluster* from, Cluster* to, CopyMode mode, QString name )
	: Linker(from, to, name), dataFrom(), dataTo() {
	dimData = qMin( to->numNeurons(), from->numNeurons() );
	// Follow initialization force setMode method to setting datas, otherwise strange automatic compile-time
	// initialization may results in unpredictable behaviour
	this->mode = (CopyMode)-1;
	setMode( mode );
}

CopyLinker::CopyLinker( ConfigurationParameters& params, QString prefix )
	: Linker( params, prefix ), dataFrom(), dataTo() {
	dimData = qMin( to()->numNeurons(), from()->numNeurons() );
	this->mode = (CopyMode)-1;
	//--- default initialisation, in the case the user forget to set the parameter
	setMode( Out2In );
	QString str = params.getValue(prefix + "mode");
	if ( !str.isNull() ) {
		str = str.toLower();
		if ( str == "in2in" ) {
			setMode( In2In );
		} else if ( str == "in2ou" ) {
			setMode( In2Out );
		} else if ( str == "out2in" ) {
			setMode( Out2In );
		} else if ( str == "out2out" ) {
			setMode( Out2Out );
		}
	}
}

CopyLinker::~CopyLinker() {
}

void CopyLinker::setMode( CopyMode cm ) {
	if ( this->mode == cm ) return;

	this->mode = cm;
	switch( mode ) {
	case In2In:
		dataFrom = from()->inputs();
		dataTo = to()->inputs();
		break;
	case In2Out:
		dataFrom = from()->inputs();
		dataTo = to()->outputs();
		break;
	case Out2In:
		dataFrom = from()->outputs();
		dataTo = to()->inputs();
		break;
	case Out2Out:
		dataFrom = from()->outputs();
		dataTo = to()->outputs();
		break;
	}
	return;
}

CopyLinker::CopyMode CopyLinker::getMode() const {
	return mode;
}

void CopyLinker::update() {
	// check if cluster 'To' needs a reset
	if ( to()->needReset() ) {
		to()->resetInputs();
	}
	for( unsigned int i=0; i<dimData; i++ ) {
		dataTo[i] += dataFrom[i];
	}
	return;
}

unsigned int CopyLinker::size() const {
    return dimData;
}

void CopyLinker::save(ConfigurationParameters& params, QString prefix) {
	Linker::save( params, prefix );
	params.startObjectParameters( prefix, "CopyLinker", this );
	switch( mode ) {
	case In2In:
		params.createParameter( prefix, "mode", "In2In" );
		break;
	case In2Out:
		params.createParameter( prefix, "mode", "In2Out" );
		break;
	case Out2In:
		params.createParameter( prefix, "mode", "Out2In" );
		break;
	case Out2Out:
		params.createParameter( prefix, "mode", "Out2Out" );
		break;
	}
}

}
