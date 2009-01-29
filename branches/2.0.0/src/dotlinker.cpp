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

#include "dotlinker.h"

namespace nnfw {

DotLinker::DotLinker( Cluster* from, Cluster* to, QString name )
	: MatrixLinker(from, to, name) {
}

DotLinker::~DotLinker() {
}

void DotLinker::update() {
	// check if cluster 'To' needs a reset
	if ( to()->needReset() ) {
		to()->resetInputs();
	}
	mul( to()->inputs(), from()->outputs(), matrix() );
	return;
}

DotLinker* DotLinker::clone() const {
	DotLinker* newclone = new DotLinker( this->from(), this->to(), name() );
	newclone->setMatrix( this->matrix() );
	return newclone;
}

}
