/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                *
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

void Pattern::setInputsOf( Cluster* cl, const RealVec& ins ) {
	pinfo[cl].inputs = ins;
};

void Pattern::setOutputsOf( Cluster* cl, const RealVec& ous ) {
	pinfo[cl].outputs = ous;
};

void Pattern::setInputsOutputsOf( Cluster* cl, const RealVec& ins, const RealVec& ous ) {
	pinfo[cl].inputs = ins;
	pinfo[cl].outputs= ous;
};

const RealVec& Pattern::inputsOf( Cluster* cl ) const {
	if ( pinfo.count(cl) != 0 ) {
		return pinfo[cl].inputs;
	} else {
		return empty;
	}
};

const RealVec& Pattern::outputsOf( Cluster* cl ) const {
	if ( pinfo.count(cl) != 0 ) {
		return pinfo[cl].outputs;
	} else {
		return empty;
	}
};

Pattern::PatternInfo& Pattern::operator[]( Cluster* cl ) {
	return pinfo[cl];
};

LearningAlgorithm::LearningAlgorithm( BaseNeuralNet* net ) {
	this->netp = net;
}

LearningAlgorithm::~LearningAlgorithm() {
}

}
