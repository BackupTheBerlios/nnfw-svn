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

#ifndef DDECLUSTER_H
#define DDECLUSTER_H

#include "cluster.h"
#include "clusterupdater.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief DDECluster Class. In this cluster the input/output relation is governed by a Discrete Differential Equation
 *  \par Motivation
 *  \par Description
 *  \par Warnings
 */
class  DDECluster : public Cluster {
public:
    /*! \brief Construct a Cluster that contains numNeurons neuron
     *  Details...
     */
    DDECluster( u_int numNeurons, const char* name = "unnamed" );

    /*! \brief Destructor
     * Details..
     */
    virtual ~DDECluster();

    /*! \brief Update the outputs of neurons
     * Details
     */
    void update();

    /*! \brief Randomize Nothing ;-)
     */
    void randomize( Real, Real ) { /* Nothing To Do */ };

private:
    Real** coeff; // RealMatrix coeff;
};

}

#endif
