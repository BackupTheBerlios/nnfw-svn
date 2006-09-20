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

#ifndef NNFW_H
#define NNFW_H

/*! \mainpage Neural Network Framework (release 0.4.7)
 *  <h1>Class Reference</h1>
 *
 *  This is a Neural Network Framework for creating, simulating and analyze any neural network you can image !!
 *  <br><br>
 *
 *
 *
 */

/*! \file
 *  \brief This is the main header files!! Include this file in your project... and play :-)
 *
 *  Details...
 *
 * \todo It's really useful this include file ?!?! I think that a file including everything is not very useful
 *       rather a set of files that includes only some categories of classes: example libclusters.h may includes
 *       all subclasses of Cluster, liblinkers.h may includes all subclasses of Linker, and so on.
 */

#include "types.h"
#include "messages.h"
#include "cluster.h"
#include "linker.h"
#include "neuralnet.h"
#include "liboutputfunctions.h"
#include "ionnfw.h"

#endif

