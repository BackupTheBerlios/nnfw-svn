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
#include <sys/time.h>

namespace nnfw {

class BaseNeuralNet;
class LearningNetwork;

/*! Function to quickly construct a FeedForward Neural Network<br>
 *  Return a BaseNeuralNetwork that represent a feedforwar neural net.<br>
 *  The neural network returned is composed by Cluster of clusterType speficied.
 *  The number of neurons of each Cluster is taken from layers vector.
 *  Also, the first Cluster is setted as input of BaseNeuralNet and the last one as output.<br>
 *  The Linker created will link each Cluster to its successive.
 *  \param layers an integer vector contains the number of neurons of each Cluster
 *  \param clusterType the Typename of Cluster to create (it must be a Typename registered to Factory)
 *  \param linkerType the Typename of Linker to create (it must be a Typename registered to Factory)
 */
NNFW_API BaseNeuralNet* feedForwardNet( U_IntVec layers, const char* clusterType = "BiasedCluster", const char* linkerType = "MatrixLinker" );

/*! \brief SimpleTimer object
 *
 *  \par Motivation
 *  It is a simple timer for performance checks
 *
 *  \par Description
 *  SimpleTimer counts microseconds elapsed between tac() calls
 *
 *  \par Warnings
 *
 */
class SimpleTimer {
public:
	SimpleTimer() {
		struct timeval tv;
		gettimeofday( &tv, NULL );
		lastTime = tv.tv_sec*1000000 + tv.tv_usec;
	};
	int tac() {
		struct timeval tv;
		gettimeofday( &tv, NULL );
		int ret = (tv.tv_sec*1000000 + tv.tv_usec) - lastTime;
		lastTime = (tv.tv_sec*1000000 + tv.tv_usec);
		return ret;
	};
private:
	long int lastTime;
};

}

#endif
