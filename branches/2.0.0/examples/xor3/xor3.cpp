
#include "nnfw.h"
#include "biasedcluster.h"
#include "dotlinker.h"
#include "liboutputfunctions.h"
#include "backpropagationalgo.h"
#include "randomgenerator.h"
#include "configuration/configurationparameters.h"

using namespace nnfw;

//-------- Neural Network Structures used in the code
BiasedCluster *in, *out;
NeuralNet* net;

int main( int , char** ) {
	globalRNG->setSeed( time(0) );

	ConfigurationParameters params(true);
	params.loadParameters( "net.xml" );
	BackPropagationAlgo* bp = params.getObjectFromGroup<BackPropagationAlgo>( "BackPropagation" );
	PatternSet learningSet = bp->loadPatternSet( params, "LearningSet", "Pattern" );

	// --- Randomize the parameters of network (biases of neuron's layers and weight of matrix linkers)
	net = bp->neuralNet();
	net->randomize( -1.0, 1.0 );
	
	// --- Main loop for learning the network
	int i;
	for( i = 0; i<50000; i++ ) {
		bp->learnOnSet( learningSet );
		// --- each 1000 iteration print out the error
		if ( i%1000 == 0 ) {
			qDebug() << "Error: " << bp->calculateMSEOnSet( learningSet );
		}
	}
	qDebug() << "Iterations: " << i << "\tError:" << bp->calculateMSEOnSet( learningSet );

	// --- compare the outputs with learning set
	net->byName( "input", in );
	net->byName( "output", out );
	for( int i = 0; i<4; i++ ) {
		in->inputs().copyValues( learningSet[i].inputsOf( in ) );
		net->step();
		double out1 = out->getOutput(0);
		double out2 = learningSet[i].outputsOf( out )[0];
		qDebug() << "Inputs: " << in->inputs()[0] << in->inputs()[1] << "Target: " << out2 << "\tRete: " << out1;
	}
	
	ConfigurationParameters paramsSave(true);
	bp->save( paramsSave, "BackPropagation" );
	bp->savePatternSet( learningSet, paramsSave, "LearningSet/Pattern" );
	paramsSave.saveParameters( "netSave.xml" );
	paramsSave.saveParameters( "netSave.ini" );

	return 0;
}
