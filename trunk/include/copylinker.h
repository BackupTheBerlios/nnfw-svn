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

#ifndef COPYLINKER_H
#define COPYLINKER_H

#include "linker.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief CopyLinker Class. This linker copies the outputs of a cluster to inputs of another cluster
 *
 * Details...
 */
class CopyLinker : public Linker
{
public:
    /*! \brief Constructor
     *
     * If the dimension of outputs and inputs between cluster aren't the same, then
     * only some data are copied <br>
     * Further Details coming soon ;-)
     */
    CopyLinker( Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*! \brief Destructor
     *
     * Details
     */
    virtual ~CopyLinker();
    
    /*! \brief Update the linker
     *
     * Details...
     */
    void update();

    /*! \brief Return the Cluster From
     *
     * Details
     */
    Cluster* getFrom() const;

    /*! \brief Return the Cluster to
     *
     * Details
     */
    Cluster* getTo() const;

    /*! \brief Returns the number of neurons connected
     *
     * Details
     */
    u_int size();

    /*! \brief Randomize nothing !!
     *
     * Details
     */
    void randomize( nnfwReal , nnfwReal ) {
        // --- Do Nothing
    };
private:
    nnfwReal* outputsFrom;
    nnfwReal* inputsTo;
    u_int dimData;
    Cluster* from;
    Cluster* to;
};

}

#endif
