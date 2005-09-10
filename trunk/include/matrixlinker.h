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

#ifndef MATRIXLINKER_H
#define MATRIXLINKER_H

#include "linker.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief MatrixLinker Class define a full connection from a group of clusters to one cluster.
 *
 * Every connection is weighted, and the weight is memorized into a weight-matrix
 * Details ...
 */
class MatrixLinker : public Linker
{
public:
    /*! \brief Connect clusters with a complete connections
     *
     * Details
     */
    MatrixLinker( Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*! \brief Destructor
     *
     * Details
     */
    virtual ~MatrixLinker();

    /*! \brief Get the number of rows
     *
     * Details...
     */
    u_int getRows();

    /*! \brief Get the number of cols
     *
     * Details...
     */
    u_int getCols();

    /*! \brief Returns the total number of the links: rows*cols
     *
     * Details...
     */
    u_int size();

    /*! \brief Randomize the weights of the MatrixLinker
     *
     * Details
     */
    void randomize( nnfwReal min, nnfwReal max );

    /*! \brief Set the weight of the connection specified
     *
     * Details
     */
    void setWeight( u_int from, u_int to, nnfwReal weight );

    /*! \brief Get the weight of the connection specified
     *
     * Details
     */
    nnfwReal getWeight( u_int from, u_int to );

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

    /*! \brief Update the Linker
     *
     * This update, for the moment, calculate a feedforward propagation.
     * In other words, multiply the vector of outputsFrom with matrix w and register the result in the vector inputsTo<br>
     *
     * Details...
     */
    void update();
private:
    //! incoming cluster output
    nnfwReal* outputsFrom;
    //! outgoing cluster inputs
    nnfwReal* inputsTo;
    //! Weight Matrix ( [column][row] )
    nnfwReal **w;
    //! Memory allocated for the weight matrix
    nnfwReal* memM;
    //! Registers the dimensions of the matrix
    u_int nrows, ncols;
    //! Registers the cluster from
    Cluster* from;
    //! Registers the cluster to
    Cluster* to;
};

}

#endif
