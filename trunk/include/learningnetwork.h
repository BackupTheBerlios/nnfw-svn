/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
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
 */


namespace nnfw {

/*! \brief The Base Learning Network Class
 *
 * Details
 */
class NNFW_API LearningNetwork {
public:
	//@}
	/*! \name Constructors */
	//@{

    /*! Construct an empty LearningNetwork
     */
    LearningNetwork();

    /*! Destructor
     */
    ~LearningNetwork();

	//@}
	/*! \name Interface */
	//@{

    /*! Add a BaseTeachBlock
     */
    void addTeachBlock( BaseTeachBlock* b );

    /*! Returns the vector of BaseTeachBlocks contained
     */
    const TeachBlockVec& teachBlocks() const;

    /*! Set the order in which learn() methods will be called
     */
    void setOrder( BaseTeachBlock* blocks[], u_int dim );

    /*! Set the order */
    void setOrder( TeachBlockVec& );

    /*! Return the BaseTeachBlock with name specified */
    BaseTeachBlock* getByName( const char* name );

    /*! Disable the calling of learn() method of BaseTeachBlock passed
     */
    void disable( BaseTeachBlock* );

    /*! Enable the BaseTeachBlock
     */
    void enable( BaseTeachBlock* );

    /*! Return true if the BaseTeachBlock object is enabled
     */
    bool isEnabled( BaseTeachBlock* );

    /*! Learn
     */
    void learn() {
        for( u_int i=0; i<dimBls; i++ ) {
            if ( mask[i] ) {
                bls[i]->learn();
            }
        }
    };

    /*! Return true if the BaseTeachBlock is in this net
     */
    bool find( const BaseTeachBlock* ) const;

	//@}

protected:
    /*! TeachBlocks */
    TeachBlockVec blocksv;

    /*! Array of Updateables ordered as specified */
    TeachBlockVec bls;
    BoolVec mask;
    unsigned int dimBls;
};

}

#endif

