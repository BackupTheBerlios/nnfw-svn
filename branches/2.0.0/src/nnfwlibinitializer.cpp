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

#include "nnfwfactory.h"

using namespace nnfw;

#ifdef NNFW_LINUX
void initUtilitiesLib() __attribute__ ((constructor));
void initUtilitiesLib() {
#ifdef NNFW_DEBUG
	qDebug() << "NNFW Initialization";
#endif
	Factory::initFactory();
}
#endif
#ifdef NNFW_WIN
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved ) {
	// Perform actions based on the reason for calling.
	switch( fdwReason ) {
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
#ifdef NNFW_DEBUG
		qDebug() << "NNFW Initialization";
#endif
		Factory::initFactory();
		break;
	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;
	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;
	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}
	return TRUE;
}
#endif
#ifdef NNFW_MAC
void initUtilitiesLib() __attribute__ ((constructor));
void initUtilitiesLib() {
#ifdef NNFW_DEBUG
	qDebug() << "NNFW Initialization";
#endif
	Factory::initFactory();
}
#endif

