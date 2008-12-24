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
#include "libradialfunctions.h"
#include "learningalgorithm.h"
#include "backpropagationalgo.h"
#include <cstdarg>
#include <QMap>

namespace nnfw {
	//--- for accessing RealVec data from C interface implementation
	NNFW_INTERNAL double* getRawData( RealVec& vec ) {
		return vec.rawdata();
	}
	//--- for accessing from C interface implementation
	NNFW_INTERNAL double* getRawData( RealMat& mat ) {
		return getRawData( mat.rawdata() );
	}
}

using namespace nnfw;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NnfwOutputFunction {
	OutputFunction* func;
} NnfwOutputFunction;

typedef struct NnfwCluster {
	Updatable* myup;
	Cluster* cluster;
	NnfwOutputFunction* func;
} NnfwCluster;

/*! enum of possible modality of copying */
typedef enum { In2In, In2Out, Out2In, Out2Out } NnfwCopyLinkerModes;
/*! enum of possible types inside NnfwIterator */
typedef enum { ClusterType, LinkerType } NnfwIteratorElementTypes;

typedef struct NnfwLinker {
	Updatable* myup;
	Linker* linker;
	NnfwCluster* from;
	NnfwCluster* to;
	//--- used only on CopyLinker
	NnfwCopyLinkerModes copymode;
} NnfwLinker;

//--- this need to be converted in object design
typedef struct NnfwIterator {
	const ClusterList* clvec;
	const LinkerList* linkvec;
	const UpdatableList* upvec;
	//--- 0 -> ClusterVec
	//--- 1 -> LinkerVec
	//--- 2 -> UpdatableVec
	int type;
} NnfwIterator;

typedef struct NnfwBaseNeuralNet {
	BaseNeuralNet* net;
} NnfwBaseNeuralNet;

typedef struct NnfwLearningAlgorithm {
	LearningAlgorithm* learn;
} NnfwLearningAlgorithm;

typedef struct NnfwPatternSet {
	PatternSet* pset;
} NnfwPatternSet;

#ifdef __cplusplus
};
#endif

//--- maps used for avoid redunant allocation memory when using NnfwIterator
QMap<Cluster*, NnfwCluster*> clmap;
QMap<Linker*, NnfwLinker*> linkmap;

