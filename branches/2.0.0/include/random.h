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
	/*! Set the seed of the random number generator */
	static void setSeed( long int seed );
	/*! Return a double number in [0;1) */
	static double flatDouble( );
	/*! Return a double number in range [min,max) */
	static double flatDouble( double min, double max );
	/*! Return a vector with all the values randomized in the range [min,max) */
	static DoubleVector& flatVector( DoubleVector& vec, double min, double max );
	/*! Return a RealMat with all the values randomized in the range [min,max) */
	static DoubleMatrix& flatMatrix( DoubleMatrix& mat, double min, double max );
	/*! Return a random boolean value; True value appear with probability specified as paramater */
	static bool boolean( double trueProb );
	/*! Return a random boolean value; True and False values are the same probability */
	static bool boolean( );
	/*! Return a random integer value in between 0 and x-1 */
	static unsigned int flatInt( unsigned int x );
	/*! Return a random integer value in between x and y-1 */
	static int flatInt( int x, int y );
	/*! Return a random double value with a gaussian distribution */
	static double gauss( double mean, double stdev );
};

}

#endif
