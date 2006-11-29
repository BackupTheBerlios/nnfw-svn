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

#ifndef SPARSEMATRIXLINKER_H
#define SPARSEMATRIXLINKER_H

/*! \file
 */

#include "types.h"
#include "linker.h"
#include "matrixlinker.h"

namespace nnfw {

/*! \brief SparseMatrixLinker Class extend MatrixLinker for allow non-full connection between Clusters
 *
 * Every connection is weighted, and the weight is memorized into a weight-matrix
 * Details ...
 */
class  SparseMatrixLinker : public MatrixLinker {
public:
    /*! \name Constructors */
    //@{

    /*!  Connect clusters with complete connections
     */
    SparseMatrixLinker( Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*!  Connect neurons of Clusters with a random connections with the passed probability.
     */
	SparseMatrixLinker( Real prob, Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*! Connect neurons of Clusters with a random connections with the passed probability.<br>
	 * With this contructor you must also specify whether the diagonal of the matrix is made of zeros
     * and whether the matrix is symmetrical
     * \warning You can use this constructor only with square matrices, otherwise it will generate a memory error!!!
	 */
	SparseMatrixLinker( Cluster* from, Cluster* to, Real prob, bool zeroDiagonal = false, bool symmetricMask = false, const char* name = "unnamed" );

    /*!  Construct by PropertySettings
     */
    SparseMatrixLinker( PropertySettings& prop );

    /*!  Destructor
     */
    virtual ~SparseMatrixLinker();

    //@}
    /*! \name Interface */
    //@{

    /*!  Set the weight of the connection specified
     */
    virtual void setWeight( u_int from, u_int to, Real weight );

    /*!  Randomize the weights of the SparseMatrixLinker
     */
    virtual void randomize( Real min, Real max );

    /*!  Connect two neurons
     */
    void connect( u_int from, u_int to );

    /*!  Connects randomly according to the given probability
     */
    void connectRandom( Real prob );

    /*!  Connect all couples of neurons
     */
    void connectAll();

    /*!  Disconnect the two neurons
     */
    void disconnect( u_int from, u_int to );

    /*!  Disconnects randomly according to the given probability
     */
    void disconnectRandom( Real prob );

    /*!  Disonnect all couples of neurons
     */
    void disconnectAll();

    /*!  Disconnect connections randomly according to the given probability
     *   \deprecated use disconnectRandom
     */
    void disconnect( Real prob ) {
        disconnectRandom( prob );
    };

    /*!  Get the mask
     */
    MatrixData<bool>& getMask() {
		return mask;
	};

    //@}

private:
    //! Mask Matrix
    MatrixData<bool> mask;
};

}

#endif
