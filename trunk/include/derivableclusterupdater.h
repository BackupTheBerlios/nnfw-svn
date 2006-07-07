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

#ifndef DerivableClusterUpdater_H
#define DerivableClusterUpdater_H

/*! \file
 *  \brief This file contains the declaration of the abstract DerivableClusterUpdater Class
 *
 *  Details...
 *
 */

#include "types.h"
#include "clusterupdater.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Derivable Cluster Updater Class
 *
 *  Details...
 */
class  DerivableClusterUpdater : public ClusterUpdater {
public:
    //! Constructor
    DerivableClusterUpdater() : ClusterUpdater() { /* Nothing to do */ };
    //! Destructor
    virtual ~DerivableClusterUpdater() { /* Nothing to do */ };
	
    /*! \brief Compute the derivate of the function represent
     *
     * Details...
     */
    virtual Real derivate( Real x, Real ) const { return x; };
	
};

}

#endif
