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

#include "liboutputfunctions.h"
#include "cluster.h"

#include <cmath>

namespace nnfw {

IdentityFunction::IdentityFunction()
	: OutputFunction() {
}

void IdentityFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	outputs.copy( inputs );
}

bool IdentityFunction::derivate( const DoubleVector&, const DoubleVector&, DoubleVector& derivates ) const {
    derivates.setAll( 1.0 );
	return true;
}

IdentityFunction* IdentityFunction::clone() const {
	return new IdentityFunction();
}

ScaleFunction::ScaleFunction( double rate )
	: OutputFunction() {
	this->rate = rate;
}

void ScaleFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	mul( outputs, rate, inputs );
}

bool ScaleFunction::derivate( const DoubleVector&, const DoubleVector&, DoubleVector& derivates ) const {
    derivates.setAll( rate );
	return true;
}

ScaleFunction* ScaleFunction::clone() const {
	return new ScaleFunction( rate );
}

GainFunction::GainFunction( double gain )
	: OutputFunction() {
	gainv = gain;
}

void GainFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	outputs.copy( inputs );
	outputs += gainv;
}

GainFunction* GainFunction::clone() const {
	return new GainFunction( gainv );
}

SigmoidFunction::SigmoidFunction( double l ) : OutputFunction() {
	lambda = l;
}

void SigmoidFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	// ____________1_________________
	//   exp( -lamba*inputs ) + 1
	inv( exp( mul( outputs, -lambda, inputs ) ) += 1.0 );
}

bool SigmoidFunction::derivate( const DoubleVector&, const DoubleVector& outputs, DoubleVector& derivates ) const {
	// derivates <- lambda * out * (1.0-out)
	minus( derivates, 1.0, outputs );
	derivates *= outputs;
	derivates *= lambda;
	return true;
}

SigmoidFunction* SigmoidFunction::clone() const {
	return new SigmoidFunction( lambda );
}

FakeSigmoidFunction::FakeSigmoidFunction( double l )
	: OutputFunction() {
	lambda = l;
}

void FakeSigmoidFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
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

bool FakeSigmoidFunction::derivate( const DoubleVector&, const DoubleVector& outputs, DoubleVector& derivates ) const {
	// derivates <- lambda * out * (1.0-out)
	minus( derivates, 1.0, outputs );
	derivates *= outputs;
	derivates *= lambda;
	return true;
}

FakeSigmoidFunction* FakeSigmoidFunction::clone() const {
	return new FakeSigmoidFunction( lambda );
}

ScaledSigmoidFunction::ScaledSigmoidFunction( double l, double min, double max )
	: OutputFunction() {
	lambda = l;
	this->min = min;
	this->max = max;
}

void ScaledSigmoidFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	//--- compute the sigmoid
	// ____________1_________________
	//   exp( -lamba*inputs ) + 1
	inv( exp( mul( outputs, -lambda, inputs ) ) += 1.0 );
	//--- and scale it
	outputs *= max-min;
	outputs += min;
}

bool ScaledSigmoidFunction::derivate( const DoubleVector&, const DoubleVector& outputs, DoubleVector& derivates ) const {
	// derivates <- lambda * out * (1.0-out)
	minus( derivates, 1.0, outputs );
	derivates *= outputs;
	derivates *= lambda;
	return true;
}

ScaledSigmoidFunction* ScaledSigmoidFunction::clone() const {
	return new ScaledSigmoidFunction( lambda, min, max );
}

RampFunction::RampFunction( double minX, double maxX, double minY, double maxY )
	: OutputFunction() {
	min_x = minX;
	max_x = maxX;
	min_y = minY;
	max_y = maxY;
}

void RampFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	unsigned int size = inputs.size();
	double m = ( max_y-min_y )/( max_x-min_x );
	double q = min_y - m*min_x;
	for ( unsigned int i = 0; i<size; i++ ) {
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

bool RampFunction::derivate( const DoubleVector& inputs, const DoubleVector&, DoubleVector& derivates ) const {
	for( unsigned int i=0; i<inputs.size(); i++ ) {
		if ( inputs[i] >= min_x && inputs[i] <= max_x ) {
			derivates[i] = ( max_y-min_y )/( max_x-min_x );
		} else {
			double y;
			y = 1.0/( 1.0 + std::exp( -inputs[i] ) );
			derivates[i] = y * ( 1.0 - y );
		}
	}
	return true;
}

RampFunction* RampFunction::clone() const {
	return new RampFunction( min_x, max_x, min_y, max_y );
}

LinearFunction::LinearFunction( double m, double b )
	: OutputFunction() {
	this->m = m;
	this->b = b;
}

void LinearFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	mul( outputs, m, inputs ) += b;
}

