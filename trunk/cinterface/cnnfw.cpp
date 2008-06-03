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

#ifdef WIN32
	#define _CRT_SECURE_NO_DEPRECATE
	#ifdef NNFW_BUILDING_DLL
		#define C_NNFW_API __declspec(dllexport)
	#else
		#define C_NNFW_API __declspec(dllimport)
	#endif
#else
	#define C_NNFW_API
#endif

#include "nnfw.h"
#include "simplecluster.h"
#include "biasedcluster.h"
#include "fakecluster.h"
#include "dotlinker.h"
#include "normlinker.h"
#include "sparsematrixlinker.h"
#include "copylinker.h"
#include "outputfunction.h"
#include "liboutputfunctions.h"
#include "learningalgorithm.h"
#include "nnfwfactory.h"

using namespace nnfw;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NnfwOutputFunction {
	OutputFunction* func;
} NnfwOutputFunction;

typedef struct NnfwCluster {
	Cluster* cluster;
	NnfwOutputFunction* func;
} NnfwCluster;

/*! enum of possible modality of copying */
typedef enum { In2In, In2Out, Out2In, Out2Out } NnfwCopyLinkerModes;

typedef struct NnfwLinker {
	Linker* linker;
	NnfwCluster* from;
	NnfwCluster* to;
	//--- used only on CopyLinker
	NnfwCopyLinkerModes copymode;
} NnfwLinker;

typedef struct NnfwBaseNeuralNet {
	BaseNeuralNet* net;
} NnfwBaseNeuralNet;

typedef struct NnfwLearningAlgorithm {
	LearningAlgorithm* learn;
} NnfwLearningAlgorithm;

C_NNFW_API NnfwCluster* NnfwClusterCreate( const char* type, unsigned int numNeurons ) {
	NnfwCluster* cl = new NnfwCluster();
	PropertySettings param;
	param["numNeurons"] = numNeurons;
	cl->cluster = Factory::createCluster( type, param );
	cl->func = new NnfwOutputFunction();
	cl->func->func = cl->cluster->getFunction();
	return cl;
}

C_NNFW_API void NnfwClusterUpdate( NnfwCluster* cl ) {
	cl->cluster->update();
}

C_NNFW_API unsigned int NnfwClusterNumNeurons( NnfwCluster* cl ) {
	return cl->cluster->numNeurons();
}

C_NNFW_API void NnfwClusterSetAccumulate( NnfwCluster* cl, int accumulate ) {
	cl->cluster->setAccumulate( bool(accumulate) );
}

C_NNFW_API int NnfwClusterIsAccumulate( NnfwCluster* cl ) {
	return cl->cluster->isAccumulate();
}

C_NNFW_API void NnfwClusterRandomize( NnfwCluster* cl, Real min, Real max ) {
	cl->cluster->randomize( min, max );
}

C_NNFW_API void NnfwClusterSetInputs( NnfwCluster* cl, Real* ins ) {
	RealVec insr( ins, cl->cluster->numNeurons() );
	cl->cluster->setInputs( insr );
}

C_NNFW_API void NnfwClusterSetInput( NnfwCluster* cl, unsigned int neuron, Real value ) {
	cl->cluster->setInput( neuron, value );
}

C_NNFW_API Real* NnfwClusterInputs( NnfwCluster* cl ) {
	printf( "not yet implemented\n" );
	return 0;
}

C_NNFW_API Real NnfwClusterInput( NnfwCluster* cl, unsigned int neuron ) {
	return cl->cluster->getInput( neuron );
}

C_NNFW_API void NnfwClusterSetOutputs( NnfwCluster* cl, Real* outs ) {
	RealVec outsr( outs, cl->cluster->numNeurons() );
	cl->cluster->setOutputs( outsr );
}

C_NNFW_API void NnfwClusterSetOutput( NnfwCluster* cl, unsigned int neuron, Real value ) {
	cl->cluster->setOutput( neuron, value );
}

