
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>

using namespace std;

#include "nnfw.h"
#include "simplecluster.h"
#include "matrixlinker.h"
#include "libclusterupdaters.h"

using namespace nnfw;

#define IN   ( 8 )
#define OUT   ( 3 )


//-------- Neural Network
SimpleCluster *in, *out;
MatrixLinker *l;
BaseNeuralNet* net;

//-------- Vectors for testing
nnfwReal inputs[ IN ];
nnfwReal outputs[ OUT ];
nnfwReal biasesIn[ IN ];
nnfwReal biasesOut[ OUT ];
nnfwReal weights[ IN ][ OUT ];

void randomize() {
    // Change the inputs
    for ( int i = 0; i<in->size(); i++ ) {
        nnfwReal v = ( rand()%2000 - 1000.0 )/1000.0;
        in->setInput( i, v );
        inputs[ i ] = v;
    }

    // Change the biases
    for ( int i = 0; i<in->size(); i++ ) {
        nnfwReal v = ( rand()%2000 - 1000.0 )/1000.0;
        in->setBias( i, v );
        biasesIn[ i ] = v;
    }
    for ( int i = 0; i<out->size(); i++ ) {
        nnfwReal v = ( rand()%2000 - 1000.0 )/1000.0;
        out->setBias( i, v );
        biasesOut[ i ] = v;
    }

    // Changing the Weights
    for ( int i = 0; i<l->getRows(); i++ ) {
        for ( int j = 0; j<l->getCols(); j++ ) {
            nnfwReal v = ( rand()%2000 - 1000.0 )/1000.0;
            l->setWeight( i, j, v );
            weights[i][j] = v;
        }
    }
}

double testingNet() {
    // Neural Network Step
    net->step();

    // Testing calculation
    // --- zeroing the outputs
    for ( int i = 0; i<OUT; i++ ) {
        outputs[i] = 0.0;
    }
    // --- appling the sigmoid function on inputs
    nnfwReal finputs[ IN ];
    for ( int i = 0; i<IN; i++ ) {
        finputs[i] = 1.0/( 1.0 + exp( - inputs[i] + biasesIn[i] ) );
    }

    // --- multiply the inputs with weights matrix
    for ( int i = 0; i<OUT; i++ ) {
        for ( int j = 0; j<IN; j++ ) {
            outputs[i] += finputs[j]*weights[j][i];
        }
    }

    // --- appling the sigmoid function on outputs
    for ( int i = 0; i<OUT; i++ ) {
        outputs[i] = 1.0/( 1.0 + exp( -outputs[i] + biasesOut[i] ) );
    }

    // --- Calculate the error between the calculations
    nnfwReal error = 0.0;
    for ( int i = 0; i<OUT; i++ ) {
        error += out->getOutput( i )/outputs[i] ;
    }
    return fabs( 1.0 - error/OUT ) ;
}

int main( int argc, char* argv[] ) {

    net = new BaseNeuralNet();
    
    in = new SimpleCluster( IN );
    out = new SimpleCluster( OUT );

    l = new MatrixLinker( in, out );

    net->addCluster( in );
    net->addCluster( out );

    net->addLinker( l );

    Updatable* ord[] = { in, l, out };
    net->setOrder( ord, 3 );

    srand( time( 0 ) );

    randomize();

    cout.precision( 10 );
    double errCum = 0.0;
    int i;
    for ( i = 0; i<1; i++ ) {
        randomize();
        errCum += testingNet();
    }
    cout << "True Sigmoid -> Error after " << i << " iteration: " << errCum << " Avg: " << errCum/i << endl;

    FakeSigmoidUpdater f = FakeSigmoidUpdater( 1.0 );
    in->setUpdater( new FakeSigmoidUpdater( 1.0 ) );
    out->setUpdater( new FakeSigmoidUpdater( 1.0 ) );

    cout.precision( 10 );
    errCum = 0.0;
    for ( i = 0; i<1; i++ ) {
        randomize();
        errCum += testingNet();
    }
    cout << "Fake Sigmoid -> Error after " << i << " iteration: " << errCum << " Avg: " << errCum/i << endl;

    return 0;
}
