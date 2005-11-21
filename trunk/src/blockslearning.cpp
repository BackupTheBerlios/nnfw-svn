
#include "blockslearning.h"

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
    this->target = target;
}

RealVec SupervisedTeachBlock::getTarget() {
    return target;
}

void SupervisedTeachBlock::addError( RealVec error ) {
    this->error += error;
}

RealVec SupervisedTeachBlock::getError() {
    return error;
}

GradientSimpleCluster::GradientSimpleCluster( SimpleCluster* cl, BaseTeachBlock* pre, BaseTeachBlock* post)
    : SupervisedTeachBlock( pre, post ) {
    this->cl = cl;
}

void GradientSimpleCluster::learn() {
    // Da implementare
    if ( mode == targetMode ) {
        //--- Calcolo l'errore
        error = target - cl->getOutputs();
    }
    // --- 1. Propaga l'errore ai preBlocks
    // --- 2. Applica la regola del gradiente ai bias (pensati come pesi attaccati ad un neurone attivo a -1)

    error.clear();
}

GradientMatrixLinker::GradientMatrixLinker( MatrixLinker* ml, BaseTeachBlock* pre, BaseTeachBlock* post)
    : SupervisedTeachBlock( pre, post ) {
    this->ml = ml;
}

void GradientMatrixLinker::learn() {
    // Da implementare
    if ( mode == targetMode ) {
        //--- Calcolo l'errore
        error = target - ml->getTo()->getInputs();
    }
    // --- 1. Propaga l'errore ai preBlocks
    // --- 2. Applica la regola del gradiente ai bias (pensati come pesi attaccati ad un neurone attivo a -1)

    error.clear();
}

};
