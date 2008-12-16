/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                     *
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
class NNFW_API SparseMatrixLinker : public MatrixLinker {
public:
    /*! \name Constructors */
    //@{

    /*! Connect clusters with complete connections */
    SparseMatrixLinker( Cluster* from, Cluster* to, QString name = "unnamed" );
    /*! Connect neurons of Clusters with a random connections with the passed probability.<br>
	 * With this contructor you must also specify whether the diagonal of the matrix is made of zeros
     * and whether the matrix is symmetrical
     * \warning You can use this constructor only with square matrices, otherwise it will generate a memory error!!!
	 */
	SparseMatrixLinker( Cluster* from, Cluster* to, double prob, bool zeroDiagonal = false, bool symmetricMask = false, QString name = "unnamed" );

    /*! Destructor */
    virtual ~SparseMatrixLinker();

    //@}
    /*! \name Interface */
    //@{

    /*! Set the weight of the connection specified */
    virtual void setWeight( unsigned int from, unsigned int to, double weight );

    /*! Randomize the weights of the SparseMatrixLinker */
    virtual void randomize( double min, double max );

    /*! Performs the dot-product calculation where the non-connection are considered as zero */
    void update();

    /*! Connect two neurons */
    void connect( unsigned int from, unsigned int to );

    /*! Connects randomly according to the given probability */
    void connectRandom( double prob );

    /*! Connect all couples of neurons */
    void connectAll();

    /*! Disconnect the two neurons */
    void disconnect( unsigned int from, unsigned int to );

    /*! Disconnects randomly according to the given probability */
    void disconnectRandom( double prob );

    /*! Disonnect all couples of neurons */
    void disconnectAll();

    /*! Get the mask 
	 *  \deprecated use mask() instead
	 */
    MatrixData<bool>& getMask() {
		return maskm;
	};

    /*! Return the mask */
    MatrixData<bool>& mask() {
		return maskm;
	};

    /*!  Set the whole mask matrix */
    void setMask( const MatrixData<bool>& mask );

	/*! Clone this SparseMatrixLinker */
	virtual SparseMatrixLinker* clone() const;

    //@}

private:
    /*! Mask Matrix */
    MatrixData<bool> maskm;
};

}

#endif
