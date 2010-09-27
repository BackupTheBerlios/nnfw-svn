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

#include "libradialfunctions.h"

namespace nnfw {

GaussFunction::GaussFunction( double centre, double variance, double maxvalue )
	: OutputFunction() {
	this->centre = centre;
	this->variancev = variance;
	msqrvar = -( variancev*variancev );
	this->max = maxvalue;
}

bool GaussFunction::setVariance( double v ) {
	variancev = v;
	msqrvar = -( variancev*variancev );
	return true;
}

double GaussFunction::variance() {
	return variancev;
}

void GaussFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	// --- out <- max * exp( (centre-inputs)^2 / -(variance^2) )
	square( minus( outputs, centre, inputs ) );
	exp( outputs /= msqrvar ) *= max;
}

bool GaussFunction::derivate( const DoubleVector& x, const DoubleVector& y, DoubleVector& d ) const {
    // --- d <- ( 2.0*(centre-x) / variance^2 ) * y
	minus( d, centre, x ) *= 2.0;
	d /= -msqrvar;
	d *= y;
	return true;
}

void GaussFunction::configure(ConfigurationParameters& params, QString prefix)
{
	centre = 0.0;
	QString str = params.getValue(prefix + "centre").
	if (!str.isNull()) {
		bool ok;
		centre = str.toDouble(&ok);
		if (!ok) {
			centre = 0.0;
		}
	}

	variancev = 1.0;
	QString str = params.getValue(prefix + "variance").
	if (!str.isNull()) {
		bool ok;
		variancev = str.toDouble(&ok);
		if (!ok) {
			variancev = 1.0;
		}
	}
	// Also recomputing minus squared variance
	msqrvar = -( variancev*variancev );

	max = 1.0;
	QString str = params.getValue(prefix + "max").
	if (!str.isNull()) {
		bool ok;
		max = str.toDouble(&ok);
		if (!ok) {
			max = 1.0;
		}
	}
}

void GaussFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "GaussFunction", this);

	params.createParameter(prefix, "centre", QString::number(centre));

	params.createParameter(prefix, "variance", QString::number(variancev));

	params.createParameter(prefix, "max", QString::number(max));
}

}
