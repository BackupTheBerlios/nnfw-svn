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

#ifndef MEMUTILS_H
#define MEMUTILS_H

/*! \file
 */

#include "types.h"

namespace nnfw {

/*!  template for memory copy of data
 */
template<class T>
inline void memoryCopy( T* dest, const T* src, unsigned int size ) {
    for( unsigned int i=0; i<size; i++ ) {
        dest[i] = src[i];
    };
};

/*! template for memory zeroing of data
 */
template<class T>
inline void memoryZeroing( T* data, unsigned int size ) {
    for( unsigned int i=0; i<size; i++ ) {
        data[i] = T();
    };
};

/*! specialization of memoryCopy for float data
 */
inline void memoryCopy( float* dest, const float* src, unsigned int size ) {
    memcpy( dest, src, sizeof(float)*size );
};

/*! specialization of memoryZeroing for float data
 */
inline void memoryZeroing( float* data, unsigned int size ) {
    memset( data, 0, sizeof(float)*size );
};

/*! specialization of memoryCopy for double data
 */
inline void memoryCopy( double* dest, const double* src, unsigned int size ) {
    memcpy( dest, src, sizeof(double)*size );
};

/*! specialization of memoryZeroing for double data
 */
inline void memoryZeroing( double* data, unsigned int size ) {
    memset( data, 0, sizeof(double)*size );
};

/*! specialization of memoryCopy for bool data
 */
inline void memoryCopy( bool* dest, const bool* src, unsigned int size ) {
    memcpy( dest, src, sizeof(bool)*size );
};

/*! specialization of memoryZeroing for bool data
 */
inline void memoryZeroing( bool* data, unsigned int size ) {
    memset( data, 0, sizeof(bool)*size );
};

}

#endif
