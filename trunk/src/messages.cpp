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

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

using namespace std;

void nnfwMessage( unsigned int category, const char* msg ) {
    switch( category ) {
    case NNFW_INFORMATION:
        cout << "== Information: " << msg << endl;
        return;
    case NNFW_WARNING:
        cout << "== Warning: " << msg << endl;
        return;
    case NNFW_ERROR:
        cout << "== ERROR: " << msg << endl;
        return;
    case NNFW_CRITICAL:
        cout << "== CRITICAL ERROR: " << msg << endl;
        exit( 1 );
    }
    cout << "== Unrecognized Category: " << msg << endl;
    return;
}

}
