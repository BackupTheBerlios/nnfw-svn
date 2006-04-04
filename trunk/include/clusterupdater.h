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

#ifndef ClusterUpdater_H
#define ClusterUpdater_H

/*! \file
 *  \brief This file contains the declaration of the abstract ClusterUpdater Class
 *
 *  Details...
 *
 *  \todo What are the needs of className() methods ???
 */

#include "types.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Cluster Updater Class
 *
 *  Details...
 */
class ClusterUpdater
{
public:
    //! Destructor
    virtual ~ClusterUpdater() { /* Nothing to do */ };
    /*! \brief Calculate the outputs of neurons by the inputs given
     *
     *  Details...
     */
    virtual void update( RealVec& inputs, RealVec& outputs );

    /*! \brief Calculate the outputs of a single neuron
     *
     *  Details...
     */
    virtual void update( Real input, Real &output );
};

}

#endif
