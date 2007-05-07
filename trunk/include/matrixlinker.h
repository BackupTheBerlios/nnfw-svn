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
 * From 0.7.0 release the update method will become pure-virtual.
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>name</td> <td>string</td> <td>read/write</td> <td> name of the object </td> <td>Updatable</td> </tr>
 *   <tr><td>from</td> <td>Cluster</td> <td>read-only</td> <td> incoming Cluster </td> <td>Linker</td> </tr>
 *   <tr><td>to</td> <td>Cluster</td> <td>read-only</td> <td> outgoing Cluster </td> <td>Linker</td> </tr>
 *   <tr><td>weights</td> <td>RealMat</td> <td>read/write</td> <td> connections' weights </td> <td>this</td> </tr>
 *   </table>
 *
 */
class NNFW_API MatrixLinker : public Linker {
public:
    /*! \name Constructors */
    //@{

    /*!  Connect clusters with a complete connections
     */
    MatrixLinker( Cluster* from, Cluster* to, const char* name = "unnamed" );

    /*!  Construct by PropertySettings
     */
    MatrixLinker( PropertySettings& prop );

    /*!  Destructor
     */
    virtual ~MatrixLinker();

    //@}
    /*! \name Interface */
    //@{

    /*!  Get the number of rows
     *  \deprecated
     */
    u_int getRows();

    /*!  Get the number of cols
     *  \deprecated
     */
    u_int getCols();

    /*!  Get the number of rows
     */
    u_int rows() {
        return nrows;
    };

    /*!  Get the number of cols
     */
    u_int cols() {
        return ncols;
    };

    /*!  Returns the total number of the links: rows*cols
     */
    u_int size() const;

    /*!  Randomize the weights of the MatrixLinker
     */
    virtual void randomize( Real min, Real max );

    /*!  Set the weight of the connection specified
     */
    virtual void setWeight( u_int from, u_int to, Real weight );

    /*!  Get the weight of the connection specified
     */
    virtual Real getWeight( u_int from, u_int to );

    /*!  Return the weight matrix
     */
	RealMat& matrix() {
		return w;
	};

    /*!  Return the weight matrix (Variant ver)
     */
    Variant matrixP() {
        return Variant( &w );
    };

    /*!  Set the whole weight matrix
     */
    void setMatrix( const RealMat& mat );

    /*!  Set the whole weight matrix (Variant ver)
     */
    bool setMatrix( const Variant& v );

    /*!  Update the Linker
     *
     *   \deprecated this method will become pure-virtual by 0.7.x releases
     */
    void update();

    //@}

private:
    //! Registers the dimensions of the matrix
    u_int nrows, ncols;
    //! Weight Matrix
    RealMat w;
};

}

#endif
