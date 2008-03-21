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

PeriodicFunction::PeriodicFunction( Real phase, Real span, Real amplitude )
    : OutputFunction() {
	phasev = phase;
	spanv = span;
	amplitudev = amplitude;
	
	addProperty( "phase", Variant::t_real, this, &SawtoothFunction::phase, &SawtoothFunction::setPhase );
	addProperty( "span", Variant::t_real, this, &SawtoothFunction::span, &SawtoothFunction::setSpan );
	addProperty( "amplitude", Variant::t_real, this, &SawtoothFunction::amplitude, &SawtoothFunction::setAmplitude );
	//setTypename( "PeriodicFunction" ); // is abstract
}

PeriodicFunction::PeriodicFunction( PropertySettings& prop )
    : OutputFunction() {
	phasev = 0.0;
	spanv = 1.0;
	amplitudev = 1.0;
	addProperty( "phase", Variant::t_real, this, &SawtoothFunction::phase, &SawtoothFunction::setPhase );
	addProperty( "span", Variant::t_real, this, &SawtoothFunction::span, &SawtoothFunction::setSpan );
	addProperty( "amplitude", Variant::t_real, this, &SawtoothFunction::amplitude, &SawtoothFunction::setAmplitude );
	setProperties( prop );
	//setTypename( "PeriodicFunction" ); // is abstract
}

bool PeriodicFunction::setPhase( const Variant& v ) {
	phasev = v.getReal();
	return true;
}

Variant PeriodicFunction::phase() {
	return phasev;
}

bool PeriodicFunction::setSpan( const Variant& v ) {
	spanv = v.getReal();
	return true;
}

Variant PeriodicFunction::span() {
	return spanv;
}

bool PeriodicFunction::setAmplitude( const Variant& v ) {
	amplitudev = v.getReal();
	return true;
}

Variant PeriodicFunction::amplitude() {
	return amplitudev;
}

SawtoothFunction::SawtoothFunction( Real phase, Real span, Real amplitude )
    : PeriodicFunction(phase,span,amplitude) {
	setTypename( "SawtoothFunction" );
}

SawtoothFunction::SawtoothFunction( PropertySettings& prop )
    : PeriodicFunction(prop) {
	setTypename( "SawtoothFunction" );
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

TriangleFunction::TriangleFunction( Real phase, Real span, Real amplitude )
    : PeriodicFunction(phase,span,amplitude) {
	setTypename( "TriangleFunction" );
}

TriangleFunction::TriangleFunction( PropertySettings& prop )
    : PeriodicFunction(prop) {
	setTypename( "TriangleFunction" );
}

void TriangleFunction::apply( RealVec& inputs, RealVec& outputs ) {
    // --- out <- 2.0*( (x-c)/a-floor((x-c)/a+0.5) )
	for( int i=0; i<(int)inputs.size(); i++ ) {
		Real sawtooth = (inputs[i]-phasev)/spanv-floor((inputs[i]-phasev)/spanv+0.5);
		outputs[i] = amplitudev*( 1.0 - fabs( sawtooth ) );
	}
}

TriangleFunction* TriangleFunction::clone() const {
	return (new TriangleFunction( phasev, spanv, amplitudev ) );
}

#define PI_GRECO 3.14159265358979323846

SinFunction::SinFunction( Real phase, Real span, Real amplitude )
    : PeriodicFunction(phase,span,amplitude) {
	setTypename( "SinFunction" );
}

SinFunction::SinFunction( PropertySettings& prop )
    : PeriodicFunction(prop) {
	setTypename( "SinFunction" );
}

Real SinFunction::frequency() {
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

PseudoGaussFunction::PseudoGaussFunction( Real phase, Real span, Real amplitude )
    : PeriodicFunction(phase,span,amplitude) {
	setTypename( "PseudoGaussFunction" );
}

PseudoGaussFunction::PseudoGaussFunction( PropertySettings& prop )
    : PeriodicFunction(prop) {
	setTypename( "PseudoGaussFunction" );
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