C_NNFW_API Real* NnfwClusterOutputs( NnfwCluster* cl ) {
	printf( "not yet implemented\n" );
	return 0;
}

C_NNFW_API Real NnfwClusterOutput( NnfwCluster* cl, unsigned int neuron ) {
	return cl->cluster->getOutput( neuron );
}

C_NNFW_API void NnfwClusterSetFunction( NnfwCluster* cl, NnfwOutputFunction* f ) {
	cl->cluster->setFunction( *(f->func) );
	cl->func->func = cl->cluster->getFunction();
}

C_NNFW_API NnfwOutputFunction* NnfwClusterFunction( NnfwCluster* cl ) {
	return cl->func;
}

C_NNFW_API NnfwCluster* NnfwClusterClone( NnfwCluster* cl ) {
	NnfwCluster* cln = new NnfwCluster();
	cln->cluster = cl->cluster->clone();
	cln->func = new NnfwOutputFunction();
	cln->func->func = cln->cluster->getFunction();
	return cln;
}

C_NNFW_API NnfwCluster* NnfwClusterCreateSimple( unsigned int numNeurons ) {
	NnfwCluster* cl = new NnfwCluster();
	cl->cluster = new SimpleCluster( numNeurons );
	cl->func = new NnfwOutputFunction();
	cl->func->func = cl->cluster->getFunction();
	return cl;
}

C_NNFW_API NnfwCluster* NnfwClusterCreateBiased( unsigned int numNeurons ) {
	NnfwCluster* cl = new NnfwCluster();
	cl->cluster = new BiasedCluster( numNeurons );
	cl->func = new NnfwOutputFunction();
	cl->func->func = cl->cluster->getFunction();
	return cl;
}

C_NNFW_API void NnfwBiasedClusterSetBiases( NnfwCluster* cl, Real* b ) {
	RealVec br( b, cl->cluster->numNeurons() );
	((BiasedCluster*)cl->cluster)->setBiases( br );
}

C_NNFW_API void NnfwBiasedClusterSetBias( NnfwCluster* cl, unsigned int neuron, Real value ) {
	((BiasedCluster*)cl->cluster)->setBias( neuron, value );
}

C_NNFW_API Real* NnfwBiasedClusterBiases( NnfwCluster* cl ) {
	printf( "not yet implemented\n" );
	return 0;
}

C_NNFW_API Real NnfwBiasedClusterBias( NnfwCluster* cl, unsigned int neuron ) {
	return ((BiasedCluster*)cl->cluster)->getBias( neuron );
}

C_NNFW_API NnfwCluster* NnfwClusterCreateFake( unsigned int numNeurons ) {
	NnfwCluster* cl = new NnfwCluster();
	cl->cluster = new FakeCluster( numNeurons );
	cl->func = new NnfwOutputFunction();
	cl->func->func = cl->cluster->getFunction();
	return cl;
}

C_NNFW_API NnfwLinker* NnfwLinkerCreate( const char* type, NnfwCluster* from, NnfwCluster* to ) {
	printf( "not implemented yet\n" );
	return 0;
	NnfwLinker* link = new NnfwLinker();
	PropertySettings param;
	param["from"] = from->cluster;
	param["to"] = to->cluster;
	link->linker = Factory::createLinker( type, param );
	link->from = from;
	link->to = to;
}

C_NNFW_API NnfwCluster* NnfwLinkerFrom( NnfwLinker* link ) {
	return link->from;
}

C_NNFW_API NnfwCluster* NnfwLinkerTo( NnfwLinker* link ) {
	return link->to;
}

C_NNFW_API void NnfwLinkerUpdate( NnfwLinker* link ) {
	link->linker->update();
}

C_NNFW_API unsigned int NnfwLinkerSize( NnfwLinker* link ) {
	return link->linker->size();
}

C_NNFW_API void NnfwLinkerRandomize( NnfwLinker* link, Real min, Real max ) {
	link->linker->randomize( min, max );
}

