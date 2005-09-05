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

#ifndef LibClusterUpdaters_H
#define LibClusterUpdaters_H

/*! \file
 *  \brief Library of Common Updaters
 *
 *  Details...
 */

#include "clusterupdater.h"

/*! \brief DummyUpdater
 *
 * DummyUpdater copyies the inputs to the outputs
 */
class DummyUpdater : public ClusterUpdater
{
public:
    //! Construct a dummy updater
    DummyUpdater( ) {
        // --- Nothing To Do
    };

    //! Implement the dummy updating method
    void update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron );

    //! \brief Return the class name
    const char* className() const;
};

/*! \brief Sigmoid Updater
 *
 * Details..
 */
class SigmoidUpdater : public ClusterUpdater
{
public:
    //! Construct a sigmoid updater with parameter l
    SigmoidUpdater( nnfwReal l = 1.0 ) {
        lambda = l;
    };

    /* \brief Implement the updating method
     *
     * Details...
     */
    void update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron );

    //! \brief Return the class name
    const char* className() const;

    //! lambda is the slope of the curve
    nnfwReal lambda;
};

/*! \brief Fake Sigmoid Updater !! Is a linear approximation of sigmoid function
 *
 * Details..
 */
class FakeSigmoidUpdater : public ClusterUpdater
{
public:
    //! Construct a sigmoid updater with parameter l
    FakeSigmoidUpdater( nnfwReal l = 1.0 ) {
        lambda = l;
    };

    /* \brief Implement the updating method
     *
     * Details...
     */
    void update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron );

    //! \brief Return the class name
    const char* className() const;

    //! lambda is the slope of the curve
    nnfwReal lambda;
};

/*! \brief ScaledSigmoid Updater
 *
 * lambda is the slope of the curve<br>
 * min is the y value when x -> -infinite <br>
 * max is the y value when x -> +infinite <br>
 * (max-min) is the y value when x == 0
 *
 */
class ScaledSigmoidUpdater : public ClusterUpdater
{
public:
    //! Construct a scaled sigmoid updater with parameter l
    ScaledSigmoidUpdater( nnfwReal l = 1.0,
                          nnfwReal min = -1.0, nnfwReal max = +1.0 ) {
        lambda = l;
        this->min = min;
        this->max = max;
    };

    /* \brief Implement the updating method
     *
     * Details...
     */
    void update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron );

    //! \brief Return the class name
    const char* className() const;

    //! lambda is the slope of the curve
    nnfwReal lambda;
    //! min is the y value when x -> -infinite
    nnfwReal min;
    //! max is the y value when x -> +infinite
    nnfwReal max;
};

/*! \brief Linear Updater
 *
 * <pre>
 *  maxY            /---------------
 *                 /
 *                /
 *               /
 *  minY -------/
 *           minX   maxX
 * </pre>
 * Further Details coming soon ;-)
 */
class LinearUpdater : public ClusterUpdater
{
public:
    //! Construct a sigmoid updater with parameter l
    LinearUpdater( nnfwReal minX, nnfwReal maxX, nnfwReal minY = -1, nnfwReal maxY = +1 ) {
        this->minX = minX;
        this->maxX = maxX;
        this->minY = minY;
        this->maxY = maxY;
    };

    /* \brief Implement the updating method
     *
     * Details...
     */
    void update( nnfwReal* inputs, nnfwReal* outputs, u_int numNeuron );

    //! \brief Return the class name
    const char* className() const;

    //! minX
    nnfwReal minX;
    //! maxX
    nnfwReal maxX;
    //! minY
    nnfwReal minY;
    //! maxY
    nnfwReal maxY;
};

#endif


