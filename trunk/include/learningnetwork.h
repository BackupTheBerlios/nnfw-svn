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

#ifndef LEARNINGNETWORK_H
#define LEARNINGNETWORK_H

#include "types.h"
#include "blockslearning.h"

/*! \file
 *  \brief This file contains the declaration learning networks
 *
 *  Details...
 *
 */


namespace nnfw {

/*! \brief The Base Learning Network Class
 *
 * Details
 */
class NNFW_API LearningNetwork {
public:
    /*! \brief Construct an empty LearningNetwork
     *  Details
     */
    LearningNetwork();

    /*! \brief Destructor
     *
     * Details
     */
    ~LearningNetwork();

    /*! \brief Add a BaseTeachBlock
     *  Details
     */
    void addTeachBlock( BaseTeachBlock* b );

    /*! \brief Returns the vector of BaseTeachBlocks contained
     * Details...
     */
    const TeachBlockVec& teachBlocks() const;

    /*! \brief Set the order in which learn() methods will be called
     * Details..
     */
    void setOrder( BaseTeachBlock* blocks[], u_int dim );
    //! \brief Set the order
    void setOrder( TeachBlockVec& );

    //! Return the BaseTeachBlock with name specified
    BaseTeachBlock* getByName( const char* name );

    /*! \brief Disable the calling of learn() method of BaseTeachBlock passed
     * Details...
     */
    void disable( BaseTeachBlock* );

    /*! \brief Enable the BaseTeachBlock
     * Details...
     */
    void enable( BaseTeachBlock* );

    /*! \brief Return true if the BaseTeachBlock object is enabled
     * Details...
     */
    bool isEnabled( BaseTeachBlock* );

    /*! \brief Learn
     *  Details
     */
    void learn() {
        for( u_int i=0; i<dimBls; i++ ) {
            if ( mask[i] ) {
                bls[i]->learn();
            }
        }
    };

    /*! \brief Return true if the BaseTeachBlock is in this net
     * Details
     */
    bool find( const BaseTeachBlock* ) const;

protected:
    //! TeachBlocks
    TeachBlockVec blocksv;

    //! Array of Updateables ordered as specified
    TeachBlockVec bls;
    BoolVec mask;
    unsigned int dimBls;
};

}

#endif