C_NNFW_API NnfwLinker* NnfwLinkerClone( NnfwLinker* link ) {
	NnfwLinker* linkn = new NnfwLinker();
	linkn->linker = link->linker->clone();
	linkn->from = link->from;
	linkn->to = link->to;
	return linkn;
}

C_NNFW_API NnfwLinker* NnfwLinkerCreateDot( NnfwCluster* from, NnfwCluster* to ) {
	NnfwLinker* link = new NnfwLinker();
	link->linker = new DotLinker( from->cluster, to->cluster );
	link->from = from;
	link->to = to;
	return link;
}

C_NNFW_API void NnfwDotLinkerSetWeights( NnfwLinker* link, Real* matrix ) {
	RealMat& mat = ((DotLinker*)link->linker)->matrix();
	int row = mat.rows();
	int col = mat.cols();
	for( int j=0; j<col; j++ ) {
		int toSkip = row*j;
		for( int i=0; i<row; i++ ) {
			mat[i][j] = matrix[i + toSkip];
		}
	}
}

C_NNFW_API void NnfwDotLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, Real w ) {
	((DotLinker*)link->linker)->setWeight( from, to, w );
}

C_NNFW_API Real* NnfwDotLinkerWeights( NnfwLinker* link ) {
	printf( "not yet implemented\n" );
	return 0;
}

C_NNFW_API Real NnfwDotLinkerWeight( NnfwLinker* link, unsigned int from, unsigned int to ) {
	return ((DotLinker*)link->linker)->getWeight( from, to );
}

C_NNFW_API int NnfwDotLinkerRows( NnfwLinker* link ) {
	return ((DotLinker*)link->linker)->rows();
}
C_NNFW_API int NnfwDotLinkerCols( NnfwLinker* link ) {
	return ((DotLinker*)link->linker)->cols();
}

C_NNFW_API NnfwLinker* NnfwLinkerCreateNorm( NnfwCluster* from, NnfwCluster* to ) {
	NnfwLinker* link = new NnfwLinker();
	link->linker = new NormLinker( from->cluster, to->cluster );
	link->from = from;
	link->to = to;
	return link;
}

C_NNFW_API void NnfwNormLinkerSetWeights( NnfwLinker* link, Real* matrix ) {
	RealMat& mat = ((NormLinker*)link->linker)->matrix();
	int row = mat.rows();
	int col = mat.cols();
	for( int j=0; j<col; j++ ) {
		int toSkip = row*j;
		for( int i=0; i<row; i++ ) {
			mat[i][j] = matrix[i + toSkip];
		}
	}
}

C_NNFW_API void NnfwNormLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, Real w ) {
	((NormLinker*)link->linker)->setWeight( from, to, w );
}

C_NNFW_API Real* NnfwNormLinkerWeights( NnfwLinker* link ) {
	printf( "not yet implemented\n" );
	return 0;
}

C_NNFW_API Real NnfwNormLinkerWeight( NnfwLinker* link, unsigned int from, unsigned int to ) {
	return ((NormLinker*)link->linker)->getWeight( from, to );
}

C_NNFW_API int NnfwNormLinkerRows( NnfwLinker* link ) {
	return ((NormLinker*)link->linker)->rows();
}

C_NNFW_API int NnfwNormLinkerCols( NnfwLinker* link ) {
	return ((NormLinker*)link->linker)->cols();
}

C_NNFW_API NnfwLinker* NnfwLinkerCreateSparse( NnfwCluster* from, NnfwCluster* to ) {
	NnfwLinker* link = new NnfwLinker();
	link->linker = new SparseMatrixLinker( from->cluster, to->cluster );
	link->from = from;
	link->to = to;
	return link;
}

