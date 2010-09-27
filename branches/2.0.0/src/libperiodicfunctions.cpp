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

#include "libperiodicfunctions.h"
#include <cmath>

namespace nnfw {

PeriodicFunction::PeriodicFunction( double phase, double span, double amplitude )
	: OutputFunction() {
	this->phase = phase;
	this->span = span;
	this->amplitude = amplitude;
}

void PeriodicFunction::configure(ConfigurationParameters& params, QString prefix)
{
	phase = 0.0;
	QString str = params.getValue(prefix + "phase").
	if (!str.isNull()) {
		bool ok;
		phase = str.toDouble(&ok);
		if (!ok) {
			phase = 0.0;
		}
	}

	span = 1.0;
	str = params.getValue(prefix + "span").
	if (!str.isNull()) {
		bool ok;
		span = str.toDouble(&ok);
		if (!ok) {
			span = 1.0;
		}
	}

	amplitude = 1.0;
	str = params.getValue(prefix + "amplitude").
	if (!str.isNull()) {
		bool ok;
		amplitude = str.toDouble(&ok);
		if (!ok) {
			amplitude = 1.0;
		}
	}
}

void PeriodicFunction::save(ConfigurationParameters& params, QString prefix)
{
	// Here we call startObjectParameters even if this is an abstract class
	// because it creates the group
	params.startObjectParameters(prefix, "PeriodicFunction", this);

	params.createParameter(prefix, "phase", QString::number(phase));

	params.createParameter(prefix, "span", QString::number(span));

	params.createParameter(prefix, "amplitude", QString::number(amplitude));
}

SawtoothFunction::SawtoothFunction( double phase, double span, double amplitude )
	: PeriodicFunction(phase,span,amplitude) {
}

void SawtoothFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	// --- out <- 2.0*( (x-c)/a-floor((x-c)/a+0.5) )
	for( int i=0; i<(int)inputs.size(); i++ ) {
		outputs[i] = amplitude*( (inputs[i]-phase)/span-floor((inputs[i]-phase)/span+0.5) );
	}
}

void SawtoothFunction::configure(ConfigurationParameters& params, QString prefix)
{
	// Calling parent function
	PeriodicFunction::configure(params, prefix)
}

void SawtoothFunction::save(ConfigurationParameters& params, QString prefix)
{
	// Calling parent function
	PeriodicFunction::save(params, prefix);

	// Now saving our parameters
	params.startObjectParameters(prefix, "SawtoothFunction", this);
}

TriangleFunction::TriangleFunction( double phase, double span, double amplitude )
	: PeriodicFunction(phase,span,amplitude) {
}

void TriangleFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	// --- out <- 2.0*( (x-c)/a-floor((x-c)/a+0.5) )
	for( unsigned int i=0; i<inputs.size(); i++ ) {
		double sawtooth = (inputs[i]-phase)/span-floor((inputs[i]-phase)/span+0.5);
		outputs[i] = amplitude*( 1.0 - fabs( sawtooth ) );
	}
}

void TriangleFunction::configure(ConfigurationParameters& params, QString prefix)
{
	// Calling parent function
	PeriodicFunction::configure(params, prefix)
}

void TriangleFunction::save(ConfigurationParameters& params, QString prefix)
{
	// Calling parent function
	PeriodicFunction::save(params, prefix);

	// Now saving our parameters
	params.startObjectParameters(prefix, "TriangleFunction", this);
}

SinFunction::SinFunction( double phase, double span, double amplitude )
	: PeriodicFunction(phase,span,amplitude) {
}

double SinFunction::frequency() {
	return 2.0*PI_GRECO/span;
}

void SinFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	for( int i=0; i<(int)inputs.size(); i++ ) {
		outputs[i] = amplitude*sin(2.0*PI_GRECO*(inputs[i]/span)-PI_GRECO*phase);
	}
}

void SinFunction::configure(ConfigurationParameters& params, QString prefix)
{
	// Calling parent function
	PeriodicFunction::configure(params, prefix)
}

void SinFunction::save(ConfigurationParameters& params, QString prefix)
{
	// Now calling parent function
	PeriodicFunction::save(params, prefix);

	// Now saving our parameters
	params.startObjectParameters(prefix, "SinFunction", this);
}

PseudoGaussFunction::PseudoGaussFunction( double phase, double span, double amplitude )
	: PeriodicFunction(phase,span,amplitude) {
}

void PseudoGaussFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	for( unsigned int i=0; i<inputs.size(); i++ ) {
		outputs[i] = 0.5*amplitude*( sin( 2.0*PI_GRECO*((inputs[i]-phase)/span+0.25) ) + 1.0 );
	}
}

void PseudoGaussFunction::configure(ConfigurationParameters& params, QString prefix)
{
	// Calling parent function
	PeriodicFunction::configure(params, prefix)
}

void PseudoGaussFunction::save(ConfigurationParameters& params, QString prefix)
{
	// Now calling parent function
	PeriodicFunction::save(params, prefix);

	// Now saving our parameters
	params.startObjectParameters(prefix, "PseudoGaussFunction", this);
}

}

