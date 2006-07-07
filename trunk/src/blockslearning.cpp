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

#include "blockslearning.h"
#include "cluster.h"
#include "derivableclusterupdater.h"
#include "biasedcluster.h"
#include "matrixlinker.h"

namespace nnfw {

BaseTeachBlock::BaseTeachBlock( BaseTeachBlock* preBlock, BaseTeachBlock* postBlock, const char* name )
    : preVec(), postVec() {
    if (preBlock) {
        preVec.push_back( preBlock );
        preBlock->addPostBlock( this );
    }
    if (postBlock) {
        postVec.push_back( postBlock );
        postBlock->addPreBlock( this );
    }
    u_int size = strlen(name);
    this->name = new char[size+1];
    strcpy( this->name, name );
}

void BaseTeachBlock::addPostBlock( BaseTeachBlock* postBlock ) {
    postVec.push_back( postBlock );
}

void BaseTeachBlock::addPreBlock( BaseTeachBlock* preBlock ) {
    preVec.push_back( preBlock );
}

SupervisedTeachBlock::SupervisedTeachBlock( BaseTeachBlock* preBlock, BaseTeachBlock* postBlock, bool modifiable, const char* name )
    : BaseTeachBlock( preBlock, postBlock, name ), target(), error(), modifiability(modifiable) {
}

void SupervisedTeachBlock::setTarget( const RealVec& target ) {
    mode = targetMode;
    this->target.assign( target );
}

RealVec& SupervisedTeachBlock::getTarget() {
    return target;
}

void SupervisedTeachBlock::addError( const RealVec& error ) {
    mode = errorMode;
    this->error += error;
}

RealVec& SupervisedTeachBlock::getError() {
    return error;
}

GradientBiasedCluster::GradientBiasedCluster( BiasedCluster* cl, BaseTeachBlock* pre, BaseTeachBlock* post, const char* name)
    : SupervisedTeachBlock( pre, post, name ) {
    this->cl = cl;
    target.resize( cl->size() );
    error.resize( cl->size() );
    oldDelta.resize( cl->size() );
    oldDelta.assign( oldDelta.size(), 0.0f );
    rate = 0.3f;
    momento = 0.0f;
}

void GradientBiasedCluster::learn() {
    if ( mode == SupervisedTeachBlock::targetMode ) {
        //--- Calcolo l'errore
        error.assign( target );
        error -= cl->outputs();
    }
    // --- calcolo del delta; error * derivataFunzioneAttivazione( input_netto )
    const RealVec& in = cl->oldInputs();
    for( u_int i=0; i<error.size(); i++ ) {
        const DerivableClusterUpdater* dup = dynamic_cast<const DerivableClusterUpdater*>( cl->getUpdater( i ) );
#ifdef NNFW_DEBUG
		if ( dup == 0 ) {
			nnfwMessage( NNFW_ERROR, "Error: you are trying to do the derivative of an underivable function!" );
            return;
        }
#endif
		error[i] *= dup->derivate( in[i] );
    }
    // --- 1. Propaga l'errore ai preBlocks che ereditano da SupervisedTeachBlock
    for( u_int i=0; i<preVec.size(); i++ ) {
        SupervisedTeachBlock* sb = dynamic_cast<SupervisedTeachBlock*>(preVec[i]);
        if ( sb != 0 ) {
            sb->addError( error ); //it sets the sb mode to errorMode
        }
    }
    // --- 2. Applica la regola del gradiente ai bias (pensati come pesi attaccati ad un neurone attivo a -1)
	if ( this->modifiability ) {
		RealVec delta( error.size() ), momTerm( error.size() );
		delta.assign_amulx( -rate, error );
		momTerm.assign_amulx( momento, oldDelta );
		delta += momTerm;
		cl->biases() += delta;
		/*
		for( u_int i=0; i<error.size(); i++ ) {
		    delta[i] = - (rate*error[i]) + momento*oldDelta[i];
		    cl->setBias( i, cl->getBias(i) + delta[i] );
		}
		*/
		oldDelta.assign( delta );
	}
    //error.assign( error.size(), 0.0f );
    error.setAll( 0.0f );
}

BiasedCluster* GradientBiasedCluster::getUpdatable() {
    return cl;
}

void GradientBiasedCluster::setRate( Real rate ) {
    this->rate = rate;
}

Real GradientBiasedCluster::getRate() {
    return rate;
}

void GradientBiasedCluster::setMomentum( Real m ) {
    this->momento = m;
}

Real GradientBiasedCluster::getMomentum() {
    return momento;
}

void GradientBiasedCluster::resetOldDeltas() {	
    oldDelta.setAll( 0.0f );
}

GradientMatrixLinker::GradientMatrixLinker( MatrixLinker* ml, BaseTeachBlock* pre, BaseTeachBlock* post, const char* name )
: SupervisedTeachBlock( pre, post, name ), oldDelta(ml->getRows(), ml->getCols()) {
    this->ml = ml;
    rate = 0.3f;
    momento = 0.0f;
    target.resize( ml->getCols() );
    error.resize( ml->getCols() );
	for(u_int r = 0; r < ml->getRows(); r++) {
		for(u_int c = 0; c < ml->getCols(); c++) {
			oldDelta.at( r, c ) = 0.0f;
		}
	}
}

void GradientMatrixLinker::learn() {
    // Da implementare
    if ( mode == targetMode ) {
        //--- Calcolo l'errore
        error.assign( target );
        error -= ml->getTo()->inputs();
    }
    // --- I delta di eventuali neuroni mi arriva direttamente da GradientBiasedCluster
    
    // --- 1. Propaga l'errore ai preBlocks
    // (devo propagare solo l'errore moltiplicato per il peso relativo)
    u_int outS = ml->getCols();
    u_int inpS = ml->getRows();
    RealVec errIn( inpS, 0.0 );
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

    // --- 2. Applica la regola del gradiente ai pesi
	if ( this->modifiability ) {
		const RealVec& outIn = ml->getFrom()->outputs();
		Matrix<Real> delta( inpS, outS );
		for( u_int c=0; c<outS; c++ ) {
		    for( u_int r=0; r<inpS; r++ ) {
				delta.at( r, c ) = ( rate * error[c] * outIn[r] ) + momento * oldDelta.at( r, c );
				ml->setWeight( r, c, ml->getWeight( r, c ) + delta.at( r, c ) );
				oldDelta.at( r, c ) = delta.at( r, c );
		    }
		}
	}
    //error.assign( error.size(), 0.0f );
    error.setAll( 0.0f );
}

MatrixLinker* GradientMatrixLinker::getUpdatable() {
    return ml;
}

void GradientMatrixLinker::setRate( Real rate ) {
    this->rate = rate;
}

Real GradientMatrixLinker::getRate() {
    return rate;
}

void GradientMatrixLinker::setMomentum( Real m ) {
    this->momento = m;
}

Real GradientMatrixLinker::getMomentum() {
    return momento;
}

void GradientMatrixLinker::resetOldDeltas() {
	for(u_int c = 0; c < ml->getCols(); c++)
		for(u_int r = 0; r < ml->getRows(); r++) { {
			oldDelta.at( r, c ) = 0.0f;
		}
	}
}

};
