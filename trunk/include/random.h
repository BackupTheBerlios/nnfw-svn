/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005 Gianluca Massera <emmegian@yahoo.it>                     *
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

#ifndef nnfwRANDOM_H
#define nnfwRANDOM_H

/*! \file
 *  \brief This file contains the Random Class
 *
 *  Random class is an utility class
 */

#include "types.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Random class define some static method for accessing the random number generator
 *
 *  Details
 */
class Random
{
public:
    /*! \brief Set the seed of the random number generator
     *
     * Details..
     */
    static void setSeed( long int seed );

    /*! \brief Return a real number in range [min,max]
     *
     * Details..
     */
    static Real flatReal( Real min, Real max );

    /*! \brief Return a random boolean value; True value appear with probability specified as paramater
     *
     * Details..
     */
    static bool boolean( Real trueProb );

    /*! \brief Return a random boolean value; True and False values are the same probability
     *
     * Details..
     */
    static bool boolean( );
private:
//    static gsl_rng* rnd;
};

}

#endif
