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

#ifndef LIBCOMPETITIVEFUNCTIONS_H
#define LIBCOMPETITIVEFUNCTIONS_H

#include "types.h"

/*! \file
 *  \brief Library of Competitive OutputFunction
 *
 */

#include "outputfunction.h"

namespace nnfw {

/*! \brief WinnerTakeAllFunction
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>value</td> <td>Real</td> <td>read/write</td> <td> value of the output of winner </td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API WinnerTakeAllFunction : public OutputFunction {
public:
	/*! \name Constructors */
	//@{
	
	/*! Construct */
	WinnerTakeAllFunction( double value = 1.0 );
	
	/*! Construct */
	WinnerTakeAllFunction( PropertySettings& prop );
	
	/*! Destructor */
	virtual ~WinnerTakeAllFunction() { /* Nothing to do */ };
	
	//@}
	/*! \name Interface */
	//@{
	
	/*! Set the value */
	bool setValue( const Variant& v );
	/*! Return the value */
	Variant value();
	
	/*! Implement the Sawtooth function */
	virtual void apply( RealVec& inputs, RealVec& outputs );

	/*! Clone this object */
	virtual WinnerTakeAllFunction* clone() const;
	
	//@}

private:
	double valuev;
};


}

#endif

