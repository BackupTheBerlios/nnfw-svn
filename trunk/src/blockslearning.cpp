
#include "blockslearning.h"
#include "cluster.h"
#include "derivableclusterupdater.h"
#include "simplecluster.h"
#include "matrixlinker.h"

namespace nnfw {

BaseTeachBlock::BaseTeachBlock( BaseTeachBlock* preBlock, BaseTeachBlock* postBlock )
    : preVec(), postVec() {
    if (preBlock) {
        preVec.push_back( preBlock );
        preBlock->addPostBlock( this );
    }
    if (postBlock) {
        postVec.push_back( postBlock );
        postBlock->addPreBlock( this );
    }
}

void BaseTeachBlock::addPostBlock( BaseTeachBlock* postBlock ) {
    postVec.push_back( postBlock );
}

void BaseTeachBlock::addPreBlock( BaseTeachBlock* preBlock ) {
    preVec.push_back( preBlock );
}

SupervisedTeachBlock::SupervisedTeachBlock( BaseTeachBlock* preBlock, BaseTeachBlock* postBlock )
    : BaseTeachBlock( preBlock, postBlock ), target(), error() {
}

void SupervisedTeachBlock::setTarget( RealVec target ) {
    mode = targetMode;
    this->target = target;
}

RealVec SupervisedTeachBlock::getTarget() {
    return target;
}

void SupervisedTeachBlock::addError( RealVec error ) {
    mode = errorMode;
    this->error += error;
}

RealVec SupervisedTeachBlock::getError() {
    return error;
}

GradientSimpleCluster::GradientSimpleCluster( SimpleCluster* cl, BaseTeachBlock* pre, BaseTeachBlock* post)
    : SupervisedTeachBlock( pre, post ) {
    this->cl = cl;
    target.resize( cl->size() );
    error.resize( cl->size() );
    rate = 0.3;
}

void GradientSimpleCluster::learn() {
    // Da implementare
    if ( mode == SupervisedTeachBlock::targetMode ) {
        //--- Calcolo l'errore
        error = target - cl->getOutputs();
    }
    // --- calcolo del delta; error * derivataFunzioneAttivazione( input_netto )
    const Real* out = cl->getOutputs();
    for( u_int i=0; i<error.size(); i++ ) {
        const DerivableClusterUpdater* dup = dynamic_cast<const DerivableClusterUpdater*>( cl->getUpdater( i ) );
        if ( dup == 0 ) {
            error[i] *= out[i]*(1.0-out[i]);
        } else {
            error[i] *= dup->derivate( out[i] );
        }
    }
    // --- 1. Propaga l'errore ai preBlocks che ereditano da SupervisedTeachBlock
    for( u_int i=0; i<preVec.size(); i++ ) {
        SupervisedTeachBlock* sb = dynamic_cast<SupervisedTeachBlock*>(preVec[i]);
        if ( sb != 0 ) {
            sb->addError( error );
        }
    }
    // --- 2. Applica la regola del gradiente ai bias (pensati come pesi attaccati ad un neurone attivo a -1)
    for( u_int i=0; i<error.size(); i++ ) {
        cl->setBias( i, cl->getBias(i) - (rate*error[i]) );
    }

    error.assign( error.size(), 0.0f );
}

GradientMatrixLinker::GradientMatrixLinker( MatrixLinker* ml, BaseTeachBlock* pre, BaseTeachBlock* post)
    : SupervisedTeachBlock( pre, post ) {
    this->ml = ml;
    rate = 0.3;
    target.resize( ml->getCols() );
    error.resize( ml->getCols() );
}

void GradientMatrixLinker::learn() {
    // Da implementare
    if ( mode == targetMode ) {
        //--- Calcolo l'errore
        error = target - ml->getTo()->getInputs();
    }
    // --- I delta di eventuali neuroni mi arriva direttamente da GradientSimpleCluster
    
    // --- 1. Propaga l'errore ai preBlocks
    // (devo propagare solo l'errore moltiplicato per il peso relativo)
    u_int outS = ml->getCols();
    u_int inpS = ml->getRows();
    RealVec errIn( inpS );
    for( u_int c=0; c<outS; c++ ) {
        for( u_int r=0; r<inpS; r++ ) {
            errIn[r] += ml->getWeight( r, c )*error[c];
        }
    }
    for( u_int i=0; i<preVec.size(); i++ ) {
        SupervisedTeachBlock* sb = dynamic_cast<SupervisedTeachBlock*>(preVec[i]);
        if ( sb != 0 ) {
            sb->addError( errIn );
        }
    }

    // --- 2. Applica la regola del gradiente ai bias (pensati come pesi attaccati ad un neurone attivo a -1)
    const Real* outIn = ml->getFrom()->getOutputs();
    for( u_int c=0; c<outS; c++ ) {
        for( u_int r=0; r<inpS; r++ ) {
            ml->setWeight( r, c, ml->getWeight(r,c) + (rate*error[c]*outIn[r]) );
        }
    }

    error.assign( error.size(), 0.0f );
}

};
