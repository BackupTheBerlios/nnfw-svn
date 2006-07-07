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

#ifndef LibClusterUpdaters_H
#define LibClusterUpdaters_H

/*! \file
 *  \brief Library of Common Updaters
 *
 *  Details...
 *
 *  \todo CompositeUpdater, a subclass of ClusterUpdater that allow to combine two or more ClusterUpdaters;
 *        example: combining a SigmoidUpdater with LinearUpdater it's possibile realize an Updater that 
 *        first truncate the input in a range [mix,max] and the apply the Sigmoid: Sigmoid( Linear( x ) )
 *  \todo ComboLinearUpdater, a subclass of ClusterUpdater that constructs a linear combination of ClusterUpdaters:
 *        example: 4*Sigmoid(x)+2*Linear(x)
 */

#include "clusterupdater.h"
#include "derivableclusterupdater.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief DummyUpdater
 *
 * DummyUpdater copyies the inputs to the outputs
 */
class  DummyUpdater : public DerivableClusterUpdater {
public:
    //! Construct a dummy updater
    DummyUpdater( ) {
        // --- Nothing To Do
    };

    //! Destructor
    virtual ~DummyUpdater() { /* Nothing to do */ };

    //! Implement the dummy updating method
    void update( RealVec& inputs, RealVec& outputs );

    //! Single neuron update
    void update( Real input, Real &output );

    //! return always 1 (an explain of why will be coming soon)
    Real derivate( Real x, Real out ) const;
};

/*! \brief Sigmoid Updater
 *
 * Details..
 */
class  SigmoidUpdater : public DerivableClusterUpdater {
public:
    //! Construct a sigmoid updater with parameter l
    SigmoidUpdater( Real l = 1.0 ) {
        lambda = l;
    };

    //! Destructor
    virtual ~SigmoidUpdater() { /* Nothing to do */ };

    /*! \brief Implement the updating method
     *
     * Details...
     */
    void update( RealVec& inputs, RealVec& outputs );

    //! Single neuron update
    void update( Real input, Real &output );

    //! return the approximation commonly used in backpropagation learning: x(1-x)
    Real derivate( Real x, Real out )  const;

    //! lambda is the slope of the curve
    Real lambda;
};

/*! \brief Fake Sigmoid Updater !! Is a linear approximation of sigmoid function
 *
 * Details..
 */
class  FakeSigmoidUpdater : public DerivableClusterUpdater {
public:
    //! Construct a sigmoid updater with parameter l
    FakeSigmoidUpdater( Real l = 1.0 ) {
        lambda = l;
    };

    //! Destructor
    virtual ~FakeSigmoidUpdater() { /* Nothing to do */ };

    /*! \brief Implement the updating method
     *
     * Details...
     */
    void update( RealVec& inputs, RealVec& outputs );

    //! Single neuron update
    void update( Real input, Real &output );

    //! return the approximation commonly used in backpropagation learning: x(1-x)
    Real derivate( Real x, Real out ) const;

    //! lambda is the slope of the curve
    Real lambda;
};

/*! \brief ScaledSigmoid Updater
 *
 * lambda is the slope of the curve<br>
 * min is the y value when x -> -infinite <br>
 * max is the y value when x -> +infinite <br>
 * (max-min) is the y value when x == 0
 *
 */
class  ScaledSigmoidUpdater : public DerivableClusterUpdater {
public:
    //! Construct a scaled sigmoid updater with parameter l
    ScaledSigmoidUpdater( Real l = 1.0,
                          Real min = -1.0, Real max = +1.0 ) {
        lambda = l;
        this->min = min;
        this->max = max;
    };

    //! Destructor
    virtual ~ScaledSigmoidUpdater() { /* Nothing to do */ };

    /*! \brief Implement the updating method
     *
     * Details...
     */
    void update( RealVec& inputs, RealVec& outputs );

    //! Single neuron update
    void update( Real input, Real &output );

    //! return the approximation commonly used in backpropagation learning: x(1-x)
    Real derivate( Real x, Real out ) const;

    //! lambda is the slope of the curve
    Real lambda;
    //! min is the y value when x -> -infinite
    Real min;
    //! max is the y value when x -> +infinite
    Real max;
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
class  LinearUpdater : public DerivableClusterUpdater {
public:
    //! Construct a linear updater
    LinearUpdater( Real minX, Real maxX, Real minY = -1, Real maxY = +1 ) {
        this->minX = minX;
        this->maxX = maxX;
        this->minY = minY;
        this->maxY = maxY;
    };

    //! Destructor
    virtual ~LinearUpdater() { /* Nothing to do */ };

    /*! \brief Implement the updating method
     *
     * Details...
     */
    void update( RealVec& inputs, RealVec& outputs );

    //! Single neuron update
    void update( Real input, Real &output );

    //! return the m coefficient if x is in [minX, maxX] and x(1-x) otherwise
    Real derivate( Real x, Real out ) const;

    //! minX
    Real minX;
    //! maxX
    Real maxX;
    //! minY
    Real minY;
    //! maxY
    Real maxY;
};

/*! \brief Binary thresholded Updater
 *
 * Further Details coming soon ;-)
 */
class  BinaryUpdater : public DerivableClusterUpdater {
public:
    //! Construct a binary updater
    BinaryUpdater( Real threshold = 0.0f ) {
        this->threshold = threshold;
    };

    //! Destructor
    virtual ~BinaryUpdater() { /* Nothing to do */ };

    /*! \brief Implement the updating method
     *
     * Details...
     */
    void update( RealVec& inputs, RealVec& outputs );

    //! Single neuron update
    void update( Real input, Real &output );

    //! return the m coefficient if x is in [minX, maxX] and x(1-x) otherwise
    Real derivate( Real x, Real out ) const;

    //! Threshold
    Real threshold;
};

}

#endif


