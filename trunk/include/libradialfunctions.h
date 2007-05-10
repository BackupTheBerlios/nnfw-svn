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

#ifndef LIBRADIALFUNCTIONS_H
#define LIBRADIALFUNCTIONS_H

#include "types.h"

/*! \file
 *  \brief Library of Radial OutputFunction
 *
 */

#include "outputfunction.h"
#include "derivableoutputfunction.h"


namespace nnfw {

/*! \brief GaussFunction
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>centre</td> <td>Real</td> <td>read/write</td> <td> Gaussian centre </td> <td>this</td> </tr>
 *   <tr><td>variance</td> <td>Real</td> <td>read/write</td> <td> Gaussian variance (sigma) </td> <td>this</td> </tr>
 *   <tr><td>max</td> <td>Real</td> <td>read/write</td> <td>function's maximum value</td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API GaussFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    //! Construct
    GaussFunction( Real centre = 0.0, Real variance = 1.0, Real maxvaule = 1.0 );

    //! Construct
    GaussFunction( PropertySettings& prop );

    //! Destructor
    virtual ~GaussFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Set the centre
     */
    bool setCentre( const Variant& v );

    /*! Return the centre
     */
    Variant getCentre();

    /*! Set the Variance
     */
    bool setVariance( const Variant& v );

    /*! Return the variance
     */
    Variant getVariance();

    /*! Set the Max value
     */
    bool setMax( const Variant& v );

    /*! Return the Max
     */
    Variant getMax();

    //! Implement the identity function
    virtual void apply( RealVec& inputs, RealVec& outputs );

    //! ???
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;

    /*! Clone this object
     */
    virtual GaussFunction* clone() const;

    //@}
private:

    // centre
    Real centre;
    // variance
    Real variance;
    // minus squared-variance
    Real msqrvar;
    // max value
    Real max;
};

}

#endif

