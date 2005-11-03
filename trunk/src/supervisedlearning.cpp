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

#include "supervisedlearning.h"
#include "derivableclusterupdater.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

Example::Example( nnfwRealVec inputs, nnfwRealVec outputs )
    : inputv(inputs), outputv(outputs), errorv() {
    errorv.resize( outputv.size() );
}

void Example::setInput( nnfwRealVec inputs ) {
    inputv = inputs;
}

nnfwRealVec& Example::input() {
    return inputv;
}

void Example::setOutput( nnfwRealVec outputs ) {
    outputv = outputs;
}

nnfwRealVec& Example::output() {
    return outputv;
}

void Example::setError( nnfwRealVec errors ) {
    if ( errors.size() != outputv.size() ) {
        nnfwMessage( NNFW_ERROR, "Error vector must have the same size of output vector" );
        return;
    }
    errorv = errors;
}

nnfwRealVec& Example::error() {
    return errorv;
}


ExampleSet::ExampleSet( u_int inputSize, u_int outputSize ) :
    examples() {
    inputDim = inputSize;
    outputDim = outputSize;
}

u_int ExampleSet::size() {
    return examples.size();
}

u_int ExampleSet::inputSize() {
    return inputDim;
}

u_int ExampleSet::outputSize() {
    return outputDim;
}

bool ExampleSet::add( Example& ex ) {
    // check the correctness of dimensions
    if ( ex.input().size() != inputDim ||
         ex.output().size() != outputDim ) {
        nnfwMessage( NNFW_ERROR, "Wrong dimesion of Example detected during add it in ExampleSet" );
        return false;
    }
    examples.push_back( ex );
    return true;
}

void ExampleSet::remove( u_int exId ) {
    //examples.erase( exId );
    return;
}

Example& ExampleSet::at( u_int id ) {
    if ( id < examples.size() ) {
        return examples.at( id );
    }
    nnfwMessage( NNFW_ERROR, "Out-boundary access to ExampleSet; last item will be returned" );
    return examples.at( examples.size() - 1 );
}


BackpropagationLearning::BackpropagationLearning() {
    rate = 0.02;
    mom = 0.02;
    net = 0;
}

bool BackpropagationLearning::setNeuralNetwork( BaseNeuralNet* n ) {
    //----- Da Implementare
    return false;
}

bool BackpropagationLearning::learn( ExampleSet* set ) {
    // -------- Check sulle dimensioni di inputs e outputs degli esempi contenuti in set
    // -------- se non sono corrette non si va avanti !!!
    u_int dimInputs = 0;
    const ClusterVec inC = net->inputClusters();
    for( u_int i=0; i<inC.size(); i++ ) {
        dimInputs += inC[i]->size();
    }
    u_int dimOutputs = 0;
    const ClusterVec outC = net->outputClusters();
    for( u_int i=0; i<outC.size(); i++ ) {
        dimOutputs += outC[i]->size();
    }
    if ( set->inputSize() != dimInputs ) {
        nnfwMessage( NNFW_ERROR, "Incorrect Dimension of Example; Learning Aborted" );
        return false;
    }
    if ( set->outputSize() != dimOutputs ) {
        nnfwMessage( NNFW_ERROR, "Incorrect Dimension of Example; Learning Aborted" );
        return false;
    }

    //---- Applico l'apprendimento su tutto il Learning Set
    for( u_int i=0; i<set->size(); i++ ) {
        learnOne( set->at(i) );
    }
    return true;
}

bool BackpropagationLearning::test( ExampleSet* set ) {
    u_int dimSet = set->size();
    for( u_int i=0; i<dimSet; i++ ) {
        errorOfNet( set->at(i) );
    }
    return true;
}

