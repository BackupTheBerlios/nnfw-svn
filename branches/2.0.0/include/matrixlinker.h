/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                *
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

#ifndef MATRIXLINKER_H
#define MATRIXLINKER_H

/*! \file
 */

#include "types.h"
#include "linker.h"

namespace nnfw {

/*! \brief MatrixLinker Class define a full connection between two cluster.
 *
 * \par Motivation
 * This class rapresent a full-connection between neuron's 'from' Cluster and neuron's 'to' Cluster.
 * The connections has a weight and are stored in a matrix which the rows are the neuron's 'from' and
 * the columns the neuron's 'to'. <br>
 *
 * \par Description
 * Every connection is weighted, and the weight is memorized into a weight-matrix. <br>
 * The effective computation of inputs' 'to' is done in the subclasses (DotLinker, NormLinker, etc).
 *
 * \par Warning
 *
 */
class NNFW_API MatrixLinker : public Linker {
public:
    /*! \name Constructors */
    //@{

    /*!  Connect clusters with a complete connections */
    MatrixLinker( Cluster* from, Cluster* to, QString name = "unnamed" );
    /*!  Destructor */
    virtual ~MatrixLinker();

    //@}
    /*! \name Interface */
    //@{

    /*!  Get the number of rows */
    unsigned int rows() {
        return nrows;
    };

    /*!  Get the number of cols */
    unsigned int cols() {
        return ncols;
    };

    /*!  Returns the total number of the links: rows*cols */
    unsigned int size() const;

    /*!  Randomize the weights of the MatrixLinker */
    virtual void randomize( double min, double max );

    /*!  Set the weight of the connection specified */
    virtual void setWeight( unsigned int from, unsigned int to, double weight );

    /*!  Get the weight of the connection specified */
    virtual double getWeight( unsigned int from, unsigned int to );

    /*!  Return the weight matrix */
	RealMat& matrix() {
		return w;
	};

	/*! const version of matrix() method */
	const RealMat& matrix() const {
		return w;
	};

    /*!  Set the whole weight matrix */
    void setMatrix( const RealMat& mat );
	/*! Return its typename */
	virtual QString typeName() {
		return "MatrixLinker";
	};

    //@}

private:
    /*! Registers the dimensions of the matrix */
    unsigned int nrows, ncols;
    /*! Weight Matrix */
    RealMat w;
};

}

#endif
