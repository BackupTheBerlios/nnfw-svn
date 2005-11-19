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
#include "neuralnet.h"
#include "simplecluster.h"
#include "matrixlinker.h"
#include <queue>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief an Example to learn
 *
 *  \par Motivation
 *    This class register a pair (input, outputT) where input is a RealVec with inputs that if given at
 *    neural network, it have to procude value present in the outputT RealVec<br>
 *    This call register also the error made(commesso) by neural network ( outputNN - outputT )
 *  \par Description
 *    This class register inputs, desired outputs and errors by RealVec and don't do any kind of control
 *    about sizes and values contained.<br>
 *    The values of inputs and outputs can be setted by setInput() and setOutput() or directly in the constructor
 *    provided for that purpose. During initialization, all error are setted to zero.<br>
 *    The inputs, outputs and errors vector can be modified, also, by the reference returned by methods
 *    inputs(), outputs() and errors() respectively.
 *
 */
class Example {
public:
    //! Construct an example with (input,output) passed
    Example( RealVec inputs, RealVec outputs );
    // Forse c'e' bisogno di un copyconstructor e di un costruttore di default !!
    //! Set the input
    void setInput( RealVec inputs );
    //! return the inputs;
    RealVec& input();
    //! Set the output
    void setOutput( RealVec outputs );
    //! return the output;
    RealVec& output();
    //! Set the vector of error
    void setError( RealVec errors );
    //! return the errors
    RealVec& error();
private:
    RealVec inputv;
    RealVec outputv;
    RealVec errorv;
};

/*! \brief ExampleSet class, in other way the examples set that neural network have to learn
 *
 *
 * \todo forse andrebbe fatta una classe piu' furba, cioe' che vincola gli esempi contenuti ad essere 
 *     tutti della stessa dimensione di inputs e outputs !!
 */
class ExampleSet {
public:
    /*! \brief This create an empty ExampleSet that accept Example of dimension of input and output as specified
     *
     *  The only constraint of the ExampleSet is that all the Example contained have to be of the same dimension
     *  I don't think that its a great constraint... instead I think that its better, safer and faster !! :-)
     */
    ExampleSet( u_int inputSize, u_int outputSize );

    /*! \brief Return the number of elements contained
     *
     * Details
     */
    u_int size();

    /*! \brief Return the dimension of input's Example
     *
     * Details
     */
    u_int inputSize();

    /*! \brief Return the dimension of output's Example
     *
     * Details
     */
    u_int outputSize();

    /*! \brief Add an Example
     *
     * This method return false is you try to insert an Example of wrong dimensions<br>
     * The Examples inserted are indexed from 0 to N following the order which this function was called;
     * So, the first Example inserted is at 0, the second at 1 and so on.
     */
    bool add( Example& ex );

    /*! \brief Remove an Example
     *
     * Details...
     */
    void remove( u_int exId );

    /*! \brief Return a reference to the Example
     *
     * Details
     */
    Example& at( u_int id );

private:
    /*! This is simply a Vector for two reasons: 
     *    (i) it's indexed then the access is faster than other structures,
     *    (ii) there's no control of duplicate Examples then it's possible create a odd examples set
     */
    Vector<Example> examples;
    u_int inputDim;
    u_int outputDim;
};

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
 *
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
    void setRate( Real );
    //! the learning rate
    Real getRate();
    //! Momentum
    void setMomentum( Real );
    //! return the Momentum
    Real getMomentum();
protected:

    /* Backpropagation 
     *
     * Check sulla rete: SimpleCluster && DerivableClusterUpdater &&
                         MatrixLinker && NienteRicorsioni &&
     *                   Cluster Input e Output specificati 
     * Check su ExampleSet: Dimesione del vettore di input == dimensioni dei Cluster di input &&
     *                   Dimesione del vettore di output == dimensioni dei Cluster di output
     *
     *  Delta per gli output: (outTarget - outProdotto)*derivate( netInput )
     *  Delta per hidden/input: derivate( netInput )*sum( peso_ij*delta_i )
     *
     *  Regola Apprendimento: dw_ij = rate * ????
     *
     * Fase 1: error backpropagation: associo ad ogni Cluster il suo vettore dei delta
     *    !! Deve necessariamente partire dai Cluster di Output e 'scendere' attraverso la 
     *       struttura della rete per propagare i delta !!
     *    ---- Visita in Ampiezza al rovescio ----
     *     strutture dati: Coda di Linker
     *                     Mappa Cluster -> vettore dei delta
     *  - calcolo i delta sui neuroni di output
     *  - inizializzo la mappa dei Cluster: clusterOutput \-> deltas sul output dell'esempio     
     *  - inizializzo la coda con i Linker entranti sui Cluster di Output
     *  - fino a che la coda contiene elementi
     *        linker <- pop dalla coda
     *        deltaOut <- vettore dei delta del Cluster di uscita del linker 
     *        deltaIn <- vettore dei delta mappato al Cluster di ingresso del linker
     *                   (se ancora non esiste ne viene creato uno inizializzato con tutti zero)
     *        for i = 0 to deltaOut.size
     *          for j = 0 to deltaIn.size
     *             deltaIn[j] += derivate( cluster.input(j) )*linker.w(i,j)*deltaOut[i]
     *        Aggiungo nella coda tutti i linker entranti nel cluster linker.from()
     * ---------------------------------------
     *        
     *  
     * Fase 2: learning: sulla base dei vettori di delta applica la regola d'apprendimento
     *    !! Non c'e' bisogno che segua la struttura delle rete !!
     *  - foreach Cluster
     *      learn( cluster, delta vector associato )
     *  - foreach Linker
     *      learn( linker, delta vector di linker.to )
     * ---------------------------------------
     *     
     */
    /*! Calculate the delta vector of output Cluster passed
     * \param errOut is the difference between target outputs and produced outputs by the net
     * \param skipCount is the number of the element of errOut that have to be skipped
     */
    void deltaOutputCluster( SimpleCluster*, RealVec errOut, u_int skipCount );
    //! Backpropagate the delta along the MatrixLinker passed;
    void backpropLinker( MatrixLinker* );
    //! Modify the biases of Cluster's neurons
    void learnCluster( SimpleCluster* );
    //! Modify the weight of MatrixLinker
    void learnLinker( MatrixLinker* );

    //! Apply learning over one Example
    void learnOne( Example& );

    //! Set the error made by the net, given the example passed
    void errorOfNet( Example& );

    //! The neural network
    BaseNeuralNet* net;
    //! The queue of MatrixLinker
    typedef std::queue<MatrixLinker*> MLqueue;
    MLqueue mls;
    //! Map from SimpleCluster to delta vector
    typedef std::map<SimpleCluster*, RealVec> DeltaMap;
    DeltaMap deltas;

    //! learning rate
    Real rate;
    //! momentum
    Real mom;
};

}

#endif
