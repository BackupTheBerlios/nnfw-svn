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

ScaleFunction::ScaleFunction( double rate )
    : OutputFunction() {
	this->rate = rate;
}

bool ScaleFunction::setRate( double v ) {
    rate = v;
    return true;
}

double ScaleFunction::getRate() {
    return rate;
}

void ScaleFunction::apply( RealVec& inputs, RealVec& outputs ) {
    outputs.assign_amulx( rate, inputs );
}

ScaleFunction* ScaleFunction::clone() const {
    return new ScaleFunction( rate );
}

GainFunction::GainFunction( double gain )
    : OutputFunction() {
	gainv = gain;
}

bool GainFunction::setGain( double v ) {
    gainv = v;
    return true;
}

double GainFunction::gain() {
    return gainv;
}

void GainFunction::apply( RealVec& inputs, RealVec& outputs ) {
    outputs.assign( inputs );
	outputs += gainv;
}

GainFunction* GainFunction::clone() const {
    return new GainFunction( gainv );
}

SigmoidFunction::SigmoidFunction( double l ) : DerivableOutputFunction() {
    lambda = l;
}

void SigmoidFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        qWarning() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
    outputs.assign_amulx( -lambda, inputs );
    outputs.exp();
    outputs += 1.0;
    outputs.inv();
}

bool SigmoidFunction::setLambda( double v ) {
    lambda = v;
    return true;
}

