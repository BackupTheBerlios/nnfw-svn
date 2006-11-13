/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2006 Gianluca Massera <emmegian@yahoo.it>                     *
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

#ifndef MEMUTILS_H
#define MEMUTILS_H

#include "types.h"


namespace nnfw {

/*! \brief template for memory copy of data
 */
template<class T>
inline void memoryCopy( T* dest, const T* src, unsigned int size ) {
    for( unsigned int i=0; i<size; i++ ) {
        dest[i] = src[i];
    };
};

/*! \brief template for memory zeroing of data
 */
template<class T>
inline void memoryZeroing( T* data, unsigned int size ) {
    for( unsigned int i=0; i<size; i++ ) {
        data[i] = T();
    };
};

/*! \brief specialization of memoryCopy for float data
 */
inline void memoryCopy( float* dest, const float* src, unsigned int size ) {
    memcpy( dest, src, sizeof(float)*size );
};

/*! \brief specialization of memoryZeroing for float data
 */
inline void memoryZeroing( float* data, unsigned int size ) {
    memset( data, 0, sizeof(float)*size );
};

/*! \brief specialization of memoryCopy for double data
 */
inline void memoryCopy( double* dest, const double* src, unsigned int size ) {
    memcpy( dest, src, sizeof(double)*size );
};

/*! \brief specialization of memoryZeroing for double data
 */
inline void memoryZeroing( double* data, unsigned int size ) {
    memset( data, 0, sizeof(double)*size );
};

}

#endif
