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

#ifndef CNNFW_H
#define CNNFW_H

/*! \file
 *  \brief This is the C interface. It allow to use the NNFW library from a standard C code 
 */

#ifdef NNFW_H
#error "Don't try to use C++ definition with C definition"
#endif
#ifdef TYPES_H
#error "Don't try to use C++ definition with C definition"
#endif

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

#define NNFW_VERSION 10400

/*! Abstraction on the type of real numbers */
#ifndef NNFW_DOUBLE_PRECISION
typedef float Real;
#else
typedef double Real;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NnfwCluster{} NnfwCluster;
typedef struct NnfwLinker{} NnfwLinker;
typedef struct NnfwOutputFunction{} NnfwOutputFunction;
typedef struct NnfwBaseNeuralNet{} NnfwBaseNeuralNet;
typedef struct NnfwLearningAlgorithm{} NnfwLearningAlgorithm;

/*! \defgroup cinterface C Interface */

/*! \defgroup cluster
 *  \ingroup cinterface
 */
//@{
/*! Create a Cluster with type specified */
C_NNFW_API NnfwCluster* NnfwClusterCreate( const char* type, unsigned int numNeurons );
/*! update the outputs of neurons */
C_NNFW_API void NnfwClusterUpdate( NnfwCluster* );
/*! Return the number of Neurons in the Cluster */
C_NNFW_API unsigned int NnfwClusterNumNeurons( NnfwCluster* );
/*! Set Accumulate mode
 * \param accumulate if 0 disable accumulate mode, otherwise enable it
 */
C_NNFW_API void NnfwClusterSetAccumulate( NnfwCluster*, int accumulate );
/*! Return 1 if accumulate is enabled, 0 otherwise */
C_NNFW_API int NnfwClusterIsAccumulate( NnfwCluster* );
/*! Randomize the parameters of Cluster within min and max values specified */
C_NNFW_API void NnfwClusterRandomize( NnfwCluster*, Real min, Real max );
/*! Configure the inputs of the Cluster
 * \param ins is an array of at least NnfwClusterNumNeurons containing the inputs values
 */
C_NNFW_API void NnfwClusterSetInputs( NnfwCluster*, Real* ins );
/*! Set a single input of Cluster
 * \param neuron index of neuron of which to setup the input
 * \param value the value of input
 */
C_NNFW_API void NnfwClusterSetInput( NnfwCluster*, unsigned int neuron, Real value );
/*! Return a pointer to an array of NnfwClusteNumNeurons Real containing the current inputs values */
C_NNFW_API Real* NnfwClusterInputs( NnfwCluster* );
/*! */
C_NNFW_API Real NnfwClusterInput( NnfwCluster*, unsigned int neuron );
/*! */
C_NNFW_API void NnfwClusterSetOutputs( NnfwCluster*, Real* outs );
/*! */
C_NNFW_API void NnfwClusterSetOutput( NnfwCluster*, unsigned int neuron, Real value );
/*! */
C_NNFW_API Real* NnfwClusterOutputs( NnfwCluster* );
/*! */
C_NNFW_API Real NnfwClusterOutput( NnfwCluster*, unsigned int neuron );
/*! */
C_NNFW_API void NnfwClusterSetFunction( NnfwCluster*, NnfwOutputFunction* );
/*! */
C_NNFW_API NnfwOutputFunction* NnfwClusterFunction( NnfwCluster* );
/*! Clone this Cluster */
C_NNFW_API NnfwCluster* NnfwClusterClone( NnfwCluster* );
//@}

/*! \defgroup simplecluster Simple Cluster specific operations
 *  \ingroup cluster
 */
//@{
/*! Create a Simple Cluster */
C_NNFW_API NnfwCluster* NnfwClusterCreateSimple( unsigned int numNeurons );
//@}

/*! \defgroup biasedcluster Biased Cluster specific operations
 *  \ingroup cluster
 */
//@{
/*! Create a Biased Cluster */
C_NNFW_API NnfwCluster* NnfwClusterCreateBiased( unsigned int numNeurons );
/*! Set the biases of neurons */
C_NNFW_API void NnfwBiasedClusterSetBiases( NnfwCluster*, Real* outs );
/*! Set the bias of neuron */
C_NNFW_API void NnfwBiasedClusterSetBias( NnfwCluster*, unsigned int neuron, Real value );
/*! Return the biases */
C_NNFW_API Real* NnfwBiasedClusterBiases( NnfwCluster* );
/*! Return the bias of neuron */
C_NNFW_API Real NnfwBiasedClusterBias( NnfwCluster*, unsigned int neuron );
//@}

/*! \defgroup fakecluster Fake Cluster specific operations
 *  \ingroup cluster
 */
//@{
/*! Create a Fake Cluster */
C_NNFW_API NnfwCluster* NnfwClusterCreateFake( unsigned int numNeurons );
//@}

/*! \defgroup linker
 *  \ingroup cinterface
 */
//@{
/*! Create a Linker with type specified */
C_NNFW_API NnfwLinker* NnfwLinkerCreate( const char* type, NnfwCluster* from, NnfwCluster* to );
/*! Return the NnfwCluster from */
C_NNFW_API NnfwCluster* NnfwLinkerFrom( NnfwLinker* );
/*! Return the NnfwCluster to */
C_NNFW_API NnfwCluster* NnfwLinkerTo( NnfwLinker* );
/*! update the linker */
C_NNFW_API void NnfwLinkerUpdate( NnfwLinker* );
/*! Return the dimension of the Linker */
C_NNFW_API unsigned int NnfwLinkerSize( NnfwLinker* );
/*! Randomize the parameters of Linker within min and max values specified */
C_NNFW_API void NnfwLinkerRandomize( NnfwLinker*, Real min, Real max );
/*! Clone this linker */
C_NNFW_API NnfwLinker* NnfwLinkerClone( NnfwLinker* );
//@}

