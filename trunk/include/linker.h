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

#ifndef LINKERS_H
#define LINKERS_H

/*! \file
 *  \brief This file contains the declaration of the linkers implemented
 *
 *  Details...
 *  \todo Quando viene creato un MatrixLinker che prende gli input da un solo Cluster, questo Cluster viene memorizzato
 *        in un array contenente un solo elemento, e durante l'update() viene eseguito un for su questo array!!!<br>
 *        Modificare l'implementazione in modo da evitare di eseguire il for in questa situazione
 */

#include "types.h"
#include "cluster.h"
#include "updatable.h"

/*! \brief Abstract Linker Class. This define the common interface among Linkers
 *
 * Details...
 */
class Linker : public Updatable
{
public:

    /*! \brief Return the Cluster From
     *
     * Details
     */
    virtual Cluster* getFrom() const = 0;

    /*! \brief Return the Cluster to
     *
     * Details
     */
    virtual Cluster* getTo() const = 0;

    /*! \brief Returns the 'size' of the linker
     *
     * The actual means of 'size' will be specified by sub-classes that implementes concrete linkers
     */
    virtual u_int size() = 0;

    /*! \brief Randomize the parameters of the Linker
     *
     * The parameters randomized by this method will be specified by sub-classes
     */
    virtual void randomize( nnfwReal min, nnfwReal max ) = 0;
};

#endif
