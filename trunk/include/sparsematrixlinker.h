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

#include "linker.h"
#include "matrixlinker.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief SparseMatrixLinker Class extend MatrixLinker for allow non-full connection between Clusters
 * Every connection is weighted, and the weight is memorized into a weight-matrix
 * Details ...
 */
class  SparseMatrixLinker : public MatrixLinker {
public:
    /*! \name Constructors */
    //@{

    /*! \brief Connect clusters with complete connections
     * Details
     */
    SparseMatrixLinker( Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*! \brief Connect neurons of Clusters with a random connections with the passed probability.
	 * Details
     */
	SparseMatrixLinker( Real prob, Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*! \brief Connect neurons of Clusters with a random connections with the passed probability.
	 * With this contructor you must also specify whether the diagonal of the matrix is made of zeros and whether the matrix is symmetrical
     * ATTENTION: You can use this constructor only with square matrices, otherwise it will generate a memory error!!!
	 */
	SparseMatrixLinker( Cluster* from, Cluster* to, Real prob, bool zeroDiagonal = false, bool symmetricMask = false, const char* name = "unnamed" );

    /*! \brief Construct by PropertySettings
     */
    SparseMatrixLinker( PropertySettings& prop );

    /*! \brief Destructor
     * Details
     */
    virtual ~SparseMatrixLinker();

    //@}
    /*! \name Interface */
    //@{

    /*! \brief Set the weight of the connection specified
     * Details
     */
    virtual void setWeight( u_int from, u_int to, Real weight );

    /*! \brief Randomize the weights of the SparseMatrixLinker
     * Details
     */
    virtual void randomize( Real min, Real max );

    /*! \brief Connect two neurons
     */
    void connect( u_int from, u_int to );

    /*! \brief Connect all couples of neurons
     */
    void connectAll();

    /*! \brief Disconnect the two neurons
     */
    void disconnect( u_int from, u_int to );

    /*! \brief Disconnect connections randomly according to the given probability
     */
    void disconnect( Real prob );

    /*! \brief Get the mask
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
