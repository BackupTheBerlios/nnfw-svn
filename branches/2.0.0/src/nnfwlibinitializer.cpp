/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2011 Gianluca Massera <emmegian@yahoo.it>                *
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

#include "nnfw.h"
#include "simplecluster.h"
#include "biasedcluster.h"
#include "ddecluster.h"
#include "fakecluster.h"
#include "copylinker.h"
#include "dotlinker.h"
#include "normlinker.h"
#include "liboutputfunctions.h"
#include "libperiodicfunctions.h"
#include "libradialfunctions.h"
#include "libcompetitivefunctions.h"
#include <factory/factory.h>

using namespace nnfw;

#ifdef NNFW_LINUX
void initNNFWLib() __attribute__ ((constructor));
void initNNFWLib() {
#ifdef NNFW_DEBUG
	qDebug() << "NNFW Initialization";
#endif
	// Registering all types to factory
	Factory::getInstance().registerClass<NeuralNet>("NeuralNet");
	// Clusters
	Factory::getInstance().registerClass<SimpleCluster>("SimpleCluster");
	Factory::getInstance().registerClass<BiasedCluster>("BiasedCluster");
	Factory::getInstance().registerClass<DDECluster>("DDECluster");
	Factory::getInstance().registerClass<FakeCluster>("FakeCluster");

	// Linkers
	Factory::getInstance().registerClass<CopyLinker>("CopyLinker");
	Factory::getInstance().registerClass<DotLinker>("DotLinker", QStringList() << "MatrixLinker"); // "MatrixLinker" is for backward compatibility
	Factory::getInstance().registerClass<NormLinker>("NormLinker");

	// Output Functions
	Factory::getInstance().registerClass<FakeSigmoidFunction>("FakeSigmoidFunction");
	Factory::getInstance().registerClass<IdentityFunction>("IdentityFunction");
	Factory::getInstance().registerClass<GainFunction>("GainFunction");
	Factory::getInstance().registerClass<LinearFunction>("LinearFunction");
	Factory::getInstance().registerClass<RampFunction>("RampFunction");
	Factory::getInstance().registerClass<ScaleFunction>("ScaleFunction");
	Factory::getInstance().registerClass<ScaledSigmoidFunction>("ScaledSigmoidFunction");
	Factory::getInstance().registerClass<SigmoidFunction>("SigmoidFunction");
	Factory::getInstance().registerClass<StepFunction>("StepFunction");
	Factory::getInstance().registerClass<LeakyIntegratorFunction>("LeakyIntegratorFunction");
	Factory::getInstance().registerClass<LogLikeFunction>("LogLikeFunction");
	Factory::getInstance().registerClass<CompositeFunction>("CompositeFunction");
	Factory::getInstance().registerClass<LinearComboFunction>("LinearComboFunction");
	Factory::getInstance().registerClass<GaussFunction>("GaussFunction");
	Factory::getInstance().registerClass<PseudoGaussFunction>("PseudoGaussFunction");
	Factory::getInstance().registerClass<SawtoothFunction>("SawtoothFunction");
	Factory::getInstance().registerClass<SinFunction>("SinFunction");
	Factory::getInstance().registerClass<TriangleFunction>("TriangleFunction");
	Factory::getInstance().registerClass<WinnerTakeAllFunction>("WinnerTakeAllFunction");
}
#endif
#ifdef NNFW_WIN
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved ) {
	// Perform actions based on the reason for calling.
	switch( fdwReason ) {
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
#ifdef NNFW_DEBUG
		qDebug() << "NNFW Initialization";
#endif
		// Registering all types to factory
		Factory::getInstance().registerClass<NeuralNet>("NeuralNet");
		// Clusters
		Factory::getInstance().registerClass<SimpleCluster>("SimpleCluster");
		Factory::getInstance().registerClass<BiasedCluster>("BiasedCluster");
		Factory::getInstance().registerClass<DDECluster>("DDECluster");
		Factory::getInstance().registerClass<FakeCluster>("FakeCluster");

		// Linkers
		Factory::getInstance().registerClass<CopyLinker>("CopyLinker");
		Factory::getInstance().registerClass<DotLinker>("DotLinker", QStringList() << "MatrixLinker"); // "MatrixLinker" is for backward compatibility
		Factory::getInstance().registerClass<NormLinker>("NormLinker");

		// Output Functions
		Factory::getInstance().registerClass<FakeSigmoidFunction>("FakeSigmoidFunction");
		Factory::getInstance().registerClass<IdentityFunction>("IdentityFunction");
		Factory::getInstance().registerClass<GainFunction>("GainFunction");
		Factory::getInstance().registerClass<LinearFunction>("LinearFunction");
		Factory::getInstance().registerClass<RampFunction>("RampFunction");
		Factory::getInstance().registerClass<ScaleFunction>("ScaleFunction");
		Factory::getInstance().registerClass<ScaledSigmoidFunction>("ScaledSigmoidFunction");
		Factory::getInstance().registerClass<SigmoidFunction>("SigmoidFunction");
		Factory::getInstance().registerClass<StepFunction>("StepFunction");
		Factory::getInstance().registerClass<LeakyIntegratorFunction>("LeakyIntegratorFunction");
		Factory::getInstance().registerClass<LogLikeFunction>("LogLikeFunction");
		Factory::getInstance().registerClass<CompositeFunction>("CompositeFunction");
		Factory::getInstance().registerClass<LinearComboFunction>("LinearComboFunction");
		Factory::getInstance().registerClass<GaussFunction>("GaussFunction");
		Factory::getInstance().registerClass<PseudoGaussFunction>("PseudoGaussFunction");
		Factory::getInstance().registerClass<SawtoothFunction>("SawtoothFunction");
		Factory::getInstance().registerClass<SinFunction>("SinFunction");
		Factory::getInstance().registerClass<TriangleFunction>("TriangleFunction");
		Factory::getInstance().registerClass<WinnerTakeAllFunction>("WinnerTakeAllFunction");
		break;
	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;
	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;
	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}
	return TRUE;
}
#endif
#ifdef NNFW_MAC
void initNNFWLib() __attribute__ ((constructor));
void initNNFWLib() {
#ifdef NNFW_DEBUG
	qDebug() << "NNFW Initialization";
#endif
	// Registering all types to factory
	Factory::getInstance().registerClass<NeuralNet>("NeuralNet");
	// Clusters
	Factory::getInstance().registerClass<SimpleCluster>("SimpleCluster");
	Factory::getInstance().registerClass<BiasedCluster>("BiasedCluster");
	Factory::getInstance().registerClass<DDECluster>("DDECluster");
	Factory::getInstance().registerClass<FakeCluster>("FakeCluster");

	// Linkers
	Factory::getInstance().registerClass<CopyLinker>("CopyLinker");
	Factory::getInstance().registerClass<DotLinker>("DotLinker", QStringList() << "MatrixLinker"); // "MatrixLinker" is for backward compatibility
	Factory::getInstance().registerClass<NormLinker>("NormLinker");

	// Output Functions
	Factory::getInstance().registerClass<FakeSigmoidFunction>("FakeSigmoidFunction");
	Factory::getInstance().registerClass<IdentityFunction>("IdentityFunction");
	Factory::getInstance().registerClass<GainFunction>("GainFunction");
	Factory::getInstance().registerClass<LinearFunction>("LinearFunction");
	Factory::getInstance().registerClass<RampFunction>("RampFunction");
	Factory::getInstance().registerClass<ScaleFunction>("ScaleFunction");
	Factory::getInstance().registerClass<ScaledSigmoidFunction>("ScaledSigmoidFunction");
	Factory::getInstance().registerClass<SigmoidFunction>("SigmoidFunction");
	Factory::getInstance().registerClass<StepFunction>("StepFunction");
	Factory::getInstance().registerClass<LeakyIntegratorFunction>("LeakyIntegratorFunction");
	Factory::getInstance().registerClass<LogLikeFunction>("LogLikeFunction");
	Factory::getInstance().registerClass<CompositeFunction>("CompositeFunction");
	Factory::getInstance().registerClass<LinearComboFunction>("LinearComboFunction");
	Factory::getInstance().registerClass<GaussFunction>("GaussFunction");
	Factory::getInstance().registerClass<PseudoGaussFunction>("PseudoGaussFunction");
	Factory::getInstance().registerClass<SawtoothFunction>("SawtoothFunction");
	Factory::getInstance().registerClass<SinFunction>("SinFunction");
	Factory::getInstance().registerClass<TriangleFunction>("TriangleFunction");
	Factory::getInstance().registerClass<WinnerTakeAllFunction>("WinnerTakeAllFunction");
}
#endif

