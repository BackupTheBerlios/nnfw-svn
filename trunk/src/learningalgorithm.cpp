/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
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

#include "neuralnet.h"
#include "learningalgorithm.h"

namespace nnfw {

LearningAlgorithm::LearningAlgorithm( BaseNeuralNet* net ) {
	this->netp = net;
}

LearningAlgorithm::~LearningAlgorithm() {
}

void LearningAlgorithm::setVectorModifier( Cluster* c, const char* propname, const VectorModifier& vm ) {
	if ( cmods.find(c) != cmods.end() ) {
		Modifier* m = cmods[c];
		cmods[c] = vm.clone();
		delete m;
	} else {
		cmods[c] = vm.clone();
	}
#ifdef NNFW_DEBUG
	Variant v = c->property( propname );
	if ( v.type() != Variant::t_realvec ) {
		nError() << "The property " << propname << " specified in setVectorModifier is not a RealVec or doesn't exist in Cluster passed; operation ignored";
	} else {
		((VectorModifier*)cmods[c])->setVector( *(v.getRealVec()) );
	}
#else
	((VectorModifier*)cmods[c])->setVector( *(c->property( propname ).getRealVec()) );
#endif
}

void LearningAlgorithm::setMatrixModifier( Cluster* c, const char* propname, const MatrixModifier& mm ) {
	if ( cmods.find(c) != cmods.end() ) {
		Modifier* m = cmods[c];
		cmods[c] = mm.clone();
		delete m;
	} else {
		cmods[c] = mm.clone();
	}
#ifdef NNFW_DEBUG
	Variant v = c->property( propname );
	if ( v.type() != Variant::t_realmat ) {
		nError() << "The property " << propname << " specified in setMatrixModifier is not a RealMat or doesn't exist in Cluster passed; operation ignored";
	} else {
		((MatrixModifier*)cmods[c])->setMatrix( *(v.getRealMat()) );
	}
#else
	((MatrixModifier*)cmods[c])->setMatrix( *(c->property( propname ).getRealMat()) );
#endif
}

void LearningAlgorithm::setVectorModifier( Linker* c, const char* propname, const VectorModifier& vm ) {
	if ( lmods.find(c) != lmods.end() ) {
		Modifier* m = lmods[c];
		lmods[c] = vm.clone();
		delete m;
	} else {
		lmods[c] = vm.clone();
	}
#ifdef NNFW_DEBUG
	Variant v = c->property( propname );
	if ( v.type() != Variant::t_realvec ) {
		nError() << "The property " << propname << " specified in setVectorModifier is not a RealVec or doesn't exist in Linker passed; operation ignored";
	} else {
		((VectorModifier*)lmods[c])->setVector( *(v.getRealVec()) );
	}
#else
	((VectorModifier*)lmods[c])->setVector( *(c->property( propname ).getRealVec()) );
#endif
}

void LearningAlgorithm::setMatrixModifier( Linker* c, const char* propname, const MatrixModifier& mm ) {
	if ( lmods.find(c) != lmods.end() ) {
		Modifier* m = lmods[c];
		lmods[c] = mm.clone();
		delete m;
	} else {
		lmods[c] = mm.clone();
	}
#ifdef NNFW_DEBUG
	Variant v = c->property( propname );
	if ( v.type() != Variant::t_realmat ) {
		nError() << "The property " << propname << " specified in setMatrixModifier is not a RealMat or doesn't exist in Linker passed; operation ignored";
	} else {
		((MatrixModifier*)lmods[c])->setMatrix( *(v.getRealMat()) );
	}
#else
	((MatrixModifier*)lmods[c])->setMatrix( *(c->property( propname ).getRealMat()) );
#endif
}

}