double SigmoidFunction::getLambda() {
    return lambda;
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

FakeSigmoidFunction::FakeSigmoidFunction( double l )
    : DerivableOutputFunction() {
    lambda = l;
}

void FakeSigmoidFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        qWarning() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
    unsigned int size = inputs.size();
    double x;
    double x0 = 6. + 2./3.;
    double zero = 0.5;
    for ( unsigned int i = 0; i<size; i++ ) {
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

bool FakeSigmoidFunction::setLambda( double v ) {
    lambda = v;
    return true;
}

double FakeSigmoidFunction::getLambda() {
    return lambda;
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

ScaledSigmoidFunction::ScaledSigmoidFunction( double l, double min, double max )
    : DerivableOutputFunction() {
    lambda = l;
    this->min = min;
    this->max = max;
}

void ScaledSigmoidFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        qWarning() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
    unsigned int size = inputs.size();
    outputs.assign_amulx( -lambda, inputs );
    outputs.exp();
    for ( unsigned int i = 0; i<size; i++ ) {
        outputs[i] = (max - min ) * (1.0/( 1.0 + outputs[i] )) + min;
    }
}

bool ScaledSigmoidFunction::setLambda( double v ) {
    lambda = v;
    return true;
}

double ScaledSigmoidFunction::getLambda() {
    return lambda;
}

bool ScaledSigmoidFunction::setMin( double v ) {
    min = v;
    return true;
}

double ScaledSigmoidFunction::getMin() {
    return min;
}

bool ScaledSigmoidFunction::setMax( double v ) {
    max = v;
    return true;
}

double ScaledSigmoidFunction::getMax() {
    return max;
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

RampFunction::RampFunction( double minX, double maxX, double minY, double maxY )
    : DerivableOutputFunction() {
	min_x = minX;
	max_x = maxX;
	min_y = minY;
	max_y = maxY;
}

void RampFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        qWarning() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
    unsigned int size = inputs.size();
    for ( unsigned int i = 0; i<size; i++ ) {
        double m = ( max_y-min_y )/( max_x-min_x );
        double q = min_y - m*min_x;
        double ret = m*(inputs[i]) + q;
        if (ret < min_y) {
            outputs[i] = min_y;
        } else if (ret > max_y) {
            outputs[i] = max_y;
        } else {
            outputs[i] = ret;
        }
    }
}

bool RampFunction::setMinX( double v ) {
    min_x = v;
    return true;
}

double RampFunction::minX() {
    return min_x;
}

bool RampFunction::setMaxX( double v ) {
	max_x = v;
	return true;
}

double RampFunction::maxX() {
	return max_x;
}

bool RampFunction::setMinY( double v ) {
	min_y = v;
	return true;
}

double RampFunction::minY() {
	return min_y;
}

bool RampFunction::setMaxY( double v ) {
	max_y = v;
	return true;
}

double RampFunction::maxY() {
	return max_y;
}

void RampFunction::derivate( const RealVec& inputs, const RealVec&, RealVec& derivates ) const {
    for( unsigned int i=0; i<inputs.size(); i++ ) {
        if ( inputs[i] >= min_x && inputs[i] <= max_x ) {
            derivates[i] = ( max_y-min_y )/( max_x-min_x );
        } else {
            double y;
            y = 1.0/( 1.0 + exp( -inputs[i] ) );
            derivates[i] = y * ( 1.0 - y );
        }
    }
}

RampFunction* RampFunction::clone() const {
    return new RampFunction( min_x, max_x, min_y, max_y );
}

LinearFunction::LinearFunction( double m, double b )
    : DerivableOutputFunction() {
    mv = m;
	bv = b;
}

void LinearFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
	if ( inputs.size() != outputs.size() ) {
		qWarning() << "The output dimension doesn't match the input dimension" ;
		return;
	}
#endif
	outputs.assign_amulx( mv, inputs );
	outputs += bv;
}

bool LinearFunction::setM( double v ) {
	mv = v;
	return true;
}

double LinearFunction::m() {
	return mv;
}

bool LinearFunction::setB( double v ) {
	bv = v;
	return true;
}

double LinearFunction::b() {
	return bv;
}

void LinearFunction::derivate( const RealVec& , const RealVec&, RealVec& derivates ) const {
	derivates.setAll( mv );
}

LinearFunction* LinearFunction::clone() const {
	return new LinearFunction( mv, bv );
}

StepFunction::StepFunction( double min, double max, double threshold )
    : DerivableOutputFunction() {
    this->min = min;
    this->max = max;
    this->threshold = threshold;
}

void StepFunction::apply( RealVec& inputs, RealVec& outputs ) {
    unsigned int size = inputs.size();
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        qWarning() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
    for ( unsigned int i = 0; i<size; i++ ) {
        ( inputs[i] > threshold ) ? outputs[i] = max : outputs[i] = min;
    }
}

bool StepFunction::setMin( double v ) {
    min = v;
    return true;
}

double StepFunction::getMin() {
    return min;
}

bool StepFunction::setMax( double v ) {
    max = v;
    return true;
}

double StepFunction::getMax() {
    return max;
}

bool StepFunction::setThreshold( double v ) {
    threshold = v;
    return true;
}

double StepFunction::getThreshold() {
    return threshold;
}

void StepFunction::derivate( const RealVec& inputs, const RealVec&, RealVec& derivates ) const {
    for( unsigned int i=0; i<inputs.size(); i++ ) {
        double y;
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
}

void LeakyIntegratorFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        qWarning() << "The output dimension doesn't match the input dimension" ;
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

LogLikeFunction::LogLikeFunction( double A, double B )
    : OutputFunction() {
	a = A;
	b = B;
}

void LogLikeFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        qWarning() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
	//--- y <- x / ( 1+A*x+b )
	outputs.assign_amulx( a, inputs );
	outputs += 1.0+b;
	outputs.inv();
	outputs *= inputs;
}

LogLikeFunction* LogLikeFunction::clone() const {
	return ( new LogLikeFunction( a, b ) );
}

PoolFunction::PoolFunction( const OutputFunction& prototype, unsigned int dim )
    : OutputFunction(), ups(dim) {
    for( unsigned int i=0; i<dim; i++ ) {
        ups[i] = prototype.clone();
    }
    // --- if dimension is zero, set at least one element to OutputFunction
    if ( dim == 0 ) {
#ifdef NNFW_DEBUG
        qWarning() << "The dimension of PoolFunction must be at least one";
#endif
        ups.resize( 1 );
        ups[0] = prototype.clone();
    }
}

PoolFunction::PoolFunction( unsigned int dim )
    : OutputFunction(), ups(dim) {
    // --- if dimension is zero, set at least one element to OutputFunction
    if ( dim == 0 ) {
#ifdef NNFW_DEBUG
        qWarning() << "The dimension of PoolFunction must be at least one" ;
#endif
        ups.resize( 1 );
    }
}

PoolFunction::~PoolFunction() {
    for( unsigned int i=0; i<ups.size(); i++ ) {
        delete (ups[i]);
    }
}

OutputFunction* PoolFunction::getOutputFunction( unsigned int i ) {
#ifdef NNFW_DEBUG
    if ( i >= ups.size() ) {
        qWarning() << "Accessing beyond boundary of this PoolFunction" ;
        return 0;
    }
#endif
    return ups[i];
}

void PoolFunction::setOutputFunction( unsigned int i, const OutputFunction& prototype ) {
#ifdef NNFW_DEBUG
    if ( i >= ups.size() ) {
        qWarning() << "Setting a OutputFunction beyond boundary of this PoolFunction";
        return;
    }
#endif
    delete (ups[i]);
    ups[i] = prototype.clone();
    return;
}

unsigned int PoolFunction::size() {
    return ups.size();
}

void PoolFunction::apply( RealVec& inputs, RealVec& outputs ) {
    unsigned int dim = ups.size();
    for( unsigned int i=0; i<dim; i++ ) {
        outputs[i] = ups[i]->apply( inputs[i] );
    }
}

PoolFunction* PoolFunction::clone() const {
    PoolFunction* pool = new PoolFunction( ups.size() );
    for( unsigned int i=0; i<ups.size(); i++ ) {
        pool->ups[i] = this->ups[i]->clone();
    }
    return pool;
}

void PoolFunction::setCluster( Cluster* c ) {
    unsigned int oldDim = ups.size();
    unsigned int newDim = c->numNeurons();
    ups.resize( newDim );
    for( unsigned int i=oldDim; i<newDim; i++ ) {
        ups[i] = new OutputFunction();
    }
}

CompositeFunction::CompositeFunction( const OutputFunction& f, const OutputFunction& g )
    : OutputFunction(), mid() {
	first = f.clone();
	second = g.clone();
}

CompositeFunction::~CompositeFunction() {
	delete first;
	delete second;
}

void CompositeFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        qWarning() << "The output dimension doesn't match the input dimension" ;
        return;
    }
