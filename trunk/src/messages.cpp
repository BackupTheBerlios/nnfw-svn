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

#include "messages.h"
#include <iostream>

extern void exit( int status );

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

//using namespace std;

void nnfwMessage( unsigned int category, const char* msg ) {
    switch( category ) {
    case NNFW_INFORMATION:
        std::cout << "== Information: " << msg << std::endl;
        return;
    case NNFW_WARNING:
        std::cout << "== Warning: " << msg << std::endl;
        return;
    case NNFW_ERROR:
        std::cout << "== ERROR: " << msg << std::endl;
        return;
    case NNFW_CRITICAL:
        std::cout << "== CRITICAL ERROR: " << msg << std::endl;
        exit( 1 );
    }
    std::cout << "== Unrecognized Category: " << msg << std::endl;
    return;
}

}
