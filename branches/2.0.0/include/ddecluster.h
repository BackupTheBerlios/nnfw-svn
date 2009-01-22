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
 */
class NNFW_API DDECluster : public Cluster {
public:
	/*! \name Constructors */
	//@{

    /*! Construct a DDECluster setting coefficients as specified */
    DDECluster( const RealVec& coeff, unsigned int numNeurons, QString name = "unnamed" );

    /*! Destructor */
    virtual ~DDECluster();

	//@}
	/*! \name Interface */
	//@{

    /*! Set the coeffiecients */
    void setCoeff( const RealVec& coef );

    /*! Return coeffiecients */
    const RealVec& getCoeff() {
        return coeff;
    };

    /*! Update the outputs of neurons */
    void update();

    /*! Randomize Nothing ;-) */
    void randomize( double, double ) { /* Nothing To Do */ };

	/*! Clone this DDECluster */
	virtual DDECluster* clone() const;
	/*! Return its typename */
	virtual QString typeName() {
		return "DDECluster";
	};

	//@}

private:
    /*! Coefficient of equation */
    RealVec coeff;
    /*! Derivates of output */
    QVector< RealVec > ds;
    /*! temporary data for calculation */
    RealVec tmpdata;
    /*! temporary data for calculation */
    RealVec tmpdata2;

    /*! Update the derivates of output */
    void updateDs();
    /*! Break the update calculates */
    void breakUpdate();
};

}

#endif