#endif
	first->apply( inputs, mid );
	second->apply( mid, outputs );
}

bool CompositeFunction::setFirstFunction( const OutputFunction& f ) {
	delete first;
    first = f.clone();
	first->setCluster( cl );
    return true;
}

OutputFunction* CompositeFunction::getFirstFunction() {
    return first;
}

bool CompositeFunction::setSecondFunction( const OutputFunction& g ) {
	delete second;
    second = g.clone();
	second->setCluster( cl );
    return true;
}

OutputFunction* CompositeFunction::getSecondFunction() {
    return second;
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

LinearComboFunction::LinearComboFunction( double w1, const OutputFunction& f, double w2, const OutputFunction& g )
    : OutputFunction(), mid() {
	first = f.clone();
	second = g.clone();
	this->w1 = w1;
	this->w2 = w2;
}

LinearComboFunction::~LinearComboFunction() {
	delete first;
	delete second;
}

void LinearComboFunction::apply( RealVec& inputs, RealVec& outputs ) {
#ifdef NNFW_DEBUG
    if ( inputs.size() != outputs.size() ) {
        qWarning() << "The output dimension doesn't match the input dimension" ;
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

bool LinearComboFunction::setFirstFunction( const OutputFunction& f ) {
	delete first;
    first = f.clone();
	first->setCluster( cl );
    return true;
}

OutputFunction* LinearComboFunction::getFirstFunction() {
    return first;
}

bool LinearComboFunction::setFirstWeight( double v ) {
	w1 = v;
    return true;
}

double LinearComboFunction::getFirstWeight() {
    return w1;
}

bool LinearComboFunction::setSecondFunction( const OutputFunction& g ) {
	delete second;
    second = g.clone();
	second->setCluster( cl );
    return true;
}

OutputFunction* LinearComboFunction::getSecondFunction() {
    return second;
}

bool LinearComboFunction::setSecondWeight( double v ) {
	w2 = v;
    return true;
}

double LinearComboFunction::getSecondWeight() {
    return w2;
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