/*! \defgroup dotlinker Dot Matrix Linker specific operations
 *  \ingroup linker
 */
//@{
/*! Create a Dot Linker */
C_NNFW_API NnfwLinker* NnfwLinkerCreateDot( NnfwCluster* from, NnfwCluster* to );
/*! Set the weight matrix
 * \param matrix is an array of Real containing a matrix in row-major convention */
C_NNFW_API void NnfwDotLinkerSetWeights( NnfwLinker* link, Real* matrix );
/*! Set a single weight from neuron 'from' to neuron 'to' */
C_NNFW_API void NnfwDotLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, Real w );
/*! Return the weight matrix in row-major convention */
C_NNFW_API Real* NnfwDotLinkerWeights( NnfwLinker* );
/*! Return the weight of connection between 'from' and 'to' neurons */
C_NNFW_API Real NnfwDotLinkerWeight( NnfwLinker*, unsigned int from, unsigned int to );
/*! return the number of row of weight matrix */
C_NNFW_API int NnfwDotLinkerRows( NnfwLinker* );
/*! return the number of columns of weight matrix */
C_NNFW_API int NnfwDotLinkerCols( NnfwLinker* );
//@}

/*! \defgroup normlinker Norm Matrix Linker specific operations
 *  \ingroup linker
 */
//@{
/*! Create a Norm Linker */
C_NNFW_API NnfwLinker* NnfwLinkerCreateNorm( NnfwCluster* from, NnfwCluster* to );
/*! Set the weight matrix
 * \param matrix is an array of Real containing a matrix in row-major convention */
C_NNFW_API void NnfwNormLinkerSetWeights( NnfwLinker* link, Real* matrix );
/*! Set a single weight from neuron 'from' to neuron 'to' */
C_NNFW_API void NnfwNormLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, Real w );
/*! Return the weight matrix in row-major convention */
C_NNFW_API Real* NnfwNormLinkerWeights( NnfwLinker* );
/*! Return the weight of connection between 'from' and 'to' neurons */
C_NNFW_API Real NnfwNormLinkerWeight( NnfwLinker*, unsigned int from, unsigned int to );
/*! return the number of row of weight matrix */
C_NNFW_API int NnfwNormLinkerRows( NnfwLinker* );
/*! return the number of columns of weight matrix */
C_NNFW_API int NnfwNormLinkerCols( NnfwLinker* );
//@}

/*! \defgroup sparselinker Sparse Matrix Linker specific operations
 *  \ingroup linker
 */
//@{
/*! Create a Sparse Linker */
C_NNFW_API NnfwLinker* NnfwLinkerCreateSparse( NnfwCluster* from, NnfwCluster* to );
/*! Set the weight matrix
 * \param matrix is an array of Real containing a matrix in row-major convention */
C_NNFW_API void NnfwSparseLinkerSetWeights( NnfwLinker* link, Real* matrix );
/*! Set a single weight from neuron 'from' to neuron 'to' */
C_NNFW_API void NnfwSparseLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, Real w );
/*! Return the weight matrix in row-major convention */
C_NNFW_API Real* NnfwSparseLinkerWeights( NnfwLinker* );
/*! Return the weight of connection between 'from' and 'to' neurons */
C_NNFW_API Real NnfwSparseLinkerWeight( NnfwLinker*, unsigned int from, unsigned int to );
/*! return the number of row of weight matrix */
C_NNFW_API int NnfwSparseLinkerRows( NnfwLinker* );
/*! return the number of columns of weight matrix */
C_NNFW_API int NnfwSparseLinkerCols( NnfwLinker* );
/*! connect the neurons 'from' and 'to' */
C_NNFW_API void NnfwSparseLinkerConnect( NnfwLinker*, unsigned int from, unsigned int to );
/*! connect all neurons */
C_NNFW_API void NnfwSparseLinkerConnectAll( NnfwLinker* );
/*! connect with probability prob */
C_NNFW_API void NnfwSparseLinkerConnectRandom( NnfwLinker*, Real prob );
/*! disconnect the neurons 'from' and 'to' */
C_NNFW_API void NnfwSparseLinkerDisconnect( NnfwLinker*, unsigned int from, unsigned int to );
/*! disconnect all neurons */
C_NNFW_API void NnfwSparseLinkerDisconnectAll( NnfwLinker* );
/*! disconnect with probability prob */
C_NNFW_API void NnfwSparseLinkerDisconnectRandom( NnfwLinker*, Real prob );
/*! return the connection mask matrix, 1 mean connected, 0 means disconnected */
C_NNFW_API int* NnfwSparseLinkerConnectionMask( NnfwLinker* );
/*! return 1 if neurons are connected, 0 otherwise */
C_NNFW_API int NnfwSparseLinkerConnectionAt( NnfwLinker*, unsigned int from, unsigned int to );
//@}

/*! \defgroup copylinker Copy Linker specific operations
 *  \ingroup linker
 */
//@{
/*! enum of possible modality of copying */
typedef enum { In2In, In2Out, Out2In, Out2Out } NnfwCopyLinkerModes;
/*! Create a Copy Linker */
C_NNFW_API NnfwLinker* NnfwLinkerCreateCopy( NnfwCluster* from, NnfwCluster* to );
/*! Set the modality of copying */
C_NNFW_API void NnfwCopyLinkerSetMode( NnfwLinker* link, NnfwCopyLinkerModes mode );
/*! Return the modality of copying */
C_NNFW_API NnfwCopyLinkerModes NnfwCopyLinkerMode( NnfwLinker* link );
//@}

#ifdef __cplusplus
};
#endif

#endif

