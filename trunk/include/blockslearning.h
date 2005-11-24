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

#ifndef BlocksLearning_H
#define BlocksLearning_H

/*! \file
 *  \brief This file contains the declaration of building blocks for learning algorithms
 *
 *  Details...
 *
 */

#include "types.h"
#include "neuralnet.h"
#include "simplecluster.h"
#include "matrixlinker.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Abstract class representing the common interface for all building blocks for learning
 *  \par Motivation
 *   Il BaseTeachBlock si collega con altri BaseTeachBlock a creare una 'rete di apprendimento'<br>
 *  \par Description
 *   Ogni BaseTeachBlock ha un vettore con i blocchi precedenti (preVec) e uno con i blocchi
 *   successivi (postVec). Questi due vettori possono essere usati per trasferire dati necessari per
 *   l'apprendimento della rete (i.e.: la backpropagation trasferire l'errore all'indietro)<br>
 *   Non e' necessario specificare esplicitamente i vettori perVec e postVec, infatti nel costruttore
 *   viene specificato solo un blocco precedente e uno successivo. I vettori, infatti, vengono
 *   creati in modo automatico mentre vengono costruiti i BaseTeachBlock.<br>
 *   Chi implementa una sottoclasse, non deve preoccuparsi di come vengono creati i preVec e postVec ma soltanto
 *   trasferire i dati secondo l'algoritmo che si ha intezione di implementare.
 *  \par Warnings
 *     Warn
 */
class BaseTeachBlock {
public:
    /*! \brief Construct a BaseTeachBlock attached to specified BaseTeachBlocks
     */
    BaseTeachBlock( BaseTeachBlock* preBlock = 0, BaseTeachBlock* postBlock = 0 );
    //! Destructor
    virtual ~BaseTeachBlock() { /* Nothing to do */ };
    //! One step of the learning method implemented by subclasses
    virtual void learn() = 0;
protected:
    //! Pre BaseTeachBlock Vector
    Vector<BaseTeachBlock*> preVec;
    //! Post BaseTeachBlock Vector
    Vector<BaseTeachBlock*> postVec;
    //! Method for automatically creation of postVec
    void addPostBlock( BaseTeachBlock* postBlock );
    //! Method for automatically creation of preVec
    void addPreBlock( BaseTeachBlock* preBlock );
};

/*! \brief Abstract class representing a generic TeachBlock for SupervisedLearning
 *  \par Motivation
 *     Un SupervisedTeachBlock e' un blocco per l'apprendimento supervisionato.
 *     Si assume che l'entita' da apprendere abbia degli inputs e degli outputs, e che prima di chiamare
 *     il metodo learn() sia stato specificato gli outputs desiderati (target). Il target puo' essere specificato
 *     o direttamente dando i valori, oppure specificando l'errore attuale commesso dall'entita' sotto apprendimento.
 *     Questo blocco funziona in due modalita' a seconda se:<br>
 *     (i) viene chiamato il metodo setTarget() specificando l'output desiderato; in questa modalita' le sottoclasse
 *         che implementano il metodo learn() dovranno prima calcolare l'errore rispetto all'output corrente e poi applicare
 *         la regola d'apprendimento, e altre operazioni quali trasferire dati ai pre/postBlocks.<br>
 *     (ii) viene chiamato il metodo addError() specificando l'errore da aggiundere; in questa modalita' le sottoclasse
 *         che implementano il metodo learn() avranno gia' l'errore sul quale applicare la regola d'apprendimento.<br>
 *     IMPORTANTE per chi implementa sottoclassi: il metodo addError() cumula l'errore. E' dovere di chi implementa
 *     le sottoclassi chiamare al momento giusto il metodo clearError() per evitare accumuli di errori precedenti non piu'
 *     correlati con l'input/output corrente.<br>
 *  \par Description
 *     Desc
 *  \par Warnings
 *     Warn
 */
class SupervisedTeachBlock : public BaseTeachBlock {
public:
    /*! \brief Construct a SupervisedTeachBlock attached to specified BaseTeachBlock
     */
    SupervisedTeachBlock( BaseTeachBlock* preBlock = 0, BaseTeachBlock* postBlock = 0 );
    //! One step of the learning method implemented by subclasses
    virtual void learn() = 0;
    //! Set the desired output values for the current input values
    void setTarget( RealVec target );
    //! Return the target values specified
    RealVec getTarget();
    //! Add the error passed to the error already registered
    void addError( RealVec error );
    //! Return the last Errors values
    RealVec getError();
protected:
    //! the desired output values
    RealVec target;
    //! The error respect the current output values
    RealVec error;
    //! Modalita' di funzionamento targetMode punto (i) della descrizione, errorMode punto (ii)
    enum learnMode { targetMode = 1, errorMode = 2 };
    //! modalita' attuale
    learnMode mode;
};

/*! \brief Implements the gradient descent rule for a SimpleCluster
 *  \par Motivation
 *     Mot
 *  \par Description
 *     Desc
 *  \par Warnings
 *     Warn
 */
class GradientSimpleCluster : public SupervisedTeachBlock {
public:
    //! Construct a Gradient block attached to SimpleCluster passed
    GradientSimpleCluster( SimpleCluster* cl, BaseTeachBlock* preBlock = 0, BaseTeachBlock* postBlock = 0);
    /*! \brief Apply one step of the Gradient descent rule
     *  La regola del gradiente applicata ad un SimpleCluster consente di apprendere i valori di bias e di calcolare
     *  l'errore sull'input netto che viene passato al preBlock (se c'e' ne uno)
     */
    virtual void learn();
    //! Set learning rate
    void setRate( Real rate );
    //! Return the learning rate
    Real getRate();
    //! Set Momentum
    void setMomentum( Real m );
    //! Return Momentum
    Real getMomentum();
protected:
    //! SimpleCluster to learn
    SimpleCluster* cl;
    //! learn rate
    Real rate;
    //! Momentum
    Real momento;
    //! Vecchio errore (per il calcolo del momento)
    RealVec errorOld;
};

/*! \brief Implements the Gradient descent rule for a MatrixLinker
 *  \par Motivation
 *     Mot
 *  \par Description
 *     Desc
 *  \par Warnings
 *     Warn
 */
class GradientMatrixLinker : public SupervisedTeachBlock {
public:
    //! Construct a Gradient block attached to SimpleCluster passed
    GradientMatrixLinker( MatrixLinker* ml, BaseTeachBlock* preBlock = 0, BaseTeachBlock* postBlock = 0);
    /*! \brief Apply one step of the Gradient descent rule
     *  La regola del gradiente applicata ad un MatrixLinker consente di apprendere i valori dei pesi e di calcolare
     *  l'errore sugli output dei neuroni del Cluster in entrata; l'errore viene passato al preBlock (se c'e' ne uno)
     */
    virtual void learn();
    //! Set learning rate
    void setRate( Real rate );
    //! Return the learning rate
    Real getRate();
    //! Set Momentum
    void setMomentum( Real m );
    //! Return Momentum
    Real getMomentum();
protected:
    //! MatrixLinker to learn
    MatrixLinker* ml;
    //! learn rate
    Real rate;
    //! Momentum
    Real momento;
    //! Vecchio errore (per il calcolo del momento)
    RealVec errorOld;
};

};

#endif
