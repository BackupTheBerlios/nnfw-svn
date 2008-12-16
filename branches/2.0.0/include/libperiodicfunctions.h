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

#ifndef LIBPERIODICFUNCTIONS_H
#define LIBPERIODICFUNCTIONS_H

#include "types.h"

/*! \file
 *  \brief Library of Periodic OutputFunction
 *
 */

#include "outputfunction.h"

namespace nnfw {

/*! \brief PeriodicFunction
 *
 */
class NNFW_API PeriodicFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	PeriodicFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~PeriodicFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Set the phase */
	bool setPhase( double v );
	/*! Return the phase */
	double phase();
	
	/*! Set the span */
	bool setSpan( double v );
	/*! Return the variance */
	double span();
	
	/*! Set the Amplitude */
	bool setAmplitude( double v );
	/*! Return the Max */
	double amplitude();
	
	/*! Implement the Periodic function */
	virtual void apply( RealVec& inputs, RealVec& outputs ) = 0;

	/*! Clone this object */
	virtual PeriodicFunction* clone() const = 0;
	
	//@}

protected:
	double phasev;
	double spanv;
	double amplitudev;
};

/*! \brief SawtoothFunction
 *
 */
class NNFW_API SawtoothFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	SawtoothFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~SawtoothFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Implement the Sawtooth function */
	virtual void apply( RealVec& inputs, RealVec& outputs );

	/*! Clone this object */
	virtual SawtoothFunction* clone() const;
	
	//@}
};

/*! \brief TriangleFunction
 *
 */
class NNFW_API TriangleFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	TriangleFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~TriangleFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Implement the Triangle function */
	virtual void apply( RealVec& inputs, RealVec& outputs );

	/*! Clone this object */
	virtual TriangleFunction* clone() const;
	
	//@}

};

/*! \brief SinFunction
 *
 */
class NNFW_API SinFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	SinFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~SinFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{

	/*! Return the frequency of the sinusoidal wave
	 *  \param frequency is equal to 2*pi_greco/span
	 */
	double frequency();
	
	/*! Implement the Sin function */
	virtual void apply( RealVec& inputs, RealVec& outputs );

	/*! Clone this object */
	virtual SinFunction* clone() const;
	
	//@}
};

/*! \brief PseudoGaussFunction
 *
 */
class NNFW_API PseudoGaussFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	PseudoGaussFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	/*! Destructor */
	virtual ~PseudoGaussFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Implement the Triangle function */
	virtual void apply( RealVec& inputs, RealVec& outputs );

	/*! Clone this object */
	virtual PseudoGaussFunction* clone() const;
	
	//@}

};

}

#endif