#ifdef __cplusplus
extern "C" {
#endif

/* *** TODO: re-implement when the Factory will come back
C_NNFW_API NnfwCluster* NnfwClusterCreate( const char* type, unsigned int numNeurons ) {
	NnfwCluster* cl = new NnfwCluster();
	cl->myup = cl->cluster;
	cl->func = new NnfwOutputFunction();
	cl->func->func = cl->cluster->getFunction();
	return cl;
}
*** */

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

C_NNFW_API void NnfwClusterRandomize( NnfwCluster* cl, double min, double max ) {
	cl->cluster->randomize( min, max );
}

C_NNFW_API void NnfwClusterSetInputs( NnfwCluster* cl, double* ins ) {
	RealVec insr( ins, cl->cluster->numNeurons() );
	cl->cluster->setInputs( insr );
}

C_NNFW_API void NnfwClusterSetInput( NnfwCluster* cl, unsigned int neuron, double value ) {
	cl->cluster->setInput( neuron, value );
}

C_NNFW_API double* NnfwClusterInputs( NnfwCluster* cl ) {
	return getRawData( cl->cluster->inputs() );
}

C_NNFW_API double NnfwClusterInput( NnfwCluster* cl, unsigned int neuron ) {
	return cl->cluster->getInput( neuron );
}

C_NNFW_API void NnfwClusterSetOutputs( NnfwCluster* cl, double* outs ) {
	RealVec outsr( outs, cl->cluster->numNeurons() );
	cl->cluster->setOutputs( outsr );
}

C_NNFW_API void NnfwClusterSetOutput( NnfwCluster* cl, unsigned int neuron, double value ) {
	cl->cluster->setOutput( neuron, value );
}

C_NNFW_API double* NnfwClusterOutputs( NnfwCluster* cl ) {
	return getRawData( cl->cluster->outputs() );
}

C_NNFW_API double NnfwClusterOutput( NnfwCluster* cl, unsigned int neuron ) {
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
	cln->myup = cln->cluster;
	cln->func = new NnfwOutputFunction();
	cln->func->func = cln->cluster->getFunction();
	return cln;
}

C_NNFW_API NnfwCluster* NnfwClusterCreateSimple( unsigned int numNeurons ) {
	NnfwCluster* cl = new NnfwCluster();
	cl->cluster = new SimpleCluster( numNeurons );
	cl->myup = cl->cluster;
	cl->func = new NnfwOutputFunction();
	cl->func->func = cl->cluster->getFunction();
	return cl;
}

C_NNFW_API NnfwCluster* NnfwClusterCreateBiased( unsigned int numNeurons ) {
	NnfwCluster* cl = new NnfwCluster();
	cl->cluster = new BiasedCluster( numNeurons );
	cl->myup = cl->cluster;
	cl->func = new NnfwOutputFunction();
	cl->func->func = cl->cluster->getFunction();
	return cl;
}

C_NNFW_API void NnfwBiasedClusterSetBiases( NnfwCluster* cl, double* b ) {
	RealVec br( b, cl->cluster->numNeurons() );
	((BiasedCluster*)cl->cluster)->setBiases( br );
}

C_NNFW_API void NnfwBiasedClusterSetBias( NnfwCluster* cl, unsigned int neuron, double value ) {
	((BiasedCluster*)cl->cluster)->setBias( neuron, value );
}

C_NNFW_API double* NnfwBiasedClusterBiases( NnfwCluster* cl ) {
	return getRawData( ((BiasedCluster*)cl->cluster)->biases() );
}

C_NNFW_API double NnfwBiasedClusterBias( NnfwCluster* cl, unsigned int neuron ) {
	return ((BiasedCluster*)cl->cluster)->getBias( neuron );
}

C_NNFW_API NnfwCluster* NnfwClusterCreateFake( unsigned int numNeurons ) {
	NnfwCluster* cl = new NnfwCluster();
	cl->cluster = new FakeCluster( numNeurons );
	cl->myup = cl->cluster;
	cl->func = new NnfwOutputFunction();
	cl->func->func = cl->cluster->getFunction();
	return cl;
}

/* ** TODO: re-implement when the Factory will come back
C_NNFW_API NnfwLinker* NnfwLinkerCreate( const char* type, NnfwCluster* from, NnfwCluster* to ) {
	NnfwLinker* link = new NnfwLinker();
	PropertySettings param;
	param["from"] = from->cluster;
	param["to"] = to->cluster;
	link->linker = Factory::createLinker( type, param );
	link->myup = link->linker;
	link->from = from;
	link->to = to;
	return link;
}
** */

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

C_NNFW_API void NnfwLinkerRandomize( NnfwLinker* link, double min, double max ) {
	link->linker->randomize( min, max );
}

C_NNFW_API NnfwLinker* NnfwLinkerClone( NnfwLinker* link ) {
	NnfwLinker* linkn = new NnfwLinker();
	linkn->linker = link->linker->clone();
	linkn->myup = linkn->linker;
	linkn->from = link->from;
	linkn->to = link->to;
	return linkn;
}

C_NNFW_API NnfwLinker* NnfwLinkerCreateDot( NnfwCluster* from, NnfwCluster* to ) {
	NnfwLinker* link = new NnfwLinker();
	link->linker = new DotLinker( from->cluster, to->cluster );
	link->myup = link->linker;
	link->from = from;
	link->to = to;
	return link;
}

C_NNFW_API void NnfwDotLinkerSetWeights( NnfwLinker* link, double* matrix ) {
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

C_NNFW_API void NnfwDotLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, double w ) {
	((DotLinker*)link->linker)->setWeight( from, to, w );
}

C_NNFW_API double* NnfwDotLinkerWeights( NnfwLinker* link ) {
	return getRawData( ((DotLinker*)link->linker)->matrix() );
}

C_NNFW_API double NnfwDotLinkerWeight( NnfwLinker* link, unsigned int from, unsigned int to ) {
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
	link->myup = link->linker;
	link->from = from;
	link->to = to;
	return link;
}

C_NNFW_API void NnfwNormLinkerSetWeights( NnfwLinker* link, double* matrix ) {
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

C_NNFW_API void NnfwNormLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, double w ) {
	((NormLinker*)link->linker)->setWeight( from, to, w );
}

C_NNFW_API double* NnfwNormLinkerWeights( NnfwLinker* link ) {
	return getRawData( ((NormLinker*)link->linker)->matrix() );
}

C_NNFW_API double NnfwNormLinkerWeight( NnfwLinker* link, unsigned int from, unsigned int to ) {
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
	link->myup = link->linker;
	link->from = from;
	link->to = to;
	return link;
}

C_NNFW_API void NnfwSparseLinkerSetWeights( NnfwLinker* link, double* matrix ) {
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

C_NNFW_API void NnfwSparseLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, double w ) {
	((SparseMatrixLinker*)link->linker)->setWeight( from, to, w );
}

C_NNFW_API double* NnfwSparseLinkerWeights( NnfwLinker* link ) {
	return getRawData( ((SparseMatrixLinker*)link->linker)->matrix() );
}

C_NNFW_API double NnfwSparseLinkerWeight( NnfwLinker* link, unsigned int from, unsigned int to ) {
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

C_NNFW_API void NnfwSparseLinkerConnectRandom( NnfwLinker* link, double prob ) {
	((SparseMatrixLinker*)link->linker)->connectRandom( prob );
}

C_NNFW_API void NnfwSparseLinkerDisconnect( NnfwLinker* link, unsigned int from, unsigned int to ) {
	((SparseMatrixLinker*)link->linker)->disconnect( from, to );
}

C_NNFW_API void NnfwSparseLinkerDisconnectAll( NnfwLinker* link ) {
	((SparseMatrixLinker*)link->linker)->disconnectAll();
}

C_NNFW_API void NnfwSparseLinkerDisconnectRandom( NnfwLinker* link, double prob ) {
	((SparseMatrixLinker*)link->linker)->disconnectRandom( prob );
}

C_NNFW_API int NnfwSparseLinkerConnectionAt( NnfwLinker* link, unsigned int from, unsigned int to ) {
	return ((SparseMatrixLinker*)link->linker)->mask()[from][to];
}

C_NNFW_API NnfwLinker* NnfwLinkerCreateCopy( NnfwCluster* from, NnfwCluster* to ) {
	NnfwLinker* link = new NnfwLinker();
	link->linker = new CopyLinker( from->cluster, to->cluster, CopyLinker::In2In );
	link->myup = link->linker;
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

/* ** TODO: re-implement when the Factory will come back
C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreate( const char* type ) {
	NnfwOutputFunction* fun = new NnfwOutputFunction();
	PropertySettings param;
	fun->func = Factory::createOutputFunction( type, param );
	return fun;
}
** */

C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateIdentity() {
	NnfwOutputFunction* fun = new NnfwOutputFunction();
	fun->func = new IdentityFunction();
	return fun;	
}

C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateLinear( double m, double b ) {
	NnfwOutputFunction* fun = new NnfwOutputFunction();
	fun->func = new LinearFunction( m, b );
	return fun;	
}

C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateRamp( double minX, double maxX, double minY, double maxY ) {
	NnfwOutputFunction* fun = new NnfwOutputFunction();
	fun->func = new RampFunction( minX, maxX, minY, maxY );
	return fun;	
}

C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateStep( double min, double max, double threshold ) {
	NnfwOutputFunction* fun = new NnfwOutputFunction();
	fun->func = new StepFunction( min, max, threshold );
	return fun;	
}

C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateSigmoid( double l ) {
	NnfwOutputFunction* fun = new NnfwOutputFunction();
	fun->func = new SigmoidFunction( l );
	return fun;	
}

C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateFakeSigmoid( double l ) {
	NnfwOutputFunction* fun = new NnfwOutputFunction();
	fun->func = new FakeSigmoidFunction( l );
	return fun;	
}

C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateScaledSigmoid( double l, double min, double max ) {
	NnfwOutputFunction* fun = new NnfwOutputFunction();
	fun->func = new ScaledSigmoidFunction( l, min, max );
	return fun;	
}

C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateGaussian( double centre, double var, double max ) {
	NnfwOutputFunction* fun = new NnfwOutputFunction();
	fun->func = new GaussFunction( centre, var, max );
	return fun;	
}

C_NNFW_API NnfwBaseNeuralNet* NnfwBaseNeuralNetCreate() {
	NnfwBaseNeuralNet* net = new NnfwBaseNeuralNet();
	net->net = new BaseNeuralNet();
	return net;
}

C_NNFW_API void NnfwBaseNeuralNetAddInputCluster( NnfwBaseNeuralNet* net, NnfwCluster* cl ) {
	net->net->addInputCluster( cl->cluster );
}

C_NNFW_API void NnfwBaseNeuralNetAddHiddenCluster( NnfwBaseNeuralNet* net, NnfwCluster* cl ) {
	net->net->addCluster( cl->cluster, false, false );
}

C_NNFW_API void NnfwBaseNeuralNetAddOutputCluster( NnfwBaseNeuralNet* net, NnfwCluster* cl ) {
	net->net->addOutputCluster( cl->cluster );
}

C_NNFW_API void NnfwBaseNeuralNetRemoveCluster( NnfwBaseNeuralNet* net, NnfwCluster* cl ) {
	net->net->removeCluster( cl->cluster );
}

C_NNFW_API void NnfwBaseNeuralNetMarkAsInput( NnfwBaseNeuralNet* net, NnfwCluster* cl ) {
	net->net->markAsInput( cl->cluster );
}

C_NNFW_API void NnfwBaseNeuralNetMarkAsOutput( NnfwBaseNeuralNet* net, NnfwCluster* cl ) {
	net->net->markAsOutput( cl->cluster );
}

C_NNFW_API void NnfwBaseNeuralNetMarkAsHidden( NnfwBaseNeuralNet* net, NnfwCluster* cl ) {
	net->net->unmark( cl->cluster );
}

C_NNFW_API NnfwIterator* NnfwBaseNeuralNetClusters( NnfwBaseNeuralNet* net ) {
	NnfwIterator* itera = new NnfwIterator();
	itera->type = 0;
	itera->clvec = &(net->net->clusters());
	return itera;
}

C_NNFW_API NnfwIterator* NnfwBaseNeuralNetInputClusters( NnfwBaseNeuralNet* net ) {
	NnfwIterator* itera = new NnfwIterator();
	itera->type = 0;
	itera->clvec = &(net->net->inputClusters());
	return itera;
}

C_NNFW_API NnfwIterator* NnfwBaseNeuralNetHiddenClusters( NnfwBaseNeuralNet* net ) {
	NnfwIterator* itera = new NnfwIterator();
	itera->type = 0;
	itera->clvec = &(net->net->hiddenClusters());
	return itera;
}

C_NNFW_API NnfwIterator* NnfwBaseNeuralNetOutputClusters( NnfwBaseNeuralNet* net ) {
	NnfwIterator* itera = new NnfwIterator();
	itera->type = 0;
	itera->clvec = &(net->net->outputClusters());
	return itera;
}

C_NNFW_API void NnfwBaseNeuralNetAddLinker( NnfwBaseNeuralNet* net, NnfwLinker* link ) {
	net->net->addLinker( link->linker );
}

C_NNFW_API void NnfwBaseNeuralNetRemoveLinker( NnfwBaseNeuralNet* net, NnfwLinker* link ) {
	net->net->removeLinker( link->linker );
}

C_NNFW_API NnfwIterator* NnfwBaseNeuralNetLinkers( NnfwBaseNeuralNet* net ) {
	NnfwIterator* itera = new NnfwIterator();
	itera->type = 1;
	itera->linkvec = &(net->net->linkers());
	return itera;
}

C_NNFW_API NnfwIterator* NnfwBaseNeuralNetOutGoingLinkers( NnfwBaseNeuralNet* net, NnfwCluster* cl ) {
	NnfwIterator* itera = new NnfwIterator();
	itera->type = 1;
	itera->linkvec = &(net->net->linkers( cl->cluster, true ));
	return itera;
}

C_NNFW_API NnfwIterator* NnfwBaseNeuralNetIncomingLinkers( NnfwBaseNeuralNet* net, NnfwCluster* cl ) {
	NnfwIterator* itera = new NnfwIterator();
	itera->type = 1;
	itera->linkvec = &(net->net->linkers( cl->cluster, false ));
	return itera;
}

C_NNFW_API void NnfwBaseNeuralNetSetOrder( NnfwBaseNeuralNet* net, int n, ... ) {
	va_list ap;
	va_start( ap, n );
	UpdatableList ord;
	//bool end = false;
	for( int i=0; i<n; i++ ) {
	//while( !end ) {
		void* ptr = va_arg( ap, void* );
		NnfwCluster* trycl = (NnfwCluster*)(ptr);
		if ( dynamic_cast<Cluster*>(trycl->myup) ) {
			ord.append( trycl->cluster );
			continue;
		}
		NnfwLinker* trylink = (NnfwLinker*)(ptr);
		if ( dynamic_cast<Linker*>(trylink->myup) ) {
			ord.append( trylink->linker );
			continue;
		}
	}
	va_end( ap );
	net->net->setOrder( ord );
}

C_NNFW_API NnfwIterator* NnfwBaseNeuralNetOrder( NnfwBaseNeuralNet* net ) {
	NnfwIterator* itera = new NnfwIterator();
	itera->type = 2;
	itera->upvec = &(net->net->order());
	return itera;
}

C_NNFW_API void NnfwBaseNeuralNetStep( NnfwBaseNeuralNet* net ) {
	net->net->step();
}

C_NNFW_API void NnfwBaseNeuralNetRandomize( NnfwBaseNeuralNet* net, double min, double max ) {
	net->net->randomize( min, max );
}

C_NNFW_API int NnfwIteratorSize( NnfwIterator* itera ) {
	switch(itera->type) {
	case 0:
		return itera->clvec->size();
		break;
	case 1:
		return itera->linkvec->size();
		break;
	case 2:
		return itera->upvec->size();
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

C_NNFW_API NnfwIteratorElementTypes NnfwIteratorGetType( NnfwIterator* itera, int i ) {
	if ( itera->type == 1 ) {
		return LinkerType;
	}
	if ( itera->type == 2 ) {
		if ( dynamic_cast<Linker*>(itera->upvec->at(i)) ) {
			return LinkerType;
		}
		return ClusterType;
	}
	return ClusterType;
}

C_NNFW_API int NnfwIteratorIsCluster( NnfwIterator* itera, int i ) {
	if ( itera->type == 1 ) {
		return 0;
	}
	if ( itera->type == 2 ) {
		if ( dynamic_cast<Linker*>(itera->upvec->at(i)) ) {
			return 0;
		}
		return 1;
	}
	return 1;
}

C_NNFW_API int NnfwIteratorIsLinker( NnfwIterator* itera, int i ) {
	if ( itera->type == 1 ) {
		return 1;
	}
	if ( itera->type == 2 ) {
		if ( dynamic_cast<Linker*>(itera->upvec->at(i)) ) {
			return 1;
		}
		return 0;
	}
	return 0;
}

C_NNFW_API NnfwCluster* NnfwIteratorGetCluster( NnfwIterator* itera, int i ) {
	Cluster* cl;
	switch( itera->type ) {
	case 0:
		cl = itera->clvec->at(i);
		break;
	case 1:
		return 0;
		break;
	case 2:
		if ( dynamic_cast<Linker*>(itera->upvec->at(i)) ) {
			return 0;
		}
		cl = (Cluster*)(itera->upvec->at(i));
		break;
	default:
		return 0;
	}
	if ( clmap.count( cl ) == 0 ) {
		NnfwCluster* ncl = new NnfwCluster();
		ncl->cluster = cl;
		ncl->myup = ncl->cluster;
		ncl->func = new NnfwOutputFunction();
		ncl->func->func = cl->getFunction();
		clmap[cl] = ncl;
	}
	return clmap[cl];
}

C_NNFW_API NnfwLinker* NnfwIteratorGetLinker( NnfwIterator* itera, int i ) {
	Linker* ln;
	switch( itera->type ) {
	case 0:
		return 0;
		break;
	case 1:
		ln = itera->linkvec->at(i);
		break;
	case 2:
		if ( dynamic_cast<Cluster*>(itera->upvec->at(i)) ) {
			return 0;
		}
		ln = (Linker*)(itera->upvec->at(i));
		break;
	default:
		return 0;
	}
	if ( linkmap.count( ln ) == 0 ) {
		NnfwLinker* nln = new NnfwLinker();
		nln->linker = ln;
		nln->myup = nln->linker;
		Cluster* cl = ln->from();
		if ( clmap.count( cl ) == 0 ) {
			NnfwCluster* ncl = new NnfwCluster();
			ncl->cluster = cl;
			ncl->myup = ncl->cluster;
			ncl->func = new NnfwOutputFunction();
			ncl->func->func = cl->getFunction();
			clmap[cl] = ncl;
		}
		nln->from = clmap[cl];
		cl = ln->to();
		if ( clmap.count( cl ) == 0 ) {
			NnfwCluster* ncl = new NnfwCluster();
			ncl->cluster = cl;
			ncl->myup = ncl->cluster;
			ncl->func = new NnfwOutputFunction();
			ncl->func->func = cl->getFunction();
			clmap[cl] = ncl;
		}
		nln->to = clmap[cl];
		linkmap[ln] = nln;
	}
	return linkmap[ln];
}

C_NNFW_API NnfwPatternSet* NnfwPatternSetCreate( int size ) {
	NnfwPatternSet* pset = new NnfwPatternSet();
	pset->pset = new PatternSet( size );
	return pset;
}

C_NNFW_API int NnfwPatternSetSize( NnfwPatternSet* set ) {
	return set->pset->size();
}

C_NNFW_API void NnfwPatternSetSetInputsOf( NnfwPatternSet* pset, int i, NnfwCluster* cl, double* inputs ) {
	RealVec rv( inputs, cl->cluster->numNeurons() );
	(*(pset->pset))[i].setInputsOf( cl->cluster, rv );
}

C_NNFW_API void NnfwPatternSetSetOutputsOf( NnfwPatternSet* pset, int i, NnfwCluster* cl, double* outputs ) {
	RealVec rv( outputs, cl->cluster->numNeurons() );
	(*(pset->pset))[i].setOutputsOf( cl->cluster, rv );
}

C_NNFW_API double* NnfwPatternSetGetInputsOf( NnfwPatternSet* pset, int i, NnfwCluster* cl ) {
	return getRawData( (RealVec&)(pset->pset->at(i).inputsOf( cl->cluster )) );
}

C_NNFW_API double* NnfwPatternSetGetOutputsOf( NnfwPatternSet* pset, int i, NnfwCluster* cl ) {
	return getRawData( (RealVec&)(pset->pset->at(i).outputsOf( cl->cluster )) );
}


C_NNFW_API NnfwLearningAlgorithm* NnfwBackPropagationCreate( NnfwBaseNeuralNet* net, double learn_rate, int n, ... ) {
	va_list ap;
	va_start( ap, n );
	UpdatableList ord;
	//bool end = false;
	for( int i=0; i<n; i++ ) {
	//while( !end ) {
		void* ptr = va_arg( ap, void* );
		NnfwCluster* trycl = (NnfwCluster*)(ptr);
		if ( dynamic_cast<Cluster*>(trycl->myup) ) {
			ord << trycl->cluster;
			continue;
		}
		NnfwLinker* trylink = (NnfwLinker*)(ptr);
		if ( dynamic_cast<Linker*>(trylink->myup) ) {
			ord << trylink->linker;
			continue;
		}
	}
	va_end( ap );
	NnfwLearningAlgorithm* learn = new NnfwLearningAlgorithm();
	learn->learn = new BackPropagationAlgo( net->net, ord, learn_rate );
	return learn;
}

C_NNFW_API void NnfwBackPropagationEnableMomentum( NnfwLearningAlgorithm* bp, int enable ) {
	if ( enable ) {
		((BackPropagationAlgo*)bp->learn)->enableMomentum();
	} else {
		((BackPropagationAlgo*)bp->learn)->disableMomentum();
	}
}

C_NNFW_API void NnfwBackPropagationSetLearnRate( NnfwLearningAlgorithm* bp, double rate ) {
	((BackPropagationAlgo*)bp->learn)->setRate( rate );
}

C_NNFW_API void NnfwBackPropagationSetMomentum( NnfwLearningAlgorithm* bp, double mom ) {
	((BackPropagationAlgo*)bp->learn)->setMomentum( mom );
}

C_NNFW_API double NnfwBackPropagationLearnRate( NnfwLearningAlgorithm* bp ) {
	return ((BackPropagationAlgo*)bp->learn)->rate();
}

C_NNFW_API double NnfwBackPropagationMomentum( NnfwLearningAlgorithm* bp ) {
	return ((BackPropagationAlgo*)bp->learn)->momentum();
}

C_NNFW_API void NnfwBackPropagationLearn( NnfwLearningAlgorithm* bp, NnfwPatternSet* pset ) {
	bp->learn->learnOnSet( *(pset->pset) );
}

C_NNFW_API double NnfwBackPropagationCalcMSE( NnfwLearningAlgorithm* bp, NnfwPatternSet* pset ) {
	return bp->learn->calculateMSEOnSet( *(pset->pset) );
}

/* ** TODO: re-implement when the load/save facilities will come back
C_NNFW_API NnfwBaseNeuralNet* NnfwLoadXML( const char* filename ) {
	NnfwBaseNeuralNet* net = new NnfwBaseNeuralNet();
	BaseNeuralNet* nn = loadXML( filename );
	net->net = nn;
	return net;
}

C_NNFW_API void NnfwSaveXML( const char* filename, NnfwBaseNeuralNet* net, int precision, const char* skipList ) {
	saveXML( filename, net->net, precision, skipList );
	return;
}
** */

#ifdef __cplusplus
};
#endif
