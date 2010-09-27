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
#include <QStringList>
#include <QRegExp>
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

void IdentityFunction::configure(ConfigurationParameters& params, QString prefix)
{
	// Nothing to configure
}

void IdentityFunction::save(ConfigurationParameters& params, QString prefix)
{
	// Just telling our type to ConfigurationParameters
	params.startObjectParameters(prefix, "IdentityFunction", this);
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

void ScaleFunction::configure(ConfigurationParameters& params, QString prefix)
{
	rate = 1.0;
	QString str = params.getValue(prefix + "rate").
	if (!str.isNull()) {
		bool ok;
		rate = str.toDouble(&ok);
		if (!ok) {
			rate = 1.0;
		}
	}
}

void ScaleFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "ScaleFunction", this);

	params.createParameter(prefix, "rate", QString::number(rate));
}

GainFunction::GainFunction( double gain )
	: OutputFunction() {
	gainv = gain;
}

void GainFunction::apply( DoubleVector& inputs, DoubleVector& outputs ) {
	outputs.copy( inputs );
	outputs += gainv;
}

bool GainFunction::derivate( const DoubleVector&, const DoubleVector&, DoubleVector& derivates ) const {
    derivates.setAll( 1.0 );
	return true;
}

void GainFunction::configure(ConfigurationParameters& params, QString prefix)
{
	gainv = 1.0;
	QString str = params.getValue(prefix + "gain").
	if (!str.isNull()) {
		bool ok;
		gainv = str.toDouble(&ok);
		if (!ok) {
			gainv = 1.0;
		}
	}
}

void GainFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "GainFunction", this);

	params.createParameter(prefix, "gain", QString::number(gainv));
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

void SigmoidFunction::configure(ConfigurationParameters& params, QString prefix)
{
	lambda = 1.0;
	QString str = params.getValue(prefix + "lambda").
	if (!str.isNull()) {
		bool ok;
		lambda = str.toDouble(&ok);
		if (!ok) {
			lambda = 1.0;
		}
	}
}

void SigmoidFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "SigmoidFunction", this);

	params.createParameter(prefix, "lambda", QString::number(lambda));
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

void FakeSigmoidFunction::configure(ConfigurationParameters& params, QString prefix)
{
	lambda = 1.0;
	QString str = params.getValue(prefix + "lambda").
	if (!str.isNull()) {
		bool ok;
		lambda = str.toDouble(&ok);
		if (!ok) {
			lambda = 1.0;
		}
	}
}

void FakeSigmoidFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "FakeSigmoidFunction", this);

	params.createParameter(prefix, "lambda", QString::number(lambda));
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

void ScaledSigmoidFunction::configure(ConfigurationParameters& params, QString prefix)
{
	lambda = 1.0;
	QString str = params.getValue(prefix + "lambda").
	if (!str.isNull()) {
		bool ok;
		lambda = str.toDouble(&ok);
		if (!ok) {
			lambda = 1.0;
		}
	}

	min = -1.0;
	str = params.getValue(prefix + "min").
	if (!str.isNull()) {
		bool ok;
		min = str.toDouble(&ok);
		if (!ok) {
			min = -1.0;
		}
	}

	max = 1.0;
	str = params.getValue(prefix + "max").
	if (!str.isNull()) {
		bool ok;
		max = str.toDouble(&ok);
		if (!ok) {
			max = 1.0;
		}
	}
}

void ScaledSigmoidFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "ScaledSigmoidFunction", this);

	params.createParameter(prefix, "lambda", QString::number(lambda));

	params.createParameter(prefix, "min", QString::number(min));

	params.createParameter(prefix, "max", QString::number(max));
}

RampFunction::RampFunction()
	: OutputFunction() {
	min_x = 0.0;
	max_x = 0.0;
	min_y = 0.0;
	max_y = 0.0;
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

void RampFunction::configure(ConfigurationParameters& params, QString prefix)
{
	min_x = 0.0;
	QString str = params.getValue(prefix + "minX").
	if (!str.isNull()) {
		bool ok;
		min_x = str.toDouble(&ok);
		if (!ok) {
			min_x = 0.0;
		}
	}

	max_x = 0.0;
	str = params.getValue(prefix + "maxX").
	if (!str.isNull()) {
		bool ok;
		max_x = str.toDouble(&ok);
		if (!ok) {
			max_x = 0.0;
		}
	}

	min_y = 0.0;
	str = params.getValue(prefix + "minY").
	if (!str.isNull()) {
		bool ok;
		min_y = str.toDouble(&ok);
		if (!ok) {
			min_y = 0.0;
		}
	}

	max_y = 0.0;
	str = params.getValue(prefix + "maxY").
	if (!str.isNull()) {
		bool ok;
		max_y = str.toDouble(&ok);
		if (!ok) {
			max_y = 0.0;
		}
	}
}

void RampFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "RampFunction", this);

	params.createParameter(prefix, "minX", QString::number(min_x));

	params.createParameter(prefix, "maxX", QString::number(max_x));

	params.createParameter(prefix, "minY", QString::number(min_y));

	params.createParameter(prefix, "maxY", QString::number(max_y));
}

