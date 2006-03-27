/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2006 Gianluca Massera <emmegian@yahoo.it>                     *
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

#include "libclusterupdaters.h"

#include <cmath>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

void DummyUpdater::update( Real* inputs, Real* outputs, u_int numNeuron ) {
    for ( u_int i = 0; i<numNeuron; i++ ) {
        outputs[i] = inputs[i];
    }
}

void DummyUpdater::update( Real input, Real &output ) {
    output = input;
}

Real DummyUpdater::derivate( Real ) const {
    return 1.0;
}

void SigmoidUpdater::update( Real* inputs, Real* outputs, u_int numNeuron ) {
    for ( u_int i = 0; i<numNeuron; i++ ) {
        outputs[i] = 1.0/( 1.0 + exp( -lambda*( inputs[i] ) ) );
    }
}

void SigmoidUpdater::update( Real input, Real &output ) {
    output = 1.0/( 1.0 + exp( -lambda*( input ) ) );
}

Real SigmoidUpdater::derivate( Real x ) const {
    return x*(1.0-x);
}

void FakeSigmoidUpdater::update( Real* inputs, Real* outputs, u_int numNeuron ) {
    Real x;
    Real x0 = 6. + 2./3.;
    Real zero = 0.5;
    for ( u_int i = 0; i<numNeuron; i++ ) {
        x = inputs[i];
        x *= lambda;
        x -= (.5 - zero) / (.075 + zero);
        if ( x <= -x0 ) {
            outputs[i] = 0.0;
        } else {
            if ( x < x0 ) {
                outputs[i] = .5 + .575 * x / ( 1 + fabs(x) );
            } else {
                outputs[i] = 1.0;
            }
        }
    }
}

void FakeSigmoidUpdater::update( Real x, Real &output ) {
    Real x0 = 6. + 2./3.;
    Real zero = 0.5;
    x *= lambda;
    x -= (.5 - zero) / (.075 + zero);
    if ( x <= -x0 ) {
        output = 0.0;
    } else {
        if ( x < x0 ) {
            output = .5 + .575 * x / ( 1 + fabs(x) );
        } else {
            output = 1.0;
        }
    }
}

Real FakeSigmoidUpdater::derivate( Real x ) const {
    return x*(1.0-x);
}

void ScaledSigmoidUpdater::update( Real* inputs, Real* outputs, u_int numNeuron ) {
    Real f;
    for ( u_int i = 0; i<numNeuron; i++ ) {
        f = 1.0/( 1.0 + exp( -lambda*( inputs[i] ) ) );
        outputs[i] = ( max - min ) * f + min ;
    }
}

void ScaledSigmoidUpdater::update( Real input, Real &output ) {
    Real f;
    f = 1.0/( 1.0 + exp( -lambda*( input ) ) );
    output = ( max - min ) * f + min ;
}

Real ScaledSigmoidUpdater::derivate( Real x ) const {
    return x*(1.0-x);
}

void LinearUpdater::update( Real* inputs, Real* outputs, u_int numNeuron ) {
    for ( u_int i = 0; i<numNeuron; i++ ) {
        Real m = ( maxY-minY )/( maxX-minX );
        Real q = minY - m*minX;
        Real ret = m*(inputs[i]) + q;
        if (ret < minY) {
            outputs[i] = minY;
        } else if (ret > maxY) {
            outputs[i] = maxY;
        } else {
            outputs[i] = ret;
        }
    }
}

void LinearUpdater::update( Real input, Real &output ) {
    Real m = ( maxY-minY )/( maxX-minX );
    Real q = minY - m*minX;
    Real ret = m*(input) + q;
    if (ret < minY) {
        output = minY;
    } else if (ret > maxY) {
        output = maxY;
    } else {
        output = ret;
    }
}

Real LinearUpdater::derivate( Real x ) const {
    if ( x >= minX && x<= maxX ) {
        return ( maxY-minY )/( maxX-minX );
    } else {
        return x*(1.0-x);
    }
}

void BinaryUpdater::update( Real* inputs, Real* outputs, u_int numNeuron ) {
    for ( u_int i = 0; i<numNeuron; i++ ) {
        ( inputs[i] > threshold ) ? outputs[i] = 1.0f : outputs[i] = 0.0f;
    }
}

void BinaryUpdater::update( Real input, Real &output ) {
    ( input > threshold ) ? output = 1.0f : output = 0.0f;
}

Real BinaryUpdater::derivate( Real x ) const {
    return x*(1.0-x);
}

}