C_NNFW_API void NnfwSparseLinkerSetWeights( NnfwLinker* link, Real* matrix ) {
	RealMat& mat = ((SparseMatrixLinker*)link->linker)->matrix();
	int row = mat.rows();
	int col = mat.cols();
	for( int j=0; j<col; j++ ) {
		int toSkip = row*j;
		for( int i=0; i<row; i++ ) {
			mat[i][j] = matrix[i + toSkip];
		}
	}
	mat.cover( ((SparseMatrixLinker*)link->linker)->mask() );
}

C_NNFW_API void NnfwSparseLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, Real w ) {
	((SparseMatrixLinker*)link->linker)->setWeight( from, to, w );
}

C_NNFW_API Real* NnfwSparseLinkerWeights( NnfwLinker* link ) {
	printf( "not yet implemented\n" );
	return 0;
}

C_NNFW_API Real NnfwSparseLinkerWeight( NnfwLinker* link, unsigned int from, unsigned int to ) {
	return ((SparseMatrixLinker*)link->linker)->getWeight( from, to );
}

C_NNFW_API int NnfwSparseLinkerRows( NnfwLinker* link ) {
	return ((SparseMatrixLinker*)link->linker)->rows();
}

C_NNFW_API int NnfwSparseLinkerCols( NnfwLinker* link ) {
	return ((SparseMatrixLinker*)link->linker)->cols();
}

C_NNFW_API void NnfwSparseLinkerConnect( NnfwLinker* link, unsigned int from, unsigned int to ) {
	((SparseMatrixLinker*)link->linker)->connect( from, to );
}

C_NNFW_API void NnfwSparseLinkerConnectAll( NnfwLinker* link ) {
	((SparseMatrixLinker*)link->linker)->connectAll();
}

C_NNFW_API void NnfwSparseLinkerConnectRandom( NnfwLinker* link, Real prob ) {
	((SparseMatrixLinker*)link->linker)->connectRandom( prob );
}

C_NNFW_API void NnfwSparseLinkerDisconnect( NnfwLinker* link, unsigned int from, unsigned int to ) {
	((SparseMatrixLinker*)link->linker)->disconnect( from, to );
}

C_NNFW_API void NnfwSparseLinkerDisconnectAll( NnfwLinker* link ) {
	((SparseMatrixLinker*)link->linker)->disconnectAll();
}

C_NNFW_API void NnfwSparseLinkerDisconnectRandom( NnfwLinker* link, Real prob ) {
	((SparseMatrixLinker*)link->linker)->disconnectRandom( prob );
}

C_NNFW_API int* NnfwSparseLinkerConnectionMask( NnfwLinker* link ) {
	printf( "not yet implemented\n" );
	return 0;
}

C_NNFW_API int NnfwSparseLinkerConnectionAt( NnfwLinker* link, unsigned int from, unsigned int to ) {
	return ((SparseMatrixLinker*)link->linker)->mask()[from][to];
}

C_NNFW_API NnfwLinker* NnfwLinkerCreateCopy( NnfwCluster* from, NnfwCluster* to ) {
	NnfwLinker* link = new NnfwLinker();
	link->linker = new CopyLinker( from->cluster, to->cluster, CopyLinker::In2In );
	link->from = from;
	link->to = to;
	link->copymode = In2In;
	return link;
}

C_NNFW_API void NnfwCopyLinkerSetMode( NnfwLinker* link, NnfwCopyLinkerModes mode ) {
	CopyLinker::CopyMode m = CopyLinker::In2In;
	switch(mode) {
	case In2In:
		m = CopyLinker::In2In;
		break;
	case In2Out:
		m = CopyLinker::In2Out;
		break;
	case Out2In:
		m = CopyLinker::Out2In;
		break;
	case Out2Out:
		m = CopyLinker::Out2Out;
		break;
	}
	((CopyLinker*)link->linker)->setMode( m );
	link->copymode = mode;
}

C_NNFW_API NnfwCopyLinkerModes NnfwCopyLinkerMode( NnfwLinker* link ) {
	return link->copymode;
}

#ifdef __cplusplus
};
#endif
