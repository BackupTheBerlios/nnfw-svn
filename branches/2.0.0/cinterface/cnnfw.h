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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NnfwCluster{} NnfwCluster;
typedef struct NnfwLinker{} NnfwLinker;
typedef struct NnfwOutputFunction{} NnfwOutputFunction;
typedef struct NnfwBaseNeuralNet{} NnfwBaseNeuralNet;
typedef struct NnfwLearningAlgorithm{} NnfwLearningAlgorithm;
typedef struct NnfwIterator{} NnfwIterator;
typedef struct NnfwPatternSet{} NnfwPatternSet;

/*! \defgroup cinterface C Interface */

/*! \defgroup cluster Cluster functions
 *  \ingroup cinterface
 */
//@{
// *! Create a Cluster with type specified */
//C_NNFW_API NnfwCluster* NnfwClusterCreate( const char* type, unsigned int numNeurons );
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
C_NNFW_API void NnfwClusterRandomize( NnfwCluster*, double min, double max );
/*! Configure the inputs of the Cluster
 * \param ins is an array of at least NnfwClusterNumNeurons containing the inputs values
 */
C_NNFW_API void NnfwClusterSetInputs( NnfwCluster*, double* ins );
/*! Set a single input of Cluster
 * \param neuron index of neuron of which to setup the input
 * \param value the value of input
 */
C_NNFW_API void NnfwClusterSetInput( NnfwCluster*, unsigned int neuron, double value );
/*! Return a pointer to an array of NnfwClusteNumNeurons double containing the current inputs values */
C_NNFW_API double* NnfwClusterInputs( NnfwCluster* );
/*! */
C_NNFW_API double NnfwClusterInput( NnfwCluster*, unsigned int neuron );
/*! */
C_NNFW_API void NnfwClusterSetOutputs( NnfwCluster*, double* outs );
/*! */
C_NNFW_API void NnfwClusterSetOutput( NnfwCluster*, unsigned int neuron, double value );
/*! */
C_NNFW_API double* NnfwClusterOutputs( NnfwCluster* );
/*! */
C_NNFW_API double NnfwClusterOutput( NnfwCluster*, unsigned int neuron );
/*! */
C_NNFW_API void NnfwClusterSetFunction( NnfwCluster*, NnfwOutputFunction* );
/*! */
C_NNFW_API NnfwOutputFunction* NnfwClusterFunction( NnfwCluster* );
/*! Clone this Cluster */
C_NNFW_API NnfwCluster* NnfwClusterClone( NnfwCluster* );
//@}

/*! \defgroup simplecluster Simple Cluster
 *  \ingroup cluster
 */
//@{
/*! Create a Simple Cluster */
C_NNFW_API NnfwCluster* NnfwClusterCreateSimple( unsigned int numNeurons );
//@}

/*! \defgroup biasedcluster Biased Cluster
 *  \ingroup cluster
 */
//@{
/*! Create a Biased Cluster */
C_NNFW_API NnfwCluster* NnfwClusterCreateBiased( unsigned int numNeurons );
/*! Set the biases of neurons */
C_NNFW_API void NnfwBiasedClusterSetBiases( NnfwCluster*, double* outs );
/*! Set the bias of neuron */
C_NNFW_API void NnfwBiasedClusterSetBias( NnfwCluster*, unsigned int neuron, double value );
/*! Return the biases */
C_NNFW_API double* NnfwBiasedClusterBiases( NnfwCluster* );
/*! Return the bias of neuron */
C_NNFW_API double NnfwBiasedClusterBias( NnfwCluster*, unsigned int neuron );
//@}

/*! \defgroup fakecluster Fake Cluster
 *  \ingroup cluster
 */
//@{
/*! Create a Fake Cluster */
C_NNFW_API NnfwCluster* NnfwClusterCreateFake( unsigned int numNeurons );
//@}

/*! \defgroup linker Linker functions
 *  \ingroup cinterface
 */
