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

#ifndef FAKECLUSTER_H
#define FAKECLUSTER_H

#include "cluster.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief FakeCluster Class. The FakeCluster is a Cluster without neurons, it's like an array ! :-)
 *
 *  \par Motivation
 *
 *  \par Description
 *  The FakeCluster implemented the Cluster Interface so it can be inserted inside a BaseNeuralNet,
 *  but it doesn't contains neurons, but it contains a simple vector with values and inputs and outputs
 *  are alias name for this vector of data. <br>
 *  It's very useful for inputs layers that don't needs true neurons but only a vector where registry
 *  the sensors values.
 *
 *  \par Warnings
 *  Due to fact that outputs() are simple an alias, or more precisly a VectorData view, of inputs(),
 *  this means that a call to resetInputs() method also resets the outputs!! In general, all methods
 *  that modify inputs also modify outputs and viceversa.
 *
 */
class  FakeCluster : public Cluster {
public:
    /*! \name Constructors */
    //@{

    /*! \brief Construct a FakeCluster
     *
     *  Details...
     */
    FakeCluster( u_int size, const char* name = "unnamed" );

    /*! \brief Construct by PropertySettings
     */
    FakeCluster( PropertySettings& );

    /*! \brief Destructor
     *
     * Details...
     */
    virtual ~FakeCluster();

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Update ;-) It's do nothing !!
     */
    void update();

    /*! \brief Randomize ?!?! What means 'randomize a FakeCluster' ???
     */
    void randomize( Real min, Real max );

    //@}

};

}

#endif
