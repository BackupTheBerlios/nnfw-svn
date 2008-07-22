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

#include "outputfunctionmodel.h"
#include "modelmaps.h"
using namespace nnfw;

OutputFunctionModel::OutputFunctionModel( nnfw::OutputFunction* f )
	: PropertizedModel( f ) {
	ModelMaps* maps = ModelMaps::instance();
	if ( maps->modelOf( f ) ) {
		//--- errore molto grave
		qFatal( "Is not allowed to create more that one model for the same OutputFunction" );
	}
	maps->addEntry( this, f );
	function = f;

}

OutputFunctionModel::~OutputFunctionModel() {
	ModelMaps::instance()->delEntry( function );
	delete function;
}

nnfw::OutputFunction* OutputFunctionModel::nnFunction() {
	return function;
}

void OutputFunctionModel::apply( nnfw::RealVec& inputs, nnfw::RealVec& outputs ) {
	function->apply( inputs, outputs );
}

nnfw::Real OutputFunctionModel::apply( nnfw::Real in ) {
	return function->apply( in );
}
