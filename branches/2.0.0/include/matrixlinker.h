/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>                *
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
 * This class rapresent connections between neuron's 'from' Cluster and neuron's 'to' Cluster with a matrix.
 * It allow sparse connections as full connections amogns neurons.
 * The connections has a weight and are stored in a matrix which the rows are the neuron's 'from' and
 * the columns the neuron's 'to'. <br>
 *
 * \par Description
 * Every connection is weighted, and the weight is memorized into a weight-matrix. <br>
 * The effective computation of inputs' 'to' is done in the subclasses (DotLinker, NormLinker, etc).
 * The disconnection / connection of neurons are implemented fixating/unfixating the weight to zero into
 * the underlying matrix using DoubleMatrix::steady and DoubleMatrix::unsteady methods
 *
 * \par Warning
 *
 * \todo Adding a boolean matrix for return the mask of connections of neurons
 */
class NNFW_API MatrixLinker : public Linker {
public:
	/*! \name Constructors */
	//@{
	/*!  Connect clusters with a complete connections
	 *   By default it create a fully-connected matrix, use one of the following methods for
	 *   choose a different way: connect, connectRandom, disconnect, disconnectRandom
	 */
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
	virtual double weight( unsigned int from, unsigned int to );
	/*!  Return the weight matrix */
	DoubleMatrix& matrix() {
		return w;
	};
	/*! const version of matrix() method */
	const DoubleMatrix& matrix() const {
		return w;
	};
	/*! Connect two neurons */
	void connect( unsigned int from, unsigned int to );
	/*! Connect neurons of Clusters with a random connections with the passed probability.<br>
	 *  And disconnect neurons with probability (1.0-prob).
	 *  \param prob is the probability to have a connection between two neurons
	 *  \param zeroDiagonal if it is true the matrix diagonal is always disconnected
	 *  \param symmetric if it is true the connections are symmetric
	 *  \warning The parameter symmetric requires a square matrix (i.e. the Cluster from and to must be
	 *     of the same size
	 */
	void connectRandom( double prob, bool zeroDiagonal = false, bool symmetric = false );
	/*! Connect all couples of neurons */
	void connectAll();
	/*! Disconnect the two neurons */
	void disconnect( unsigned int from, unsigned int to );
	/*! Disonnect all couples of neurons */
	void disconnectAll();
	//@}
private:
	/*! Registers the dimensions of the matrix */
	unsigned int nrows, ncols;
	/*! Weight Matrix */
	DoubleMatrix w;
};

}

#endif
