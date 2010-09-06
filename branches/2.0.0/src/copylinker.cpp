/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>                *
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

CopyLinker* CopyLinker::clone() const {
	CopyLinker* newclone = new CopyLinker( this->from(), this->to(), this->getMode(), name() );
	return newclone;
}

}
