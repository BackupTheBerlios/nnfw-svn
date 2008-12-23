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

#include "libperiodicfunctions.h"
#include <cmath>

namespace nnfw {

PeriodicFunction::PeriodicFunction( double phase, double span, double amplitude )
    : OutputFunction() {
	phasev = phase;
	spanv = span;
	amplitudev = amplitude;
}

bool PeriodicFunction::setPhase( double v ) {
	phasev = v;
	return true;
}

double PeriodicFunction::phase() {
	return phasev;
}

bool PeriodicFunction::setSpan( double v ) {
	spanv = v;
	return true;
}

double PeriodicFunction::span() {
	return spanv;
}

bool PeriodicFunction::setAmplitude( double v ) {
	amplitudev = v;
	return true;
}

double PeriodicFunction::amplitude() {
	return amplitudev;
}

SawtoothFunction::SawtoothFunction( double phase, double span, double amplitude )
    : PeriodicFunction(phase,span,amplitude) {
}

void SawtoothFunction::apply( RealVec& inputs, RealVec& outputs ) {
    // --- out <- 2.0*( (x-c)/a-floor((x-c)/a+0.5) )
	for( int i=0; i<(int)inputs.size(); i++ ) {
		outputs[i] = amplitudev*( (inputs[i]-phasev)/spanv-floor((inputs[i]-phasev)/spanv+0.5) );
	}
}

SawtoothFunction* SawtoothFunction::clone() const {
	return (new SawtoothFunction( phasev, spanv, amplitudev ) );
}

TriangleFunction::TriangleFunction( double phase, double span, double amplitude )
    : PeriodicFunction(phase,span,amplitude) {
}

void TriangleFunction::apply( RealVec& inputs, RealVec& outputs ) {
    // --- out <- 2.0*( (x-c)/a-floor((x-c)/a+0.5) )
	for( int i=0; i<(int)inputs.size(); i++ ) {
		double sawtooth = (inputs[i]-phasev)/spanv-floor((inputs[i]-phasev)/spanv+0.5);
		outputs[i] = amplitudev*( 1.0 - fabs( sawtooth ) );
	}
}

TriangleFunction* TriangleFunction::clone() const {
	return (new TriangleFunction( phasev, spanv, amplitudev ) );
}

#define PI_GRECO 3.14159265358979323846

SinFunction::SinFunction( double phase, double span, double amplitude )
    : PeriodicFunction(phase,span,amplitude) {
}

double SinFunction::frequency() {
	return 2.0*PI_GRECO/spanv;
}

void SinFunction::apply( RealVec& inputs, RealVec& outputs ) {
	for( int i=0; i<(int)inputs.size(); i++ ) {
		outputs[i] = amplitudev*sin(2.0*PI_GRECO*(inputs[i]/spanv)-PI_GRECO*phasev);
	}
}

SinFunction* SinFunction::clone() const {
	return (new SinFunction( phasev, spanv, amplitudev ) );
}

PseudoGaussFunction::PseudoGaussFunction( double phase, double span, double amplitude )
    : PeriodicFunction(phase,span,amplitude) {
}

void PseudoGaussFunction::apply( RealVec& inputs, RealVec& outputs ) {
	for( int i=0; i<(int)inputs.size(); i++ ) {
		outputs[i] = 0.5*amplitudev*( sin( 2.0*PI_GRECO*((inputs[i]-phasev)/spanv+0.25) ) + 1.0 );
	}
}

PseudoGaussFunction* PseudoGaussFunction::clone() const {
	return (new PseudoGaussFunction( phasev, spanv, amplitudev ) );
}

}

