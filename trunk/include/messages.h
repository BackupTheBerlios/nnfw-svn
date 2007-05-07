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

// --- You can't include it directly
#ifndef TYPES_INCLUDES
#error "You can't include messages.h directly; Instead, You have to include types.h"
#endif

#ifndef MESSAGGES_H
#define MESSAGGES_H


namespace nnfw {

/*! \file
 *  \brief This file contains the declaration of function nnfwMessage for displaying debug messages; Don't include this file directly, instead include types.h
 *
 *  The messages are divided in four categories:
 *  <ul>
 *     <li><b>NNFW_INFORMATION</b>: it's only an informational message, this is not an error </li>
 *     <li><b>NNFW_WARNING</b>: it isn't a true error, and maybe you can ignore this messages...
 *          but it's not guaranteed that an error may happens in other methods during the simulation </li>
 *     <li><b>NNFW_ERROR</b>: it's a error. The program will not be terminated, but the error will be corrected
 *         in such way that the program can go on. In the message will be indicated the correction procedure applied.
 *         The user of library must avoid that this messages arises </li>
 *     <li><b>NNFW_CRITICAL</b>: it's a critical error. It's not possibile go on !!! The program will be
 *         terminated. The user of library must avoid that this messages arises </li>
 *  </ul>
 */

//! Categories of messages
enum {
    NNFW_INFORMATION = 0,
    NNFW_WARNING = 1,
    NNFW_ERROR = 2,
    NNFW_CRITICAL = 3
};

/*! \brief Print out a message
 *
 */
NNFW_API void nnfwMessage( unsigned int category, const char* msg );

}

#endif