LinearFunction::LinearFunction()
	: OutputFunction() {
	m = 0.0;
	b = 0.0;
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

void LinearFunction::configure(ConfigurationParameters& params, QString prefix)
{
	m = 0.0;
	QString str = params.getValue(prefix + "m").
	if (!str.isNull()) {
		bool ok;
		m = str.toDouble(&ok);
		if (!ok) {
			m = 0.0;
		}
	}

	b = 0.0;
	str = params.getValue(prefix + "b").
	if (!str.isNull()) {
		bool ok;
		b = str.toDouble(&ok);
		if (!ok) {
			b = 0.0;
		}
	}
}

void LinearFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "LinearFunction", this);

	params.createParameter(prefix, "m", QString::number(m));

	params.createParameter(prefix, "b", QString::number(b));
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

void StepFunction::configure(ConfigurationParameters& params, QString prefix)
{
	min = 0.0;
	QString str = params.getValue(prefix + "min").
	if (!str.isNull()) {
		bool ok;
		min = str.toDouble(&ok);
		if (!ok) {
			min = 0.0;
		}
	}

	max = 1.0;
	str = params.getValue(prefix + "max").
	if (!str.isNull()) {
		bool ok;
		max = str.toDouble(&ok);
		if (!ok) {
			max = 1.0;
		}
	}

	threshold = 0.0;
	str = params.getValue(prefix + "threshold").
	if (!str.isNull()) {
		bool ok;
		threshold = str.toDouble(&ok);
		if (!ok) {
			threshold = 0.0;
		}
	}
}

void StepFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "StepFunction", this);

	params.createParameter(prefix, "min", QString::number(min));

	params.createParameter(prefix, "max", QString::number(max));

	params.createParameter(prefix, "threshold", QString::number(threshold));
}

LeakyIntegratorFunction::LeakyIntegratorFunction()
	: OutputFunction(), delta(), outprev()
{
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

void LeakyIntegratorFunction::setCluster( Cluster* c ) {
	if ( c->numNeurons() != delta.size() ) {
		delta.resize( c->numNeurons() );
		outprev.resize( c->numNeurons() );
	}
}

void LeakyIntegratorFunction::configure(ConfigurationParameters& params, QString prefix)
{
	// Delta is a vector, that is a list of space-separated values
	QString str = params.getValue(prefix + "delta").
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		delta.resize(list.size());

		for (unsigned int i = 0; i < list.size(); i++) {
			bool ok;
			delta[i] = list[i].toDouble(&ok);
			if (!ok) {
				delta[i] = 0.0;
			}
		}
	}

	// Also reloading outprev (resizing it to match delta length)
	QString str = params.getValue(prefix + "outprev").
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		outprev.resize(list.size());

		for (unsigned int i = 0; i < list.size(); i++) {
			bool ok;
			outprev[i] = list[i].toDouble(&ok);
			if (!ok) {
				outprev[i] = 0.0;
			}
		}
	}
	outprev.resize(delta.size());
}

void LeakyIntegratorFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "LeakyIntegratorFunction", this);

	// First creating a string list, then transforming to a single string
	QStringList list;
	for (unsigned int i = 0; i < delta.size(); i++) {
		list.push_back(QString::number(delta[i]));
	}
	params.createParameter(prefix, "delta", list.join(" "));

	// Saving in the same way also outprev
	QStringList list;
	for (unsigned int i = 0; i < delta.size(); i++) {
		list.push_back(QString::number(outprev[i]));
	}
	params.createParameter(prefix, "outprev", list.join(" "));
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

void LogLikeFunction::configure(ConfigurationParameters& params, QString prefix)
{
	A = 1.0;
	QString str = params.getValue(prefix + "A").
	if (!str.isNull()) {
		bool ok;
		A = str.toDouble(&ok);
		if (!ok) {
			A = 1.0;
		}
	}

	B = 5.0;
	str = params.getValue(prefix + "B").
	if (!str.isNull()) {
		bool ok;
		B = str.toDouble(&ok);
		if (!ok) {
			B = 5.0;
		}
	}
}

void LogLikeFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "LogLikeFunction", this);

	params.createParameter(prefix, "A", QString::number(A));

	params.createParameter(prefix, "B", QString::number(B));
}

CompositeFunction::CompositeFunction()
	: OutputFunction(), mid(), first(), second() {
}

CompositeFunction::CompositeFunction( OutputFunction *f, OutputFunction *g )
	: OutputFunction(), mid(), first(f), second(g) {
}

CompositeFunction::~CompositeFunction() {
	// auto_ptr will release memory for us
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

bool CompositeFunction::setFirstFunction( OutputFunction *f ) {
	first.reset(f);
	first->setCluster( cl );
	return true;
}

OutputFunction& CompositeFunction::getFirstFunction() {
	return *first;
}

bool CompositeFunction::setSecondFunction( OutputFunction *g ) {
	second.reset(g);
	second->setCluster( cl );
	return true;
}

OutputFunction& CompositeFunction::getSecondFunction() {
	return *second;
}

void CompositeFunction::setCluster( Cluster* c ) {
	this->cl = c;
	mid.resize( c->numNeurons() );
	first->setCluster( c );
	second->setCluster( c );
}

void CompositeFunction::configure(ConfigurationParameters& params, QString prefix)
{
	// We don't need configured component functions here (and they will be
	// configured after exiting from this function)
	first.reset(params.getObjectFromParameter(prefix + "first", false, false);
	second.reset(params.getObjectFromParameter(prefix + "second", false, false);

	// We don't need to reload a reference to the cluster as he calls our setCluster
	// function after our creation
}

void CompositeFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "CompositeFunction", this);

	params.createParameter(prefix, "first", first.get());

	params.createParameter(prefix, "second", second.get());

	// We don't need to save the reference to the cluster as he calls our setCluster
	// function after our creation
}

LinearComboFunction::LinearComboFunction()
	: OutputFunction(), mid(), first(), second()
{
	this->w1 = 0.0;
	this->w2 = 0.0;
}

LinearComboFunction::LinearComboFunction( double w1, OutputFunction *f, double w2, OutputFunction *g )
	: OutputFunction(), mid(), first(f), second(g) {
	this->w1 = w1;
	this->w2 = w2;
}

LinearComboFunction::~LinearComboFunction() {
	// auto_ptr will release memory for us
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

bool LinearComboFunction::setFirstFunction( OutputFunction *f ) {
	first.reset(f);
	first->setCluster( cl );
	return true;
}

OutputFunction& LinearComboFunction::getFirstFunction() {
	return *first;
}

bool LinearComboFunction::setFirstWeight( double v ) {
	w1 = v;
	return true;
}

double LinearComboFunction::getFirstWeight() {
	return w1;
}

bool LinearComboFunction::setSecondFunction( OutputFunction *g ) {
	second.reset(g);
	second->setCluster( cl );
	return true;
}

OutputFunction& LinearComboFunction::getSecondFunction() {
	return *second;
}

bool LinearComboFunction::setSecondWeight( double v ) {
	w2 = v;
	return true;
}

double LinearComboFunction::getSecondWeight() {
	return w2;
}

void LinearComboFunction::setCluster( Cluster* c ) {
	this->cl = c;
	mid.resize( c->numNeurons() );
	first->setCluster( c );
	second->setCluster( c );
}

void LinearComboFunction::configure(ConfigurationParameters& params, QString prefix)
{
	// We don't need configured component functions here (and they will be
	// configured after exiting from this function)
	first.reset(params.getObjectFromParameter(prefix + "first", false, false);

	w1 = 0.0;
	QString str = params.getValue(prefix + "w1").
	if (!str.isNull()) {
		bool ok;
		w1 = str.toDouble(&ok);
		if (!ok) {
			w1 = 0.0;
		}
	}

	second.reset(params.getObjectFromParameter(prefix + "second", false, false);

	w2 = 0.0;
	str = params.getValue(prefix + "w2").
	if (!str.isNull()) {
		bool ok;
		w2 = str.toDouble(&ok);
		if (!ok) {
			w2 = 0.0;
		}
	}

	// We don't need to reload a reference to the cluster as he calls our setCluster
	// function after our creation
}

void LinearComboFunction::save(ConfigurationParameters& params, QString prefix)
{
	params.startObjectParameters(prefix, "LinearComboFunction", this);

	params.createParameter(prefix, "first", first.get());

	params.createParameter(prefix, "w1", QString::number(w1));

	params.createParameter(prefix, "second", second.get());

	params.createParameter(prefix, "w2", QString::number(w2));

	// We don't need to save the reference to the cluster as he calls our setCluster
	// function after our creation
}

}
