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

#ifndef SIMPLECLUSTER_H
#define SIMPLECLUSTER_H

#include "types.h"
#include "cluster.h"
#include "outputfunction.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief SimpleCluster Class. In this cluster a neuron have an input, a output.
 *
 */
class  SimpleCluster : public Cluster {
public:
    /*! \name Constructors */
    //@{

    /*! \brief Construct a Cluster that contains numNeurons neuron
     *
     *  Details...
     */
    SimpleCluster( u_int numNeurons, const char* name = "unnamed" );

    /*! \brief Construct by a PropertySettings
     */
    SimpleCluster( PropertySettings& prop );

    /*! \brief Destructor
     *
     * Details..
     */
    virtual ~SimpleCluster();

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Update the outputs of neurons
     *
     * Details
     */
    void update();

    /*! \brief Randomize Nothing ;-)
     */
    void randomize( Real, Real ) { /* Nothing To Do */ };

    //@}
};

}

#endif
