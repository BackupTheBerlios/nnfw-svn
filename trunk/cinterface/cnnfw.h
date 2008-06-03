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
// avoid to export C++ data
#define NNFW_API
#define NNFW_TEMPLATE

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

/*! \name NnfwCluster Operations */
//@{
/*! Create a Simple Cluster */
C_NNFW_API NnfwCluster* NnfwClusterCreateSimple( unsigned int numNeurons );
/*! Create a Biased Cluster */
C_NNFW_API NnfwCluster* NnfwClusterCreateBiased( unsigned int numNeurons );
/*! Create a Fake Cluster */
C_NNFW_API NnfwCluster* NnfwClusterCreateFake( unsigned int numNeurons );
/*! Return the number of Neurons in the Cluster */
C_NNFW_API unsigned int NnfwClusterNumNeurons( NnfwCluster* );
/*! Set Accumulate mode
 * \param accumulate if 0 disable accumulate mode, otherwise enable it
 */
C_NNFW_API void NnfwClusterSetAccumulate( NnfwCluster*, int accumulate );
/*! Return 1 if accumulate is enabled, 0 otherwise */
C_NNFW_API int NnfwClusterIsAccumulate( NnfwCluster* );
/*! Randomize the parameters of Cluster within min and max values specified */
C_NNFW_API void NnfwClusterRandomize( Real min, Real max );
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
/*! */
C_NNFW_API NnfwCluster* NnfwClusterClone( NnfwCluster* );
//@}

#ifdef __cplusplus
};
#endif

#endif