//@{
// *! Create a Linker with type specified */
// C_NNFW_API NnfwLinker* NnfwLinkerCreate( const char* type, NnfwCluster* from, NnfwCluster* to );
/*! Return the NnfwCluster from */
C_NNFW_API NnfwCluster* NnfwLinkerFrom( NnfwLinker* );
/*! Return the NnfwCluster to */
C_NNFW_API NnfwCluster* NnfwLinkerTo( NnfwLinker* );
/*! update the linker */
C_NNFW_API void NnfwLinkerUpdate( NnfwLinker* );
/*! Return the dimension of the Linker */
C_NNFW_API unsigned int NnfwLinkerSize( NnfwLinker* );
/*! Randomize the parameters of Linker within min and max values specified */
C_NNFW_API void NnfwLinkerRandomize( NnfwLinker*, double min, double max );
/*! Clone this linker */
C_NNFW_API NnfwLinker* NnfwLinkerClone( NnfwLinker* );
//@}

/*! \defgroup dotlinker Dot Matrix Linker
 *  \ingroup linker
 */
//@{
/*! Create a Dot Linker */
C_NNFW_API NnfwLinker* NnfwLinkerCreateDot( NnfwCluster* from, NnfwCluster* to );
/*! Set the weight matrix
 * \param matrix is an array of double containing a matrix in row-major convention */
C_NNFW_API void NnfwDotLinkerSetWeights( NnfwLinker* link, double* matrix );
/*! Set a single weight from neuron 'from' to neuron 'to' */
C_NNFW_API void NnfwDotLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, double w );
/*! Return the weight matrix in row-major convention */
C_NNFW_API double* NnfwDotLinkerWeights( NnfwLinker* );
/*! Return the weight of connection between 'from' and 'to' neurons */
C_NNFW_API double NnfwDotLinkerWeight( NnfwLinker*, unsigned int from, unsigned int to );
/*! return the number of row of weight matrix */
C_NNFW_API int NnfwDotLinkerRows( NnfwLinker* );
/*! return the number of columns of weight matrix */
C_NNFW_API int NnfwDotLinkerCols( NnfwLinker* );
//@}

/*! \defgroup normlinker Norm Matrix Linker
 *  \ingroup linker
 */
//@{
/*! Create a Norm Linker */
C_NNFW_API NnfwLinker* NnfwLinkerCreateNorm( NnfwCluster* from, NnfwCluster* to );
/*! Set the weight matrix
 * \param matrix is an array of double containing a matrix in row-major convention */
C_NNFW_API void NnfwNormLinkerSetWeights( NnfwLinker* link, double* matrix );
/*! Set a single weight from neuron 'from' to neuron 'to' */
C_NNFW_API void NnfwNormLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, double w );
/*! Return the weight matrix in row-major convention */
C_NNFW_API double* NnfwNormLinkerWeights( NnfwLinker* );
/*! Return the weight of connection between 'from' and 'to' neurons */
C_NNFW_API double NnfwNormLinkerWeight( NnfwLinker*, unsigned int from, unsigned int to );
/*! return the number of row of weight matrix */
C_NNFW_API int NnfwNormLinkerRows( NnfwLinker* );
/*! return the number of columns of weight matrix */
C_NNFW_API int NnfwNormLinkerCols( NnfwLinker* );
//@}

/*! \defgroup sparselinker Sparse Matrix Linker
 *  \ingroup linker
 */
//@{
/*! Create a Sparse Linker */
C_NNFW_API NnfwLinker* NnfwLinkerCreateSparse( NnfwCluster* from, NnfwCluster* to );
/*! Set the weight matrix
 * \param matrix is an array of double containing a matrix in row-major convention */
