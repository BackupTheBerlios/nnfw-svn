
#include "nnfw.h"
#include "biasedcluster.h"
#include "dotlinker.h"
#include "liboutputfunctions.h"
#include "backpropagationalgo.h"
#include "randomgenerator.h"

using namespace nnfw;

//-------- Neural Network Structures
BiasedCluster *in, *hid, *out;
DotLinker *l1, *l2;
NeuralNet* net;

int main( int , char** ) {
	globalRNG->setSeed( time(0) );

	net = new NeuralNet();

	// --- Create the Layers of network
	in = new BiasedCluster( 2 );
	in->setOutFunction( new SigmoidFunction( 1.0f ) );
	hid = new BiasedCluster( 4 );
	hid->setOutFunction( new SigmoidFunction( 1.0f ) );
	out = new BiasedCluster( 1 );
	out->setOutFunction( new SigmoidFunction( 1.0f ) );

	// --- Create the Matrix connection among layers
	l1 = new DotLinker( in, hid );
	l2 = new DotLinker( hid, out );

	// --- Add all in the BaseNeuralNet class
	net->addCluster( in, true );
	net->addCluster( hid );
	net->addCluster( out, false, true );
	net->addLinker( l1 );
	net->addLinker( l2 );
	// --- Specify the order on which layers and matrix weight will be updated
	UpdatableList ord;
	net->setOrder( ord << in << l1 << hid << l2 << out );

	// --- Randomize the parameters of network (biases of neuron's layers and weight of matrix linkers)
	net->randomize( -1.0, 1.0 );

	UpdatableList bp_ord;
	bp_ord << out << l2 << hid << l1 << in;
	BackPropagationAlgo* bp = new BackPropagationAlgo( net, bp_ord, 0.2 );

	// --- The learning Set
	PatternSet learningSet(4);
	// --- Input <0,0> -> Output <0>
	learningSet[0].setInputsOf( in, DoubleVector() << 0.0 << 0.0 );
	learningSet[0].setOutputsOf( out, DoubleVector() << 0.0 );
	// --- Input <0,1> -> Output <1>
	learningSet[1].setInputsOf( in, DoubleVector() << 0.0 << 1.0 );
	learningSet[1].setOutputsOf( out, DoubleVector() << 1.0 );
	// --- Input <1,0> -> Output <1>
	learningSet[2].setInputsOf( in, DoubleVector() << 1.0 << 0.0 );
	learningSet[2].setOutputsOf( out, DoubleVector() << 1.0 );
	// --- Input <1,1> -> Output <0>
	learningSet[3].setInputsOf( in, DoubleVector() << 1.0 << 1.0 );
	learningSet[3].setOutputsOf( out, DoubleVector() << 0.0 );

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
	for( int i = 0; i<4; i++ ) {
		in->inputs().copyValues( learningSet[i].inputsOf( in ) );
		net->step();
		double out1 = out->getOutput(0);
		double out2 = learningSet[i].outputsOf( out )[0];
		qDebug() << "Inputs: " << in->inputs()[0] << in->inputs()[1] << "Target: " << out2 << "\tRete: " << out1;
	}

	return 0;
}
