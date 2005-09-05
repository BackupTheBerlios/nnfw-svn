/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005 Gianluca Massera <emmegian@yahoo.it>                     *
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

void DummyUpdater::update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron ) {
    for ( u_int i = 0; i<numNeuron; i++ ) {
        outputs[i] = inputs[i];
    }
}

const char* DummyUpdater::className() const {
    return "DummyUpdater";
}

void SigmoidUpdater::update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron ) {
    for ( u_int i = 0; i<numNeuron; i++ ) {
        outputs[i] = 1.0/( 1.0 + exp( -lambda*( inputs[i] ) ) );
    }
}

const char* SigmoidUpdater::className() const {
    return "SigmoidUpdater";
}

void FakeSigmoidUpdater::update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron ) {
    nnfwReal x;
    nnfwReal x0 = 6. + 2./3.;
    nnfwReal zero = 0.5;
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

const char* FakeSigmoidUpdater::className() const {
    return "FakeSigmoidUpdater";
}

void ScaledSigmoidUpdater::update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron ) {
    nnfwReal f;
    for ( u_int i = 0; i<numNeuron; i++ ) {
        f = 1.0/( 1.0 + exp( -lambda*( inputs[i] ) ) );
        outputs[i] = ( max - min ) * f + min ;
    }
}

const char* ScaledSigmoidUpdater::className() const {
    return "ScaledSigmoidUpdater";
}

void LinearUpdater::update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron ) {
    for ( u_int i = 0; i<numNeuron; i++ ) {
        double m = ( maxY-minY )/( maxX-minX );
        double q = minY - m*minX;
        double ret = m*(inputs[i]) + q;
        if (ret < minY) {
            outputs[i] = minY;
        } else if (ret > maxY) {
            outputs[i] = maxY;
        } else {
            outputs[i] = ret;
        }
    }
}

const char* LinearUpdater::className() const {
    return "LinearUpdater";
}
