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

#ifdef NNFW_USE_MKL
#include <mkl_vml.h>
#include <mkl_cblas.h>
#endif

#ifdef NNFW_DEBUG
#include "messages.h"
#endif

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

void DummyUpdater::update( RealVec& inputs, RealVec& outputs ) {
    outputs.assign( inputs );
}

void DummyUpdater::update( Real input, Real &output ) {
    output = input;
}

Real DummyUpdater::derivate( Real, Real ) const {
    return 1.0;
}

void SigmoidUpdater::update( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nnfwMessage( NNFW_ERROR, "The output dimension doesn't match the input dimension" );
        return;
    }
#endif
    outputs.assign_amulx( -lambda, inputs );
    outputs.exp();
    outputs += 1.0;
    outputs.inv();
}

void SigmoidUpdater::update( Real input, Real &output ) {
    output = 1.0/( 1.0 + exp( -lambda*( input ) ) );
}

Real SigmoidUpdater::derivate( Real , Real out ) const {
    return lambda * out * (1.0-out);
}

void FakeSigmoidUpdater::update( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nnfwMessage( NNFW_ERROR, "The output dimension doesn't match the input dimension" );
        return;
    }
#endif
    u_int size = inputs.size();
    Real x;
    Real x0 = 6. + 2./3.;
    Real zero = 0.5;
    for ( u_int i = 0; i<size; i++ ) {
        x = inputs[i];
        x *= lambda;
        x -= (.5 - zero) / (.075 + zero);
        if ( x <= -x0 ) {
            outputs[i] = 0.0;
        } else {
            if ( x < x0 ) {
                outputs[i] = .5 + .575 * x / ( 1.0 + fabs(x) );
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

Real FakeSigmoidUpdater::derivate( Real , Real out ) const {
    return lambda * out * (1.0-out);
}

void ScaledSigmoidUpdater::update( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nnfwMessage( NNFW_ERROR, "The output dimension doesn't match the input dimension" );
        return;
    }
#endif
    u_int size = inputs.size();
    outputs.assign_amulx( -lambda, inputs );
    outputs.exp();
    for ( u_int i = 0; i<size; i++ ) {
        outputs[i] = (max - min ) * (1.0/( 1.0 + outputs[i] )) + min;
    }
}

void ScaledSigmoidUpdater::update( Real input, Real &output ) {
    Real f;
    f = 1.0/( 1.0 + exp( -lambda*( input ) ) );
    output = ( max - min ) * f + min ;
}

Real ScaledSigmoidUpdater::derivate( Real , Real out ) const {
    return lambda * out * (1.0-out);
}

void LinearUpdater::update( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nnfwMessage( NNFW_ERROR, "The output dimension doesn't match the input dimension" );
        return;
    }
#endif
    u_int size = inputs.size();
    for ( u_int i = 0; i<size; i++ ) {
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

Real LinearUpdater::derivate( Real x, Real ) const {
    if ( x >= minX && x<= maxX ) {
        return ( maxY-minY )/( maxX-minX );
    } else {
		Real y;
		y = 1.0/( 1.0 + exp( -x ) );
		return y * ( 1.0 - y );
    }
}

void BinaryUpdater::update( RealVec& inputs, RealVec& outputs ) {
    u_int size = inputs.size();
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nnfwMessage( NNFW_ERROR, "The output dimension doesn't match the input dimension" );
        return;
    }
#endif
    for ( u_int i = 0; i<size; i++ ) {
        ( inputs[i] > threshold ) ? outputs[i] = 1.0f : outputs[i] = 0.0f;
    }
}

void BinaryUpdater::update( Real input, Real &output ) {
    ( input > threshold ) ? output = 1.0f : output = 0.0f;
}

Real BinaryUpdater::derivate( Real x, Real ) const {
	Real y;
	y = 1.0/( 1.0 + exp( -x ) );
    return y * ( 1.0 - y );
}

}
