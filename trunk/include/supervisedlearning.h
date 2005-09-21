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

#ifndef SupervisedLearning_H
#define SupervisedLearning_H

/*! \file
 *  \brief This file contains the declaration of abstract classes for supervised learning algorithm
 *
 *  Details...
 *
 *  \todo Utilities function to calculate:
 *        the average error over an ExampleSet given, 
 *        the max/min errors in an ExampleSet given,
 * 
 */

#include "types.h"

/*! \brief an Example to learn
 *
 *  \par Motivation
 *    This class register a pair (input, outputT) where input is a nnfwRealVec with inputs that if given at
 *    neural network, it have to procude value present in the outputT nnfwRealVec<br>
 *    This call register also the error made(commesso) by neural network ( outputNN - outputT )
 *  \par Description
 *    This class register inputs, desired outputs and errors by nnfwRealVec and don't do any kind of control
 *    about sizes and values contained.<br>
 *    The values of inputs and outputs can be setted by setInput() and setOutput() or directly in the constructor
 *    provided for that purpose. During initialization, all error are setted to zero.<br>
 *    The inputs, outputs and errors vector can be modified, also, by the reference returned by methods
 *    inputs(), outputs() and errors() respectively.
 */
class Example {
public:
    //! Construct an example with (input,output) passed
    Example( nnfwRealVec inputs, nnfwRealVec outputs );
    // Forse c'e' bisogno di un copyconstructor e di un costruttore di default !!
    //! Set the inputs
    void setInput( nnfwRealVec inputs );
    //! return the inputs;
    nnfwRealVec& inputs();
    //! Set the outputs
    void setOutput( nnfwRealVec outputs );
    //! return the outputs;
    nnfwRealVec& outputs();
    //! Set the vector of errors
    void setErrors( nnfwRealVec errors );
    //! return the errors
    nnfwRealVec& errors();
private:
    nnfwRealVec inputs;
    nnfwRealVec outputs;
    nnfwRealVec errors;
};

/*! \brief A vector of Example, in other way the examples set that neural network have to learn
 *
 *  This is simply a nnfwVector for two reasons: (i) it's indexed then the access is faster than other structures,
 *  (ii) there's no control of duplicate Examples then it's possible create a odd examples set
 */
typedef ExampleSet nnfwVector<Example>;

/*! \brief Abstract class for Supervised Learning Algorithm
 *
 * \par Motivation
 *   This class defines a common interface amongs classes that implement supervised learning algorithm
 *   such as Backpropagation, Simulated Annealing and so on.
 * \par Description
 *   Every methods of this class return a boolean value: true indicates "all it's ok", but false indicates
 *   that an error occurs. To know what kind of error occurs a message will print out by nnfwMessage() function.
 *
 */
class SupervisedLearning {
public:
    /*! \brief Set the neural network
     *
     * Details
     */
    virtual bool setNeuralNetwork( BaseNeuralNet* ) = 0;
    //! apply the learning algorithm. It modifies ExampleSet passed setting the errors vectors
    virtual bool learn( ExampleSet* ) = 0;
    //! test the neural network. It modifies ExampleSet passed setting the errors vectors
    virtual bool test( ExampleSet* ) = 0;    
};

/*! \brief Backpropagation learning algorithm
 *
 * Details
 */
class BackpropagationLearning : public SupervisedLearning {
public:
    /*! Construct a BackpropagationLearning without neural network associated;
     *   Call setNeuralNetwork() before using this class
     */
    BackpropagationLearning();

    /*! \brief Set the neural network
     *  Check if the neural network can be learned by backpropagation
     */
    bool setNeuralNetwork( BaseNeuralNet* );
    //! apply the backpropagation learning
    bool learn( ExampleSet* );
    //! test the neural network 
    bool test( ExampleSet* );
    //! Set the learning rate
    void setRate( nnfwReal );
    //! the learning rate
    nnfwReal getRate();
    //! Momentum
    void setMomentum( nnfwReal );
    //! the learning rate
    nnfwReal getMomentum();
protected:

    /* Backpropagation 
     *    - errore sui Cluster di output
     *    - propago l'errore sugli input & apprendo i Cluster
     *    - errore sui Linker 
     *    - propago l'errore & apprendo i pesi dei MatrixLinker ( delta = learnRate*errOut*peso*ActInput )
     */
    //! Backpropagate the errors from the Cluster's outputs to Cluster's inputs
    nnfwRealVec backpropCluster( SimpleCluster*, nnfwRealVec errOut );
    //! Modify the biases of Cluster's neurons
    void learnCluster( SimpleCluster*, nnfwRealVec errors );
    //! Backpropagate the errors ( inputs of the outgoing cluster of linker);
    nnfwRealVec backpropLinker( MatrixLinker*, nnfwRealVec errOut );
    //! Modify the weight of MatrixLinker
    void learnLinker( MatrixLinker*, nnfwRealVec errors );

    //! The neural network
    BaseNeuralNet* net;
    //! learning rate
    nnfwReal rate;
    //! momentum
    nnfwReal mom;
};

#endif