C_NNFW_API void NnfwSparseLinkerSetWeights( NnfwLinker* link, double* matrix );
/*! Set a single weight from neuron 'from' to neuron 'to' */
C_NNFW_API void NnfwSparseLinkerSetWeight( NnfwLinker* link, unsigned int from, unsigned int to, double w );
/*! Return the weight matrix in row-major convention */
C_NNFW_API double* NnfwSparseLinkerWeights( NnfwLinker* );
/*! Return the weight of connection between 'from' and 'to' neurons */
C_NNFW_API double NnfwSparseLinkerWeight( NnfwLinker*, unsigned int from, unsigned int to );
/*! return the number of row of weight matrix */
C_NNFW_API int NnfwSparseLinkerRows( NnfwLinker* );
/*! return the number of columns of weight matrix */
C_NNFW_API int NnfwSparseLinkerCols( NnfwLinker* );
/*! connect the neurons 'from' and 'to' */
C_NNFW_API void NnfwSparseLinkerConnect( NnfwLinker*, unsigned int from, unsigned int to );
/*! connect all neurons */
C_NNFW_API void NnfwSparseLinkerConnectAll( NnfwLinker* );
/*! connect with probability prob */
C_NNFW_API void NnfwSparseLinkerConnectRandom( NnfwLinker*, double prob );
/*! disconnect the neurons 'from' and 'to' */
C_NNFW_API void NnfwSparseLinkerDisconnect( NnfwLinker*, unsigned int from, unsigned int to );
/*! disconnect all neurons */
C_NNFW_API void NnfwSparseLinkerDisconnectAll( NnfwLinker* );
/*! disconnect with probability prob */
C_NNFW_API void NnfwSparseLinkerDisconnectRandom( NnfwLinker*, double prob );
//--- the connection matrix can't be returned because there is no obvious or conventional way
//--- (or portable) to convert the bool C++ type into an "int-boolean" C value
//C_NNFW_API int* NnfwSparseLinkerConnectionMask( NnfwLinker* );
/*! return 1 if neurons are connected, 0 otherwise */
C_NNFW_API int NnfwSparseLinkerConnectionAt( NnfwLinker*, unsigned int from, unsigned int to );
//@}

/*! \defgroup copylinker Copy Linker
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

/*! \defgroup outfunc Output Functions
 *  \ingroup cinterface
 */
//@{
// *! Create an OutputFunction with type specified */
//C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreate( const char* type );
//@}
/*! \defgroup func1 Identity Function
 *  \ingroup outfunc
 */
//@{
/*! Create an Identity Function */
C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateIdentity();
//@}
/*! \defgroup func2 Linear Function
 *  \ingroup outfunc
 */
//@{
/*! Create an Linear Function */
C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateLinear( double m, double b );
//@}
/*! \defgroup func3 Ramp Function
 *  \ingroup outfunc
 */
//@{
/*! Create an Ramp Function */
C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateRamp( double minX, double maxX, double minY, double maxY );
//@}
/*! \defgroup func4 Step Function
 *  \ingroup outfunc
 */
//@{
/*! Create an Step Function */
C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateStep( double min, double max, double threshold );
//@}
/*! \defgroup func5 Sigmoid Function
 *  \ingroup outfunc
 */
//@{
/*! Create an Sigmoid Function */
C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateSigmoid( double l );
//@}
/*! \defgroup func6 Fake Sigmoid Function
 *  \ingroup outfunc
 */
//@{
/*! Create an Fake Sigmoid Function */
C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateFakeSigmoid( double l );
//@}
/*! \defgroup func7 Scaled Sigmoid Function
 *  \ingroup outfunc
 */
//@{
/*! Create an Scaled Sigmoid Function */
C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateScaledSigmoid( double l, double min, double max );
//@}
/*! \defgroup func8 Gaussian Function
 *  \ingroup outfunc
 */
//@{
/*! Create an Gaussian Function */
C_NNFW_API NnfwOutputFunction* NnfwOutputFunctionCreateGaussian( double centre, double var, double max );
//@}

/*! \defgroup net Base Neural Net functions
 *  \ingroup cinterface
 */
