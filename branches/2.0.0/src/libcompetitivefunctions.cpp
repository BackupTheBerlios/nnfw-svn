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

#include "libcompetitivefunctions.h"
#include <cmath>

namespace nnfw {

WinnerTakeAllFunction::WinnerTakeAllFunction( double value )
	: OutputFunction() {
	valuev = value;
}

bool WinnerTakeAllFunction::setValue( double v ) {
	valuev = v;
	return true;
}

double WinnerTakeAllFunction::value() {
	return valuev;
}

void WinnerTakeAllFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	outputs.zeroing();
	outputs[ maxIndex( inputs ) ] = valuev;
}

void WinnerTakeAllFunction::configure(ConfigurationParameters& params, QString prefix)
{
	valuev = 1.0;
	QString str = params.getValue(prefix + "value");
	if (!str.isNull()) {
		bool ok;
		valuev = str.toDouble(&ok);
		if (!ok) {
			valuev = 1.0;
		}
	}
}

void WinnerTakeAllFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "WinnerTakeAllFunction", this);
	params.createParameter(prefix, "value", QString::number(valuev));
}

}

