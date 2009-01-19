
#include "nnfw/nnfw.h"
#include "nnfw/biasedcluster.h"
#include "nnfw/sparsematrixlinker.h"
#include "nnfw/liboutputfunctions.h"
#include "nnfw/backpropagationalgo.h"
#include "nnfw/random.h"
#include "nnfw/utils.h"

using namespace nnfw;

//-------- Neural Network Structures
BiasedCluster *in, *hid, *out;
SparseMatrixLinker *l1, *l2;
BaseNeuralNet* net;

int main( int , char** ) {
	Random::setSeed( time(0) );

	net = new BaseNeuralNet();

	// --- Create the Layers of network
	in = new BiasedCluster( 2 );
	in->setFunction( SigmoidFunction( 1.0f ) );
	hid = new BiasedCluster( 12 );
	hid->setFunction( SigmoidFunction( 1.0f ) );
	out = new BiasedCluster( 1 );
	out->setFunction( SigmoidFunction( 1.0f ) );

	// --- Create the Matrix connection among layers
	l1 = new SparseMatrixLinker( 0.7, in, hid );
	l2 = new SparseMatrixLinker( 0.7, hid, out );

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
	learningSet[0].setInputsOf( in, RealVec() << 0.0 << 0.0 );
	learningSet[0].setOutputsOf( out, RealVec() << 0.0 );
	// --- Input <0,1> -> Output <1>
	learningSet[1].setInputsOf( in, RealVec() << 0.0 << 1.0 );
	learningSet[1].setOutputsOf( out, RealVec() << 1.0 );
	// --- Input <1,0> -> Output <1>
	learningSet[2].setInputsOf( in, RealVec() << 1.0 << 0.0 );
	learningSet[2].setOutputsOf( out, RealVec() << 1.0 );
	// --- Input <1,1> -> Output <0>
	learningSet[3].setInputsOf( in, RealVec() << 1.0 << 1.0 );
	learningSet[3].setOutputsOf( out, RealVec() << 0.0 );

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
		in->inputs() = learningSet[i].inputsOf( in );
		net->step();
		double out1 = out->getOutput(0);
		double out2 = learningSet[i].outputsOf( out )[0];
		qDebug() << "Target: " << out2 << "\tRete: " << out1;
	}

	// Print out Weights
	for( int i=0; i<(int)l1->rows(); i++ ) {
		for( int j=0; j<(int)l1->cols(); j++ ) {
			double w = l1->getWeight( i, j );
			if ( w == 0 ) {
				qDebug() << "FALSE, ";
			} else {
				qDebug() << w << ", ";
			}
		}
	}
	qDebug() << "-----------";
	for( int i=0; i<(int)l2->rows(); i++ ) {
		for( int j=0; j<(int)l2->cols(); j++ ) {
			double w = l2->getWeight( i, j );
			if ( w == 0 ) {
				qDebug() << "FALSE, ";
			} else {
				qDebug() << w << ", ";
			}
		}
	}
	return 0;
}
