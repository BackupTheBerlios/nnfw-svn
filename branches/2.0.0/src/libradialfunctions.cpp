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

#include "libradialfunctions.h"

namespace nnfw {

GaussFunction::GaussFunction( double centre, double variance, double maxvalue )
    : DerivableOutputFunction() {
    this->centre = centre;
    this->variance = variance;
    msqrvar = -( variance*variance );
    this->max = maxvalue;
}

bool GaussFunction::setCentre( double v ) {
    centre = v;
    return true;
}

double GaussFunction::getCentre() {
    return centre;
}

bool GaussFunction::setVariance( double v ) {
    variance = v;
    msqrvar = -( variance*variance );
    return true;
}

double GaussFunction::getVariance() {
    return variance;
}

bool GaussFunction::setMax( double v ) {
    max = v;
    return true;
}

double GaussFunction::getMax() {
    return max;
}

void GaussFunction::apply( RealVec& inputs, RealVec& outputs ) {
    // --- out <- max * exp( (centre-inputs)^2 / -(variance^2) )
    outputs.assign_aminusx( centre, inputs );
    outputs.square();
    outputs /= msqrvar;
    outputs.exp();
    outputs *= max;
}

void GaussFunction::derivate( const RealVec& x, const RealVec& y, RealVec& d ) const {
    // --- d <- ( 2.0*(centre-x) / variance^2 ) * y
    d.assign_aminusx( centre, x );
    d *= 2.0;
    d /= -msqrvar;
    d *= y;
}

GaussFunction* GaussFunction::clone() const {
    return (new GaussFunction( centre, variance, max ) );
}

}

