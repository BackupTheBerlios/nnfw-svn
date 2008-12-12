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
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>phase</td> <td>double</td> <td>read/write</td> <td> X offset of the centre </td> <td>this</td> </tr>
 *   <tr><td>span</td> <td>double</td> <td>read/write</td> <td> distance between peaks</td> <td>this</td> </tr>
 *   <tr><td>amplitude</td> <td>double</td> <td>read/write</td> <td>nonnegative value of the wave's magnitude</td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API PeriodicFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	PeriodicFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	
	/*! Construct */
	PeriodicFunction( PropertySettings& prop );
	
	/*! Destructor */
	virtual ~PeriodicFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Set the phase */
	bool setPhase( const Variant& v );
	/*! Return the phase */
	Variant phase();
	
	/*! Set the span */
	bool setSpan( const Variant& v );
	/*! Return the variance */
	Variant span();
	
	/*! Set the Amplitude */
	bool setAmplitude( const Variant& v );
	/*! Return the Max */
	Variant amplitude();
	
	/*! Implement the Sawtooth function */
	virtual void apply( doubleVec& inputs, doubleVec& outputs ) = 0;

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
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>phase</td> <td>double</td> <td>read/write</td> <td> X offset of the centre </td> <td>PeriodicFunction</td> </tr>
 *   <tr><td>span</td> <td>double</td> <td>read/write</td> <td> distance between peaks</td> <td>PeriodicFunction</td> </tr>
 *   <tr><td>amplitude</td> <td>double</td> <td>read/write</td> <td>nonnegative value of the wave's magnitude</td> <td>PeriodiFunction</td> </tr>
 *   </table>
 */
class NNFW_API SawtoothFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	SawtoothFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	
	/*! Construct */
	SawtoothFunction( PropertySettings& prop );
	
	/*! Destructor */
	virtual ~SawtoothFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Implement the Sawtooth function */
	virtual void apply( doubleVec& inputs, doubleVec& outputs );

	/*! Clone this object */
	virtual SawtoothFunction* clone() const;
	
	//@}
};

/*! \brief TriangleFunction
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>phase</td> <td>double</td> <td>read/write</td> <td> X offset of the centre </td> <td>PeriodicFunction</td> </tr>
 *   <tr><td>span</td> <td>double</td> <td>read/write</td> <td> distance between peaks</td> <td>PeriodicFunction</td> </tr>
 *   <tr><td>amplitude</td> <td>double</td> <td>read/write</td> <td>nonnegative value of the wave's magnitude</td> <td>PeriodicFunction</td> </tr>
 *   </table>
 */
class NNFW_API TriangleFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	TriangleFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	
	/*! Construct */
	TriangleFunction( PropertySettings& prop );
	
	/*! Destructor */
	virtual ~TriangleFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Implement the Triangle function */
	virtual void apply( doubleVec& inputs, doubleVec& outputs );

	/*! Clone this object */
	virtual TriangleFunction* clone() const;
	
	//@}

};

/*! \brief SinFunction
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>phase</td> <td>double</td> <td>read/write</td> <td> X offset of the centre </td> <td>PeriodicFunction</td> </tr>
 *   <tr><td>span</td> <td>double</td> <td>read/write</td> <td>distance between peaks; see frequency</td> <td>PeriodicFunction</td> </tr>
 *   <tr><td>amplitude</td> <td>double</td> <td>read/write</td> <td>nonnegative value of the wave's magnitude</td> <td>PeriodicFunction</td> </tr>
 *   </table>
 */
class NNFW_API SinFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	SinFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );

	/*! Construct */
	SinFunction( PropertySettings& prop );
	
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
	virtual void apply( doubleVec& inputs, doubleVec& outputs );

	/*! Clone this object */
	virtual SinFunction* clone() const;
	
	//@}
};

/*! \brief PseudoGaussFunction
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>phase</td> <td>double</td> <td>read/write</td> <td> X offset of the centre </td> <td>PeriodicFunction</td> </tr>
 *   <tr><td>span</td> <td>double</td> <td>read/write</td> <td> distance between peaks</td> <td>PeriodicFunction</td> </tr>
 *   <tr><td>amplitude</td> <td>double</td> <td>read/write</td> <td>nonnegative value of the wave's magnitude</td> <td>PeriodicFunction</td> </tr>
 *   </table>
 */
class NNFW_API PseudoGaussFunction : public PeriodicFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	PseudoGaussFunction( double phase = 0.0, double span = 1.0, double amplitude = 1.0 );
	
	/*! Construct */
	PseudoGaussFunction( PropertySettings& prop );
	
	/*! Destructor */
	virtual ~PseudoGaussFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Implement the Triangle function */
	virtual void apply( doubleVec& inputs, doubleVec& outputs );

	/*! Clone this object */
	virtual PseudoGaussFunction* clone() const;
	
	//@}

};

}

#endif

