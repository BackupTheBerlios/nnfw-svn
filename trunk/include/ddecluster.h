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
 *    Create a Cluster where the outputs dependes on previous value, or in other words on derivative of outputs
 *  \par Description
 *    This Cluster calculate the outputs accordlying with follow equation:<br>
 *    a0 + a1 * f(x) + a2 * x + a3 * y + a4 * y' + a5 * y'' + ... + aN * y^(n-3) <br>
 *    
 *  \par Warnings
 */
class  DDECluster : public Cluster {
public:

    /*! \brief Construct a DDECluster setting coefficients as specified
     *  Details...
     */
    DDECluster( const RealVec& coeff, u_int numNeurons, const char* name = "unnamed" );

    /*! \brief Destructor
     * Details..
     */
    virtual ~DDECluster();

    /*! \brief Set the Coeffiecients
     */
    void setCoeff( const RealVec& coef );

    /*! \brief Update the outputs of neurons
     * Details
     */
    void update();

    /*! \brief Randomize Nothing ;-)
     */
    void randomize( Real, Real ) { /* Nothing To Do */ };

private:
    //! Coefficient of equation
    RealVec coeff;
    //! Output Story
    RealVec* story;
    //! index of current position of story
    u_int idstory;
};

}

#endif