void BackpropagationLearning::learnOne( Example& ex ) {
    // ------- Preparo la mappa dei delta vectors
    const ClusterVec cls = net->clusters();
    for( u_int i=0; i<cls.size(); i++ ) {
        // ---- per ogni SimpleCluster associo un vettore della stessa dimensione inizializzato con zeri
        deltas.insert( std::make_pair( (SimpleCluster*)(cls[i]), nnfwRealVec( cls[i]->size(), 0.0 ) ) );
    }

    // ----- Calcolo i delta per i Cluster di output
    u_int skip = 0;
    errorOfNet( ex );
    nnfwRealVec errNet = ex.error();
    const ClusterVec outC = net->outputClusters();
    for( u_int i=0; i<outC.size(); i++ ) {
        deltaOutputCluster( (SimpleCluster*)(outC[i]), errNet, skip );
        skip += outC[i]->size();
        // ----- Inizializzo la Coda dei MatrixLinker
        const LinkerVec incom = net->linkers( outC[i], false );
        for( u_int i=0; i<incom.size(); i++ ) {
            mls.push( (MatrixLinker*)(incom[i]) );
        }
    }
    
    // ----- Ciclo di Backpropagation
    while( !mls.empty() ) {
        MatrixLinker* l = mls.front();
        backpropLinker( l );
        //Aggiungo nella coda tutti i linker entranti nel cluster l->from()
        const LinkerVec incom = net->linkers( l->getFrom(), false );
        for( u_int i=0; i<incom.size(); i++ ) {
            mls.push( (MatrixLinker*)(incom[i]) );
        }
        mls.pop();
    }

    // ----- Ciclo di Modifica dei pesi secondo la regola d'apprendimento
    const LinkerVec ls = net->linkers();
    for( u_int i=0; i<ls.size(); i++ ) {
        // ---- Modifico i pesi del MatrixLinker corrente
        learnLinker( (MatrixLinker*)(ls[i]) );
    }
    // cls e' gia' stato inizializzato all'inizio della funzione
    // const ClusterVec cls = net->clusters();
    for( u_int i=0; i<cls.size(); i++ ) {
        // ---- Modifico i bias del SimpleCluster corrente
        learnCluster( (SimpleCluster*)(cls[i]) );
    }
    return;
}

void BackpropagationLearning::setRate( nnfwReal r ) {
    rate = r;
}

nnfwReal BackpropagationLearning::getRate() {
    return rate;
}

void BackpropagationLearning::setMomentum( nnfwReal m ) {
    mom = m;
}

nnfwReal BackpropagationLearning::getMomentum() {
    return mom;
}

void BackpropagationLearning::deltaOutputCluster( SimpleCluster* c, nnfwRealVec errOut, u_int skipCount ) {
    nnfwRealVec& delta = deltas[c];
    u_int dimC = c->size();
    //----- ERROR: usa la derivata del ClusterUpdater settato sul neurone 0 per tutti i neuroni !!!!
    DerivableClusterUpdater* dup = (DerivableClusterUpdater*)(c->getUpdater( 0 ));
    for( u_int i=0; i<dimC; i++ ) {
        delta[i] = errOut[i+skipCount]*dup->derivate( c->getInput( i ) );
    }
    return;
}

void BackpropagationLearning::backpropLinker( MatrixLinker* l ) {
    // vettore dei delta del Cluster di uscita del linker 
    nnfwRealVec& deltaOut = deltas[ (SimpleCluster*)(l->getTo()) ];
    // vettore dei delta mappato al Cluster di ingresso del linker
    SimpleCluster* clsIn = (SimpleCluster*)(l->getFrom());
    nnfwRealVec& deltaIn  = deltas[ clsIn ];
    // Per il calcolo della derivata sul Cluster di ingresso del linker
    //----- ERROR: usa la derivata del ClusterUpdater settato sul neurone 0 per tutti i neuroni !!!!
    DerivableClusterUpdater* dup = (DerivableClusterUpdater*)(clsIn->getUpdater( 0 ));
    // Calcolo dei deltaIn backpropagando sul MatrixLinker corrente i deltaOut
    for ( u_int i=0; i<deltaOut.size(); i++ ) {
        for ( u_int j=0; j<deltaIn.size(); j++ ) {
            deltaIn[j] += dup->derivate( clsIn->getInput(j) )*l->getWeight(j,i)*deltaOut[i];
        }
    }
    return;
}

void BackpropagationLearning::learnCluster( SimpleCluster* ) {
    // ---- Da Implementare
    return;
}

void BackpropagationLearning::learnLinker( MatrixLinker* ) {
    // ---- Da Implementare
    return;
}

void BackpropagationLearning::errorOfNet( Example& ex ) { 
    nnfwRealVec& inputs = ex.input();
    nnfwRealVec& outputs = ex.output();
    nnfwRealVec& errors = ex.error();    
    // Set the inputs of the network
    const ClusterVec inC = net->inputClusters();
    u_int count = 0;
    for( u_int i=0; i<inC.size(); i++ ) {
        for( u_int j=0; j<inC[i]->size(); j++ ) {
            inC[i]->setInput( j, inputs[count] );
            count++;
        }
    }
    // Step of the net
    net->step();
    // Compute the errors 
    count = 0;
    const ClusterVec outC = net->outputClusters();
    for( u_int i=0; i<outC.size(); i++ ) {
        for( u_int j = 0; j<outC[i]->size(); j++ ) {
            errors[count] = outC[i]->getOutput( j ) - outputs[count];
        }
    }
    return;
}

}
