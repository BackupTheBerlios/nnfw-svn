/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
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
#include "cluster.h"

#include <cmath>

#ifdef NNFW_USE_MKL
#include <mkl_vml.h>
#include <mkl_cblas.h>
#endif


namespace nnfw {

IdentityFunction::IdentityFunction()
    : DerivableOutputFunction() {
    setTypename( "IdentityFunction" );
}

IdentityFunction::IdentityFunction( PropertySettings& )
    : DerivableOutputFunction() {
    // --- non ha proprieta'
    setTypename( "IdentityFunction" );
}

void IdentityFunction::apply( RealVec& inputs, RealVec& outputs ) {
    outputs.assign( inputs );
}

void IdentityFunction::derivate( const RealVec&, const RealVec&, RealVec& derivates ) const {
    derivates.assign( derivates.size(), 1.0f );
}

IdentityFunction* IdentityFunction::clone() const {
    return new IdentityFunction();
}

ScaleFunction::ScaleFunction( Real rate )
    : OutputFunction() {
	this->rate = rate;
    addProperty( "rate", Variant::t_real, this, &ScaleFunction::getRate, &ScaleFunction::setRate );
    setTypename( "ScaleFunction" );
}

ScaleFunction::ScaleFunction( PropertySettings& prop )
    : OutputFunction() {
	rate = 1.0;
    addProperty( "rate", Variant::t_real, this, &ScaleFunction::getRate, &ScaleFunction::setRate );
    setProperties( prop );
    setTypename( "ScaleFunction" );
}

bool ScaleFunction::setRate( const Variant& v ) {
    rate = v.getReal();
    return true;
}

Variant ScaleFunction::getRate() {
    return Variant( rate );
}

void ScaleFunction::apply( RealVec& inputs, RealVec& outputs ) {
    outputs.assign_amulx( rate, inputs );
}

ScaleFunction* ScaleFunction::clone() const {
    return new ScaleFunction( rate );
}

SigmoidFunction::SigmoidFunction( Real l ) : DerivableOutputFunction() {
    lambda = l;
    addProperty( "lambda", Variant::t_real, this, &SigmoidFunction::getLambda, &SigmoidFunction::setLambda );
    setTypename( "SigmoidFunction" );
}

SigmoidFunction::SigmoidFunction( PropertySettings& prop )
    : DerivableOutputFunction() {
    lambda = 1.0;
    addProperty( "lambda", Variant::t_real, this, &SigmoidFunction::getLambda, &SigmoidFunction::setLambda );
    setProperties( prop );
    setTypename( "SigmoidFunction" );
}

void SigmoidFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nError() << "The output dimension doesn't match the input dimension" ;
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

FakeSigmoidFunction::FakeSigmoidFunction( Real l )
    : DerivableOutputFunction() {
    lambda = l;
    addProperty( "lambda", Variant::t_real, this, &FakeSigmoidFunction::getLambda, &FakeSigmoidFunction::setLambda );
    setTypename( "FakeSigmoidFunction" );
}

FakeSigmoidFunction::FakeSigmoidFunction( PropertySettings& prop )
    : DerivableOutputFunction() {
    lambda = 1.0;
    addProperty( "lambda", Variant::t_real, this, &FakeSigmoidFunction::getLambda, &FakeSigmoidFunction::setLambda );
    setProperties( prop );
    setTypename( "FakeSigmoidFunction" );
}

void FakeSigmoidFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nError() << "The output dimension doesn't match the input dimension" ;
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

ScaledSigmoidFunction::ScaledSigmoidFunction( Real l, Real min, Real max )
    : DerivableOutputFunction() {
    lambda = l;
    this->min = min;
    this->max = max;
    addProperty( "lambda", Variant::t_real, this, &ScaledSigmoidFunction::getLambda, &ScaledSigmoidFunction::setLambda );
    addProperty( "min", Variant::t_real, this, &ScaledSigmoidFunction::getMin, &ScaledSigmoidFunction::setMin );
    addProperty( "max", Variant::t_real, this, &ScaledSigmoidFunction::getMax, &ScaledSigmoidFunction::setMax );
    setTypename( "ScaledSigmoidFunction" );
}

ScaledSigmoidFunction::ScaledSigmoidFunction( PropertySettings& prop )
    : DerivableOutputFunction() {
    lambda = 1.0;
    min = -1.0;
    max = +1.0;
    addProperty( "lambda", Variant::t_real, this, &ScaledSigmoidFunction::getLambda, &ScaledSigmoidFunction::setLambda );
    addProperty( "min", Variant::t_real, this, &ScaledSigmoidFunction::getMin, &ScaledSigmoidFunction::setMin );
    addProperty( "max", Variant::t_real, this, &ScaledSigmoidFunction::getMax, &ScaledSigmoidFunction::setMax );
    setProperties( prop );
    setTypename( "FakeSigmoidFunction" );
}

void ScaledSigmoidFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nError() << "The output dimension doesn't match the input dimension" ;
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

LinearFunction::LinearFunction( Real minX, Real maxX, Real minY, Real maxY )
    : DerivableOutputFunction() {
    this->minX = minX;
    this->maxX = maxX;
    this->minY = minY;
    this->maxY = maxY;
    addProperty( "minX", Variant::t_real, this, &LinearFunction::getMinX, &LinearFunction::setMinX );
    addProperty( "maxX", Variant::t_real, this, &LinearFunction::getMaxX, &LinearFunction::setMaxX );
    addProperty( "minY", Variant::t_real, this, &LinearFunction::getMinY, &LinearFunction::setMinY );
    addProperty( "maxY", Variant::t_real, this, &LinearFunction::getMaxY, &LinearFunction::setMaxY );
    setTypename( "LinearFunction" );
}

LinearFunction::LinearFunction( PropertySettings& prop )
    : DerivableOutputFunction() {
    minX = -1.0;
    maxX = +1.0;
    minY = -1.0;
    maxY = +1.0;
    addProperty( "minX", Variant::t_real, this, &LinearFunction::getMinX, &LinearFunction::setMinX );
    addProperty( "maxX", Variant::t_real, this, &LinearFunction::getMaxX, &LinearFunction::setMaxX );
    addProperty( "minY", Variant::t_real, this, &LinearFunction::getMinY, &LinearFunction::setMinY );
    addProperty( "maxY", Variant::t_real, this, &LinearFunction::getMaxY, &LinearFunction::setMaxY );
    setProperties( prop );
    setTypename( "LinearFunction" );
}

void LinearFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nError() << "The output dimension doesn't match the input dimension" ;
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

StepFunction::StepFunction( Real min, Real max, Real threshold )
    : DerivableOutputFunction() {
    this->min = min;
    this->max = max;
    this->threshold = threshold;
    addProperty( "min", Variant::t_real, this, &StepFunction::getMin, &StepFunction::setMin );
    addProperty( "max", Variant::t_real, this, &StepFunction::getMax, &StepFunction::setMax );
    addProperty( "threshold", Variant::t_real, this, &StepFunction::getThreshold, &StepFunction::setThreshold );
    setTypename( "StepFunction" );
}

StepFunction::StepFunction( PropertySettings& prop )
    : DerivableOutputFunction() {
    min = 0.0;
    max = +1.0;
    threshold = 0.0;
    addProperty( "min", Variant::t_real, this, &StepFunction::getMin, &StepFunction::setMin );
    addProperty( "max", Variant::t_real, this, &StepFunction::getMax, &StepFunction::setMax );
    addProperty( "threshold", Variant::t_real, this, &StepFunction::getThreshold, &StepFunction::setThreshold );
    setProperties( prop );
    setTypename( "StepFunction" );
}

void StepFunction::apply( RealVec& inputs, RealVec& outputs ) {
    u_int size = inputs.size();
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nError() << "The output dimension doesn't match the input dimension" ;
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

LeakyIntegratorFunction::LeakyIntegratorFunction( const RealVec& d )
    : OutputFunction(), delta(), outprev() {
    delta.resize( d.size() );
	delta.assign( d );
	outprev.resize( delta.size() );
	outprev.zeroing();
    addProperty( "delta", Variant::t_realvec, this, &LeakyIntegratorFunction::getDeltaV, &LeakyIntegratorFunction::setDeltaV );
    setTypename( "LeakyIntegratorFunction" );
}

LeakyIntegratorFunction::LeakyIntegratorFunction( PropertySettings& prop )
    : OutputFunction(), delta(1), outprev(1) {
	delta[0] = 0.5;
	outprev[0] = 0.0f;
    addProperty( "delta", Variant::t_realvec, this, &LeakyIntegratorFunction::getDeltaV, &LeakyIntegratorFunction::setDeltaV );
    setProperties( prop );
    setTypename( "LeakyIntegratorFunction" );
}

void LeakyIntegratorFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nError() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
	//--- y <- delta*y(t-1) + (1.0-delta)*inputs
	//---  its equivalent to
	//--- y <- delta*( y(t-1) - inputs ) + inputs
	outputs.assign_xminusy( outprev, inputs );
	outputs *= delta;
	outputs += inputs;
	outprev.assign( outputs );
}

Variant LeakyIntegratorFunction::getDeltaV() {
	return Variant( &delta );
}

bool LeakyIntegratorFunction::setDeltaV( const Variant& v ) {
	setDelta( *(v.getRealVec()) );
	return true;
}

void LeakyIntegratorFunction::zeroingStatus() {
	outprev.zeroing();
}

LeakyIntegratorFunction* LeakyIntegratorFunction::clone() const {
	LeakyIntegratorFunction* cl = new LeakyIntegratorFunction( delta );
	cl->outprev.assign( outprev );
    return cl;
}

void LeakyIntegratorFunction::setCluster( Cluster* c ) {
	if ( c->numNeurons() != delta.size() ) {
		delta.resize( c->numNeurons() );
		outprev.resize( c->numNeurons() );
	}
}

PoolFunction::PoolFunction( const OutputFunction& prototype, u_int dim )
    : OutputFunction(), ups(dim) {
    for( u_int i=0; i<dim; i++ ) {
        ups[i] = prototype.clone();
    }
    // --- if dimension is zero, set at least one element to OutputFunction
    if ( dim == 0 ) {
#ifdef NNFW_DEBUG
        nWarning() << "The dimension of PoolFunction must be at least one";
#endif
        ups.resize( 1 );
        ups[0] = prototype.clone();
    }
    addProperty( "size", Variant::t_uint, this, &PoolFunction::sizeV );
    addVectorProperty( "functions", Variant::t_outfunction, this, &PoolFunction::getOutputFunctionV, &PoolFunction::setOutputFunction );
    setTypename( "PoolFunction" );
}

PoolFunction::PoolFunction( u_int dim )
    : OutputFunction(), ups(dim) {
    // --- if dimension is zero, set at least one element to OutputFunction
    if ( dim == 0 ) {
#ifdef NNFW_DEBUG
        nWarning() << "The dimension of PoolFunction must be at least one" ;
#endif
        ups.resize( 1 );
    }
    addProperty( "size", Variant::t_uint, this, &PoolFunction::sizeV );
    addVectorProperty( "functions", Variant::t_outfunction, this, &PoolFunction::getOutputFunctionV, &PoolFunction::setOutputFunction );
    setTypename( "PoolFunction" );
}

PoolFunction::PoolFunction( PropertySettings& prop )
    : OutputFunction(), ups() {
    Variant& v = prop["size"];
    int dim = 1;
    if ( ! v.isNull() ) {
        dim = v.getUInt();
    }
    for( int i=0; i<dim; i++ ) {
        ups.push_back( new OutputFunction() );
    }
    addProperty( "size", Variant::t_uint, this, &PoolFunction::sizeV );
    addVectorProperty( "functions", Variant::t_outfunction, this, &PoolFunction::getOutputFunctionV, &PoolFunction::setOutputFunction );
    setProperties( prop );
    setTypename( "StepFunction" );
}

PoolFunction::~PoolFunction() {
    for( u_int i=0; i<ups.size(); i++ ) {
        delete (ups[i]);
    }
}

OutputFunction* PoolFunction::getOutputFunction( u_int i ) {
#ifdef NNFW_DEBUG
    if ( i >= ups.size() ) {
        nError() << "Accessing beyond boundary of this PoolFunction" ;
        return 0;
    }
#endif
    return ups[i];
}

void PoolFunction::setOutputFunction( u_int i, const OutputFunction& prototype ) {
#ifdef NNFW_DEBUG
    if ( i >= ups.size() ) {
        nError() << "Setting a OutputFunction beyond boundary of this PoolFunction";
        return;
    }
#endif
    delete (ups[i]);
    ups[i] = prototype.clone();
    return;
}

Variant PoolFunction::getOutputFunctionV( u_int i ) {
    if ( i < ups.size() ) {
        return Variant( ups[i] );
    } else {
        return Variant();
    }
}

bool PoolFunction::setOutputFunction( u_int i, const Variant& v ) {
    if ( i >= ups.size() ) {
        return false;
    }
    setOutputFunction( i, *(v.getOutputFunction()) );
    return true;
}

unsigned int PoolFunction::size() {
    return ups.size();
}

Variant PoolFunction::sizeV() {
    return Variant( ups.size() );
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

void PoolFunction::setCluster( Cluster* c ) {
    u_int oldDim = ups.size();
    u_int newDim = c->numNeurons();
    ups.resize( newDim );
    for( u_int i=oldDim; i<newDim; i++ ) {
        ups[i] = new OutputFunction();
    }
}

CompositeFunction::CompositeFunction( const OutputFunction& f, const OutputFunction& g )
    : OutputFunction(), mid() {
	first = f.clone();
	second = g.clone();

    addProperty( "first", Variant::t_outfunction, this, &CompositeFunction::getFirstFunction, &CompositeFunction::setFirstFunction );
    addProperty( "second", Variant::t_outfunction, this, &CompositeFunction::getSecondFunction, &CompositeFunction::setSecondFunction );
    setTypename( "CompositeFunction" );
}

CompositeFunction::CompositeFunction( PropertySettings& prop )
    : OutputFunction(), mid() {
	first = new IdentityFunction();
	second = new IdentityFunction();
    addProperty( "first", Variant::t_outfunction, this, &CompositeFunction::getFirstFunction, &CompositeFunction::setFirstFunction );
    addProperty( "second", Variant::t_outfunction, this, &CompositeFunction::getSecondFunction, &CompositeFunction::setSecondFunction );
    setProperties( prop );
    setTypename( "CompositeFunction" );
}

CompositeFunction::~CompositeFunction() {
	delete first;
	delete second;
}

void CompositeFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nError() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
	first->apply( inputs, mid );
	second->apply( mid, outputs );
}

bool CompositeFunction::setFirstFunction( const Variant& v ) {
	delete first;
    first = v.getOutputFunction()->clone();
	first->setCluster( cl );
    return true;
}

Variant CompositeFunction::getFirstFunction() {
    return Variant( first );
}

bool CompositeFunction::setSecondFunction( const Variant& v ) {
	delete second;
    second = v.getOutputFunction()->clone();
	second->setCluster( cl );
    return true;
}

Variant CompositeFunction::getSecondFunction() {
    return Variant( second );
}

CompositeFunction* CompositeFunction::clone() const {
	return new CompositeFunction( *first, *second );
}

void CompositeFunction::setCluster( Cluster* c ) {
	this->cl = c;
	mid.resize( c->numNeurons() );
	first->setCluster( c );
	second->setCluster( c );
}

LinearComboFunction::LinearComboFunction( Real w1, const OutputFunction& f, Real w2, const OutputFunction& g )
    : OutputFunction(), mid() {
	first = f.clone();
	second = g.clone();
	this->w1 = w1;
	this->w2 = w2;

    addProperty( "first", Variant::t_outfunction, this, &LinearComboFunction::getFirstFunction, &LinearComboFunction::setFirstFunction );
    addProperty( "second", Variant::t_outfunction, this, &LinearComboFunction::getSecondFunction, &LinearComboFunction::setSecondFunction );
    addProperty( "w1", Variant::t_real, this, &LinearComboFunction::getFirstWeight, &LinearComboFunction::setFirstWeight );
    addProperty( "w2", Variant::t_real, this, &LinearComboFunction::getSecondWeight, &LinearComboFunction::setSecondWeight );
    setTypename( "LinearComboFunction" );
}

LinearComboFunction::LinearComboFunction( PropertySettings& prop )
    : OutputFunction(), mid() {
	first = new IdentityFunction();
	second = new IdentityFunction();
	w1 = w2 = 1.0;
    addProperty( "first", Variant::t_outfunction, this, &LinearComboFunction::getFirstFunction, &LinearComboFunction::setFirstFunction );
    addProperty( "second", Variant::t_outfunction, this, &LinearComboFunction::getSecondFunction, &LinearComboFunction::setSecondFunction );
    addProperty( "w1", Variant::t_real, this, &LinearComboFunction::getFirstWeight, &LinearComboFunction::setFirstWeight );
    addProperty( "w2", Variant::t_real, this, &LinearComboFunction::getSecondWeight, &LinearComboFunction::setSecondWeight );
    setProperties( prop );
    setTypename( "LinearComboFunction" );
}

LinearComboFunction::~LinearComboFunction() {
	delete first;
	delete second;
}

void LinearComboFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        nError() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
	mid.assign( outputs );
	first->apply( inputs, mid );
	mid.scale( w1 );
	second->apply( inputs, outputs );
	outputs.scale( w2 );
	outputs += mid;
}

bool LinearComboFunction::setFirstFunction( const Variant& v ) {
	delete first;
    first = v.getOutputFunction()->clone();
	first->setCluster( cl );
    return true;
}

Variant LinearComboFunction::getFirstFunction() {
    return Variant( first );
}

bool LinearComboFunction::setFirstWeight( const Variant& v ) {
	w1 = v.getReal();
    return true;
}

Variant LinearComboFunction::getFirstWeight() {
    return Variant( w1 );
}

bool LinearComboFunction::setSecondFunction( const Variant& v ) {
	delete second;
    second = v.getOutputFunction()->clone();
	second->setCluster( cl );
    return true;
}

Variant LinearComboFunction::getSecondFunction() {
    return Variant( second );
}

bool LinearComboFunction::setSecondWeight( const Variant& v ) {
	w2 = v.getReal();
    return true;
}

Variant LinearComboFunction::getSecondWeight() {
    return Variant( w2 );
}

LinearComboFunction* LinearComboFunction::clone() const {
	return new LinearComboFunction( w1, *first, w2, *second );
}

void LinearComboFunction::setCluster( Cluster* c ) {
	this->cl = c;
	mid.resize( c->numNeurons() );
	first->setCluster( c );
	second->setCluster( c );
}

}
