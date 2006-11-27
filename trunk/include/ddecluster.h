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

#ifndef DDECLUSTER_H
#define DDECLUSTER_H

/*! \file
 */

#include "types.h"
#include "cluster.h"
#include "outputfunction.h"

namespace nnfw {

/*! \brief DDECluster Class. In this cluster the input/output relation is governed by a Discrete Differential Equation
 *  \par Motivation
 *    Create a Cluster where the outputs dependes on previous value, or in other words on derivative of outputs
 *  \par Description
 *    This Cluster calculate the outputs accordlying with follow equation:<br>
 *    y(t) <- a0 + a1*f(x) + a2*x(t) + a3*y(t-1) + a4*y'(t-1) + a5*y''(t-1) + a6*y'''(t-1) + ... <br>
 *    Note: L'attivazione delta alla 'Stefano' e' possibile ottenerla configurando i coefficienti nel seguente modo:<br>
 *    a0 <- 0.0 ; a1 <- delta ; a2 <- 0.0 ; a3 <- 1.0-delta <br>
 *    ottenendo: <br>
 *    y(t) <- (delta)*f(x) + (1.0-delta)*y(t-1)
 *  \par Warnings
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>Propertized</td> </tr>
 *   <tr><td>name</td> <td>string</td> <td>read/write</td> <td> name of the object </td> <td>Updatable</td> </tr>
 *   <tr><td>accumulate</td> <td>boolean</td> <td>read/write</td> <td> if inputs are accumulated </td> <td>Cluster</td> </tr>
 *   <tr><td>inputs</td> <td>RealVec</td> <td>read/write</td> <td> neuron's input </td> <td>Cluster</td> </tr>
 *   <tr><td>outfunction</td> <td>OutputFunction</td> <td>read/write</td> <td> neuron's output function </td> <td>Cluster</td> </tr>
 *   <tr><td>outputs</td> <td>RealVec</td> <td>read/write</td> <td> neuron's output </td> <td>Cluster</td> </tr>
 *   <tr><td>size</td> <td>unsigned int</td> <td>read-only</td> <td> number of neurons </td> <td>Cluster</td> </tr>
 *   <tr><td>coeff</td> <td>RealVec</td> <td>read/write</td> <td> equation's coefficients </td> <td>this</td> </tr>
 *   </table>
 */
class  DDECluster : public Cluster {
public:

    /*! Construct a DDECluster setting coefficients as specified
     */
    DDECluster( const RealVec& coeff, u_int numNeurons, const char* name = "unnamed" );

    /*! Construct by PropertySettings
     */
    DDECluster( PropertySettings& prop );

    /*! Destructor
     */
    virtual ~DDECluster();

    /*! Set the coeffiecients
     */
    void setCoeff( const RealVec& coef );

    /*! Return coeffiecients
     */
    const RealVec& getCoeff() {
        return coeff;
    };

    /*! Set Coefficeients (Variant version)
     */
    bool setCoeff( const Variant& );

    /*! Return coeffiecients (Variant version)
     */
    Variant getCoeffP();

    /*! Update the outputs of neurons
     */
    void update();

    /*! Randomize Nothing ;-)
     */
    void randomize( Real, Real ) { /* Nothing To Do */ };

private:
    //! Coefficient of equation
    RealVec coeff;
    //! Derivates of output
    VectorData< RealVec > ds;
    //! temporary data for calculation
    RealVec tmpdata;
    //! temporary data for calculation
    RealVec tmpdata2;

    //! Update the derivates of output
    void updateDs();
    //! Break the update calculates
    void breakUpdate();

    //! property definitions
    void propdefs();
};

}

#endif
