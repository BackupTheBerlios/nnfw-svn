/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                     *
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

#ifndef UTILS_H
#define UTILS_H

/*! \file
 *  \brief This file contains some utilities functions
 *
 * Details...
 */

#include "types.h"

#ifndef WIN32
	#include <sys/time.h>
#else
	#include <windows.h>
#endif

namespace nnfw {

/*! \brief SimpleTimer object
 *
 *  \par Motivation
 *  It is a simple timer for performance checks
 *
 *  \par Description
 *  SimpleTimer counts microseconds elapsed since the last tic() calls
 *
 *  \par Warnings
 *
 */
class SimpleTimer {
public:
	/*! Construct the timer */
	SimpleTimer() {
#ifdef NNFW_WIN
		QueryPerformanceFrequency( &frequency );
		QueryPerformanceCounter( &baseCount );
#else
		struct timeval tv;
		gettimeofday( &tv, NULL );
		lastTime = tv.tv_sec*1000000 + tv.tv_usec;
#endif
	};
	/*! return microsecond elapsed from last tic() call */
	int tac() {
#ifdef NNFW_WIN
		unsigned ticks;
		QueryPerformanceCounter( &count );
		count.QuadPart -= baseCount.QuadPart;
		ticks = unsigned( count.QuadPart * LONGLONG (1000000) / frequency.QuadPart );
		return ticks;
#else
		struct timeval tv;
		gettimeofday( &tv, NULL );
		return (tv.tv_sec*1000000 + tv.tv_usec) - lastTime;
#endif
	};
	/*! return microsecond elapsed from last tic() call */
	int tic() {
#ifdef NNFW_WIN
		unsigned ticks;
		QueryPerformanceCounter( &count );
		count.QuadPart -= baseCount.QuadPart;
		ticks = unsigned( count.QuadPart * LONGLONG (1000000) / frequency.QuadPart );
		baseCount = count;
		return ticks;
#else
		struct timeval tv;
		gettimeofday( &tv, NULL );
		int ret = (tv.tv_sec*1000000 + tv.tv_usec) - lastTime;
		lastTime = (tv.tv_sec*1000000 + tv.tv_usec);
		return ret;
#endif
	};
private:
#ifdef NNFW_WIN
	LARGE_INTEGER count;
	LARGE_INTEGER frequency;
	LARGE_INTEGER baseCount;
#else
	long int lastTime;
#endif
};

}

#endif