//@{
/*! Create a BaseNeuralNet */
C_NNFW_API NnfwBaseNeuralNet* NnfwBaseNeuralNetCreate();
/*! Add an Input Cluster to BaseNeuralNet */
C_NNFW_API void NnfwBaseNeuralNetAddInputCluster( NnfwBaseNeuralNet* net, NnfwCluster* cl );
/*! Add an Hidden Cluster to BaseNeuralNet */
C_NNFW_API void NnfwBaseNeuralNetAddHiddenCluster( NnfwBaseNeuralNet* net, NnfwCluster* cl );
/*! Add an Output Cluster to BaseNeuralNet */
C_NNFW_API void NnfwBaseNeuralNetAddOutputCluster( NnfwBaseNeuralNet* net, NnfwCluster* cl );
/*! Remove the Cluster from BaseNeuralNet */
C_NNFW_API void NnfwBaseNeuralNetRemoveCluster( NnfwBaseNeuralNet* net, NnfwCluster* cl );
/*! Mark the Cluster passed as an Input Cluster of the BaseNeuralNet */
C_NNFW_API void NnfwBaseNeuralNetMarkAsInput( NnfwBaseNeuralNet* net, NnfwCluster* cl );
/*! Mark the Cluster passed as an Output Cluster of the BaseNeuralNet */
C_NNFW_API void NnfwBaseNeuralNetMarkAsOutput( NnfwBaseNeuralNet* net, NnfwCluster* cl );
/*! Mark the Cluster passed as an Hidden Cluster of the BaseNeuralNet */
C_NNFW_API void NnfwBaseNeuralNetMarkAsHidden( NnfwBaseNeuralNet* net, NnfwCluster* cl );
/*! Return the iterator over all Clusters of the BaseNeuralNet */
C_NNFW_API NnfwIterator* NnfwBaseNeuralNetClusters( NnfwBaseNeuralNet* net );
/*! Return the iterator over Input Clusters of the BaseNeuralNet */
C_NNFW_API NnfwIterator* NnfwBaseNeuralNetInputClusters( NnfwBaseNeuralNet* net );
/*! Return the iterator over Hidden Clusters of the BaseNeuralNet */
C_NNFW_API NnfwIterator* NnfwBaseNeuralNetHiddenClusters( NnfwBaseNeuralNet* net );
/*! Return the iterator over Output Clusters of the BaseNeuralNet */
C_NNFW_API NnfwIterator* NnfwBaseNeuralNetOutputClusters( NnfwBaseNeuralNet* net );
/*! Add a Linker to BaseNeuralNet */
C_NNFW_API void NnfwBaseNeuralNetAddLinker( NnfwBaseNeuralNet* net, NnfwLinker* link );
/*! Remove a Linker to BaseNeuralNet */
C_NNFW_API void NnfwBaseNeuralNetRemoveLinker( NnfwBaseNeuralNet* net, NnfwLinker* link );
/*! Return the iterator over all Linkers of the BaseNeuralNet */
C_NNFW_API NnfwIterator* NnfwBaseNeuralNetLinkers( NnfwBaseNeuralNet* net );
/*! Return the iterator over Linkers out-going from Cluster passed of the BaseNeuralNet */
C_NNFW_API NnfwIterator* NnfwBaseNeuralNetOutGoingLinkers( NnfwBaseNeuralNet* net, NnfwCluster* cl );
/*! Return the iterator over Linkers incoming to Cluster passed of the BaseNeuralNet */
C_NNFW_API NnfwIterator* NnfwBaseNeuralNetIncomingLinkers( NnfwBaseNeuralNet* net, NnfwCluster* cl );
/*! Set the order of net spreading
 *  \param n is the number of element listed after it
 */
C_NNFW_API void NnfwBaseNeuralNetSetOrder( NnfwBaseNeuralNet* net, int n, ... );
/*! Return the iterator over the spreading order */
C_NNFW_API NnfwIterator* NnfwBaseNeuralNetOrder( NnfwBaseNeuralNet* net );
/*! Update the net */
C_NNFW_API void NnfwBaseNeuralNetStep( NnfwBaseNeuralNet* net );
/*! Randomize the net */
C_NNFW_API void NnfwBaseNeuralNetRandomize( NnfwBaseNeuralNet* net, double min, double max );
//@}
/*! \defgroup itera NnfwIterator functions
 *  NnfwIterator is an encapsulation of vectors implemented in NNFW.
 *  They behaves like array, but you cannot use the indexing operator '[]',
 *  so, an appropriate function has been provided for accessing elements
 *  \ingroup net
 */
