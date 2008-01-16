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

#ifndef NNFWRANDOM_H
#define NNFWRANDOM_H

/*! \file
 *  \brief This file contains the Random Class
 *
 *  Random class is an utility class
 */

#include "types.h"


namespace nnfw {

/*! \brief Random class define some static method for accessing the random number generator
 *
 *  Details
 */
class NNFW_API Random {
public:
	/*! \name Static Interface */
	//@{

    /*! Set the seed of the random number generator
     */
    static void setSeed( long int seed );

	/*! Return a real number in [0;1)
     */
    static Real flatReal( );

    /*! Return a real number in range [min,max)
     */
    static Real flatReal( Real min, Real max );
	
    /*! Return a RealVec with all the values randomized in the range [min,max)
     */
    static RealVec& flatRealVec( RealVec& vec, Real min, Real max );
	
    /*! Return a RealMat with all the values randomized in the range [min,max)
     */
    static RealMat& flatRealMat( RealMat& mat, Real min, Real max );

    /*! Return a random boolean value; True value appear with probability specified as paramater
     */
    static bool boolean( Real trueProb );

    /*! Return a random boolean value; True and False values are the same probability
     */
    static bool boolean( );

    /*! Return a random integer value in between 0 and x-1
     */
    static u_int flatInt( u_int x );

    /*! Return a random integer value in between x and y-1
     */
    static int flatInt( int x, int y );

    /*! Return a random real value with a gaussian distribution
     */
	static Real gauss( Real mean, Real stdev );
};

}

#endif