bool LinearFunction::derivate( const DoubleVector& , const DoubleVector&, DoubleVector& derivates ) const {
	derivates.setAll( m );
	return true;
}

LinearFunction* LinearFunction::clone() const {
	return new LinearFunction( m, b );
}

StepFunction::StepFunction( double min, double max, double threshold )
	: OutputFunction() {
	this->min = min;
	this->max = max;
	this->threshold = threshold;
}

void StepFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	unsigned int size = inputs.size();
	for ( unsigned int i = 0; i<size; i++ ) {
		( inputs[i] > threshold ) ? outputs[i] = max : outputs[i] = min;
	}
}

bool StepFunction::derivate( const DoubleVector& inputs, const DoubleVector&, DoubleVector& derivates ) const {
	//--- return the same as if it is a sigmoid with lambda = 1
	for( unsigned int i=0; i<inputs.size(); i++ ) {
		double y;
		y = 1.0/( 1.0 + std::exp( -inputs[i] ) );
		derivates[i] = y * ( 1.0 - y );
	}
	return true;
}

StepFunction* StepFunction::clone() const {
	return new StepFunction( min, max, threshold );
}

LeakyIntegratorFunction::LeakyIntegratorFunction( const DoubleVector& d )
	: OutputFunction(), delta(d.size()), outprev(d.size()) {
	delta.copy( d );
	outprev.zeroing();
}

void LeakyIntegratorFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	//--- y <- delta*y(t-1) + (1.0-delta)*inputs
	//---  its equivalent to
	//--- y <- delta*( y(t-1) - inputs ) + inputs
	outputs = minus( outputs, outprev, inputs );
	outputs *= delta;
	outputs += inputs;
	outprev.copy( outputs );
}

void LeakyIntegratorFunction::zeroingStatus() {
	outprev.zeroing();
}

LeakyIntegratorFunction* LeakyIntegratorFunction::clone() const {
	LeakyIntegratorFunction* cl = new LeakyIntegratorFunction( delta );
	cl->outprev.copy( outprev );
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
	this->A = A;
	this->B = B;
}

void LogLikeFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	//--- y <- x / ( 1+A*x+b )
	outputs = inv( mul( outputs, A, inputs ) += (1.0+B) );
	outputs *= inputs;
}

LogLikeFunction* LogLikeFunction::clone() const {
	return ( new LogLikeFunction( A, B ) );
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
	for( int i=0; i<ups.size(); i++ ) {
		delete (ups[i]);
	}
}

OutputFunction* PoolFunction::getOutputFunction( unsigned int i ) {
#ifdef NNFW_DEBUG
	if ( (int)i >= ups.size() ) {
		qWarning() << "Accessing beyond boundary of this PoolFunction" ;
		return 0;
	}
#endif
	return ups[i];
}

void PoolFunction::setOutputFunction( unsigned int i, const OutputFunction& prototype ) {
#ifdef NNFW_DEBUG
	if ( (int)i >= ups.size() ) {
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

void PoolFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	unsigned int dim = ups.size();
	for( unsigned int i=0; i<dim; i++ ) {
		outputs[i] = ups[i]->apply( inputs[i] );
	}
}

PoolFunction* PoolFunction::clone() const {
	PoolFunction* pool = new PoolFunction( ups.size() );
	for( int i=0; i<ups.size(); i++ ) {
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

void CompositeFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
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

void LinearComboFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
#ifdef NNFW_DEBUG
	if ( inputs.size() != outputs.size() ) {
		qWarning() << "The output dimension doesn't match the input dimension" ;
		return;
	}
#endif
	mid.copy( outputs );
	first->apply( inputs, mid );
	mid *= w1 ;
	second->apply( inputs, outputs );
	outputs *= w2;
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
