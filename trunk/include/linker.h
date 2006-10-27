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

#ifndef LINKERS_H
#define LINKERS_H

/*! \file
 *  \brief This file contains the declaration of the linkers implemented
 *
 *  Details...
 *
 */

#include "types.h"
#include "cluster.h"
#include "updatable.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Abstract Linker Class. This define the common interface among Linkers
 *
 * Details...
 */
class  Linker : public Updatable {
public:
    /*! \name Constructors */
    //@{

    //! Construct
    Linker( Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*! \brief Construct by PropertySettings
     */
    Linker( PropertySettings& );

    //@}
    /*! \name Deprecated Methods */
    //@{

    /*! \brief Return the Cluster From -- Deprecated ( use from() )
     *
     * Details
     */
    Cluster* getFrom() const {
        return fromc;
    };

    /*! \brief Return the Cluster to -- Deprecated ( use to() )
     *
     * Details
     */
    Cluster* getTo() const {
        return toc;
    };

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Return the Cluster From
     */
    Cluster* from() const {
        return fromc;
    };

    /*! \brief Return the Cluster to
     */
    Cluster* to() const {
        return toc;
    };

    /*! \brief Return Cluster from (Variant version)
     */
    Variant fromP() {
        return Variant( fromc );
    };

    /*! \brief Return Cluster to (Variant version)
     */
    Variant toP() {
        return Variant( toc );
    };

    /*! \brief Returns the 'size' of the linker
     *
     * The actual means of 'size' will be specified by sub-classes that implementes concrete linkers
     */
    virtual u_int size() = 0;

    /*! \brief Randomize the parameters of the Linker
     *
     * The parameters randomized by this method will be specified by sub-classes
     */
    virtual void randomize( Real min, Real max ) = 0;

    //@}

private:
    //! incoming Cluster
    Cluster* fromc;
    //! outgoing Cluster
    Cluster* toc;
};

}

#endif
