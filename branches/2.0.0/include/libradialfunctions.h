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
 */
class NNFW_API GaussFunction : public DerivableOutputFunction {
public:
    /*! \name Constructors */
    //@{

    /*! Construct */
    GaussFunction( double centre = 0.0, double variance = 1.0, double maxvaule = 1.0 );
    /*! Destructor */
    virtual ~GaussFunction() { /* Nothing to do */ };

    //@}
    /*! \name Interface */
    //@{

    /*! Set the centre */
    bool setCentre( double v );
    /*! Return the centre */
    double getCentre();

    /*! Set the Variance */
    bool setVariance( double v );
    /*! Return the variance */
    double getVariance();

    /*! Set the Max value */
    bool setMax( double v );
    /*! Return the Max */
    double getMax();

    /*! Implement the Gaussian function */
    virtual void apply( RealVec& inputs, RealVec& outputs );
    /*! derivate of Gauss function */
    virtual void derivate( const RealVec& x, const RealVec& y, RealVec& d ) const;
    /*! Clone this object */
    virtual GaussFunction* clone() const;

    //@}

private:
    // centre
    double centre;
    // variance
    double variance;
    // minus squared-variance
    double msqrvar;
    // max value
    double max;
};

}

#endif

