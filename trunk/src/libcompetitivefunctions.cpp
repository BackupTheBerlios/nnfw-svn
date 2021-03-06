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

#include "libcompetitivefunctions.h"
#include <cmath>

namespace nnfw {

WinnerTakeAllFunction::WinnerTakeAllFunction( Real value )
    : OutputFunction() {
	valuev = value;
	
	addProperty( "value", Variant::t_real, this, &WinnerTakeAllFunction::value, &WinnerTakeAllFunction::setValue );
	setTypename( "WinnerTakeAllFunction" );
}

WinnerTakeAllFunction::WinnerTakeAllFunction( PropertySettings& prop )
    : OutputFunction() {
	valuev = 1.0;
	addProperty( "value", Variant::t_real, this, &WinnerTakeAllFunction::value, &WinnerTakeAllFunction::setValue );
	setProperties( prop );
	setTypename( "WinnerTakeAllFunction" );
}

bool WinnerTakeAllFunction::setValue( const Variant& v ) {
	valuev = v.getReal();
	return true;
}

Variant WinnerTakeAllFunction::value() {
	return valuev;
}

void WinnerTakeAllFunction::apply( RealVec& inputs, RealVec& outputs ) {
	outputs.zeroing();
	outputs[ inputs.maxIndex() ] = valuev;
}

WinnerTakeAllFunction* WinnerTakeAllFunction::clone() const {
	return (new WinnerTakeAllFunction( valuev ) );
}

}