//@{
/*! enum of possible types inside NnfwIterator */
typedef enum { ClusterType, LinkerType } NnfwIteratorElementTypes;
/*! Return the number of elements */
C_NNFW_API int NnfwIteratorSize( NnfwIterator* itera );
/*! Return the type of element at i-th position */
C_NNFW_API NnfwIteratorElementTypes NnfwIteratorGetType( NnfwIterator* itera, int i );
/*! Return true is at i-th position there is a NnfwCluster */
C_NNFW_API int NnfwIteratorIsCluster( NnfwIterator* itera, int i );
/*! Return true is at i-th position there is a NnfwLinker */
C_NNFW_API int NnfwIteratorIsLinker( NnfwIterator* itera, int i );
/*! Return the i-th position element */
C_NNFW_API NnfwCluster* NnfwIteratorGetCluster( NnfwIterator* itera, int i );
/*! Return the i-th position element */
C_NNFW_API NnfwLinker* NnfwIteratorGetLinker( NnfwIterator* itera, int i );
//@}

/*! \defgroup patternset Pattern Set functions
 *  \ingroup cinterface
 */
//@{
/*! Create a PatternSet of dimension size */
C_NNFW_API NnfwPatternSet* NnfwPatternSetCreate( int size );
/*! Return the number of Patterns containted into the PatternSet */
C_NNFW_API int NnfwPatternSetSize( NnfwPatternSet* set );
/*! Configure the Input of NnfwCluster passed for the i-th pattern */
C_NNFW_API void NnfwPatternSetSetInputsOf( NnfwPatternSet* pset, int i, NnfwCluster* cl, double* inputs );
/*! Configure the Output of NnfwCluster passed for the i-th pattern */
C_NNFW_API void NnfwPatternSetSetOutputsOf( NnfwPatternSet* pset, int i, NnfwCluster* cl, double* outputs );
/*! Return the Input of NnfwCluster of the pattern i-th */
C_NNFW_API double* NnfwPatternSetGetInputsOf( NnfwPatternSet* pset, int i, NnfwCluster* cl );
/*! Return the Output of NnfwCluster of the pattern i-th */
C_NNFW_API double* NnfwPatternSetGetOutputsOf( NnfwPatternSet* pset, int i, NnfwCluster* cl );
//@}

/*! \defgroup backprop BackPropagation Algorithm Learning
 *  \ingroup cinterface
 */
//@{
/*! Create a BackPropagation Algorithm 
 *  \param neuralnet is the Neural Network to teach (optimize)
 *  \param lear_rate is the learning rate to use
 *  \param n is the number of successive elements that specifies the teaching order
 *  \param ... is the order of backpropagation of the error (usually the reverse order of spreading)
 */
C_NNFW_API NnfwLearningAlgorithm* NnfwBackPropagationCreate( NnfwBaseNeuralNet* net, double learn_rate, int n, ... );
/*! Enable/Disable momentum */
C_NNFW_API void NnfwBackPropagationEnableMomentum( NnfwLearningAlgorithm* bp, int enable );
/*! Set Learn Rate */
C_NNFW_API void NnfwBackPropagationSetLearnRate( NnfwLearningAlgorithm* bp, double rate );
/*! Set Momentum Rate */
C_NNFW_API void NnfwBackPropagationSetMomentum( NnfwLearningAlgorithm* bp, double mom );
/*! Return Learn Rate */
C_NNFW_API double NnfwBackPropagationLearnRate( NnfwLearningAlgorithm* bp );
/*! Return Momentum Rate */
C_NNFW_API double NnfwBackPropagationMomentum( NnfwLearningAlgorithm* bp );
/*! Iterate over PatternSet passed and teach the net */
C_NNFW_API void NnfwBackPropagationLearn( NnfwLearningAlgorithm* bp, NnfwPatternSet* pset );
/*! Calculate the MSE over PatternSet passed */
C_NNFW_API double NnfwBackPropagationCalcMSE( NnfwLearningAlgorithm* bp, NnfwPatternSet* pset );
//@}

/*! \defgroup iocnnfw I/O functions
 *  \ingroup cinterface
 */
//@{
// *! load a neural network from an XML file */
//C_NNFW_API NnfwBaseNeuralNet* NnfwLoadXML( const char* filename );
// *! save the neural network to an XML file */
//C_NNFW_API void NnfwSaveXML( const char* filename, NnfwBaseNeuralNet*, int precision, const char* skipList );
//@}

#ifdef __cplusplus
};
#endif

#endif

