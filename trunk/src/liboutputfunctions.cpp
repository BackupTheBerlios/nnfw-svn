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

#include "liboutputfunctions.h"

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

void IdentityFunction::apply( RealVec& inputs, RealVec& outputs ) {
    outputs.assign( inputs );
}

void IdentityFunction::derivate( const RealVec&, const RealVec&, RealVec& derivates ) const {
    derivates.assign( derivates.size(), 1.0f );
}

IdentityFunction* IdentityFunction::clone() const {
    return new IdentityFunction();
}

void SigmoidFunction::apply( RealVec& inputs, RealVec& outputs ) {
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

bool SigmoidFunction::setLambda( const Variant& v ) {
    lambda = v.getReal();
    return true;
}

Variant SigmoidFunction::getLambda() {
    return Variant( lambda );
}

void SigmoidFunction::derivate( const RealVec&, const RealVec& outputs, RealVec& derivates ) const {
    // derivates <- lambda * out * (1.0-out)
    derivates.assign_aminusx( 1.0, outputs );
    derivates *= outputs;
    derivates *= lambda;
}

SigmoidFunction* SigmoidFunction::clone() const {
    return new SigmoidFunction( lambda );
}

void FakeSigmoidFunction::apply( RealVec& inputs, RealVec& outputs ) {
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

bool FakeSigmoidFunction::setLambda( const Variant& v ) {
    lambda = v.getReal();
    return true;
}

Variant FakeSigmoidFunction::getLambda() {
    return Variant( lambda );
}

void FakeSigmoidFunction::derivate( const RealVec&, const RealVec& outputs, RealVec& derivates ) const {
    // derivates <- lambda * out * (1.0-out)
    derivates.assign_aminusx( 1.0, outputs );
    derivates *= outputs;
    derivates *= lambda;
}

FakeSigmoidFunction* FakeSigmoidFunction::clone() const {
    return new FakeSigmoidFunction( lambda );
}

void ScaledSigmoidFunction::apply( RealVec& inputs, RealVec& outputs ) {
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

bool ScaledSigmoidFunction::setLambda( const Variant& v ) {
    lambda = v.getReal();
    return true;
}

Variant ScaledSigmoidFunction::getLambda() {
    return Variant( lambda );
}

bool ScaledSigmoidFunction::setMin( const Variant& v ) {
    min = v.getReal();
    return true;
}

Variant ScaledSigmoidFunction::getMin() {
    return Variant( min );
}

bool ScaledSigmoidFunction::setMax( const Variant& v ) {
    max = v.getReal();
    return true;
}

Variant ScaledSigmoidFunction::getMax() {
    return Variant( max );
}

void ScaledSigmoidFunction::derivate( const RealVec&, const RealVec& outputs, RealVec& derivates ) const {
    // derivates <- lambda * out * (1.0-out)
    derivates.assign_aminusx( 1.0, outputs );
    derivates *= outputs;
    derivates *= lambda;
}

ScaledSigmoidFunction* ScaledSigmoidFunction::clone() const {
    return new ScaledSigmoidFunction( lambda, min, max );
}

void LinearFunction::apply( RealVec& inputs, RealVec& outputs ) {
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

bool LinearFunction::setMinX( const Variant& v ) {
    minX = v.getReal();
    return true;
}

Variant LinearFunction::getMinX() {
    return Variant( minX );
}

bool LinearFunction::setMaxX( const Variant& v ) {
    maxX = v.getReal();
    return true;
}

Variant LinearFunction::getMaxX() {
    return Variant( maxX );
}

bool LinearFunction::setMinY( const Variant& v ) {
    minY = v.getReal();
    return true;
}

Variant LinearFunction::getMinY() {
    return Variant( minY );
}

bool LinearFunction::setMaxY( const Variant& v ) {
    maxY = v.getReal();
    return true;
}

Variant LinearFunction::getMaxY() {
    return Variant( maxY );
}

void LinearFunction::derivate( const RealVec& inputs, const RealVec&, RealVec& derivates ) const {
    for( u_int i=0; i<inputs.size(); i++ ) {
        if ( inputs[i] >= minX && inputs[i] <= maxX ) {
            derivates[i] = ( maxY-minY )/( maxX-minX );
        } else {
            Real y;
            y = 1.0/( 1.0 + exp( -inputs[i] ) );
            derivates[i] = y * ( 1.0 - y );
        }
    }
}

LinearFunction* LinearFunction::clone() const {
    return new LinearFunction( minX, maxX, minY, maxY );
}

void StepFunction::apply( RealVec& inputs, RealVec& outputs ) {
    u_int size = inputs.size();
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nnfwMessage( NNFW_ERROR, "The output dimension doesn't match the input dimension" );
        return;
    }
#endif
    for ( u_int i = 0; i<size; i++ ) {
        ( inputs[i] > threshold ) ? outputs[i] = max : outputs[i] = min;
    }
}

bool StepFunction::setMin( const Variant& v ) {
    min = v.getReal();
    return true;
}

Variant StepFunction::getMin() {
    return Variant( min );
}

bool StepFunction::setMax( const Variant& v ) {
    max = v.getReal();
    return true;
}

Variant StepFunction::getMax() {
    return Variant( max );
}

bool StepFunction::setThreshold( const Variant& v ) {
    threshold = v.getReal();
    return true;
}

Variant StepFunction::getThreshold() {
    return Variant( threshold );
}

void StepFunction::derivate( const RealVec& inputs, const RealVec&, RealVec& derivates ) const {
    for( u_int i=0; i<inputs.size(); i++ ) {
        Real y;
        y = 1.0/( 1.0 + exp( -inputs[i] ) );
        derivates[i] = y * ( 1.0 - y );
    }
}

StepFunction* StepFunction::clone() const {
    return new StepFunction( min, max, threshold );
}


PoolFunction::PoolFunction( const OutputFunction& prototype, u_int dim )
    : OutputFunction(), ups(dim) {
    for( u_int i=0; i<dim; i++ ) {
        ups[i] = prototype.clone();
    }
    // --- if dimension is zero, set at least one element to OutputFunction
    if ( dim == 0 ) {
        nnfwMessage( NNFW_ERROR, "The dimension of PoolFunction must be at least one" );
        ups.resize( 1 );
        ups[0] = new OutputFunction();
    }
    setTypename( "PoolFunction" );
}

PoolFunction::PoolFunction( u_int dim )
    : OutputFunction(), ups(dim) {
    // --- if dimension is zero, set at least one element to OutputFunction
    if ( dim == 0 ) {
        nnfwMessage( NNFW_ERROR, "The dimension of PoolFunction must be at least one" );
        ups.resize( 1 );
    }
    setTypename( "PoolFunction" );
}

PoolFunction::~PoolFunction() {
    for( u_int i=0; i<ups.size(); i++ ) {
        delete (ups[i]);
    }
}

Variant PoolFunction::getOuputFunctions() {
    return Variant( &ups );
}

void PoolFunction::setOutputFunction( u_int i, const OutputFunction& prototype ) {
    if ( i >= ups.size() ) {
        nnfwMessage( NNFW_ERROR, "Setting a OutputFunction beyond boundary of this PoolFunction" );
        return;
    }
    delete (ups[i]);
    ups[i] = prototype.clone();
    return;
}

void PoolFunction::apply( RealVec& inputs, RealVec& outputs ) {
    u_int dim = ups.size();
    for( u_int i=0; i<dim; i++ ) {
        outputs[i] = ups[i]->apply( inputs[i] );
    }
}

PoolFunction* PoolFunction::clone() const {
    PoolFunction* pool = new PoolFunction( ups.size() );
    for( u_int i=0; i<ups.size(); i++ ) {
        pool->ups[i] = this->ups[i]->clone();
    }
    return pool;
}

}
