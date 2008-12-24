
#include <stdio.h>
#include "nnfw/cnnfw.h"

//-------- Neural Network Structures
NnfwCluster *in, *hid, *out;
NnfwLinker *l1, *l2;
NnfwBaseNeuralNet* net;
NnfwLearningAlgorithm* bp;

int main( int , char*  ) {

    //Random::setSeed( time(0) );

    net = NnfwBaseNeuralNetCreate();

    // --- Create the Layers of network
	NnfwOutputFunction* sig1 = NnfwOutputFunctionCreateSigmoid( 1.0 );
	in = NnfwClusterCreateBiased( 2 );
	NnfwClusterSetFunction( in, sig1 );
    hid = NnfwClusterCreateBiased( 4 );
	NnfwClusterSetFunction( hid, sig1 );
    out = NnfwClusterCreateBiased( 1 );
	NnfwClusterSetFunction( out, sig1 );

    // --- Create the Matrix connection among layers
    l1 = NnfwLinkerCreateDot( in, hid );
    l2 = NnfwLinkerCreateDot( hid, out );

    // --- Add all in the BaseNeuralNet class
	NnfwBaseNeuralNetAddInputCluster( net, in );
	NnfwBaseNeuralNetAddHiddenCluster( net, hid );
	NnfwBaseNeuralNetAddOutputCluster( net, out );
	NnfwBaseNeuralNetAddLinker( net, l1 );
	NnfwBaseNeuralNetAddLinker( net, l2 );
    // --- Specify the order on which layers and matrix weight will be updated
	NnfwBaseNeuralNetSetOrder( net, 5, in, l1, hid, l2, out );

    // --- Randomize the parameters of network (biases of neuron's layers and weight of matrix linkers)
	NnfwBaseNeuralNetRandomize( net, -1.0, +1.0 );

	bp = NnfwBackPropagationCreate( net, 0.2, 5, out, l2, hid, l1, in );

    // --- The learning Set 
	NnfwPatternSet* set = NnfwPatternSetCreate( 4 );
	double buf[2];
	// --- Input <0,0> -> Output <0>
	buf[0] = 0.0; buf[1] = 0.0;
	NnfwPatternSetSetInputsOf( set, 0, in, buf );
	buf[0] = 0.0;
	NnfwPatternSetSetOutputsOf( set, 0, out, buf );
	// --- Input <0,1> -> Output <1>
	buf[0] = 0.0; buf[1] = 1.0;
	NnfwPatternSetSetInputsOf( set, 1, in, buf );
	buf[0] = 1.0;
	NnfwPatternSetSetOutputsOf( set, 1, out, buf );
	// --- Input <1,0> -> Output <1>
	buf[0] = 1.0; buf[1] = 0.0;
	NnfwPatternSetSetInputsOf( set, 2, in, buf );
	buf[0] = 1.0;
	NnfwPatternSetSetOutputsOf( set, 2, out, buf );
	// --- Input <1,1> -> Output <0>
	buf[0] = 1.0; buf[1] = 1.0;
	NnfwPatternSetSetInputsOf( set, 3, in, buf );
	buf[0] = 0.0;
	NnfwPatternSetSetOutputsOf( set, 3, out, buf );

    // --- Main loop for learning the network
	int i;
    for( i = 0; i<50000; i++ ) {
		NnfwBackPropagationLearn( bp, set );
		// --- each 1000 iteration print out the error
        if ( i%1000 == 0 ) {
			printf( "Error: %g\n", NnfwBackPropagationCalcMSE( bp, set ) );
        }
    }
	printf( "Iterations: %d\tError: %g\n", i, NnfwBackPropagationCalcMSE( bp, set ) );
    // --- compare the outputs with learning set
    for( int i = 0; i<4; i++ ) {
		NnfwClusterSetInputs( in, NnfwPatternSetGetInputsOf( set, i, in ) );
		NnfwBaseNeuralNetStep( net );
		printf( "Target: %g\tRete:%g\n", NnfwPatternSetGetOutputsOf( set, i, out)[0], NnfwClusterOutputs( out )[0] );
    }
    return 0;
}
