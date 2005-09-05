
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>

extern "C" {
#include "network.h"
}

using namespace std;

#include "nnfw.h"
#include "simplecluster.h"
#include "matrixlinker.h"
#include "libclusterupdaters.h"

#define NA   ( 27 )
#define ND   ( 8 )

//-------- Neural Network
SimpleCluster *in, *out;
MatrixLinker *l;
BaseNeuralNet* net;

int main( int argc, char* argv[] ) {

    net = new BaseNeuralNet();

    in = new SimpleCluster( 27 );
    out = new SimpleCluster( 8 );

    l = new MatrixLinker( in, out );

    net->addCluster( in );
    net->addCluster( out );

    in->setUpdater( new FakeSigmoidUpdater( 1.0 ) );
    out->setUpdater( new FakeSigmoidUpdater( 1.0 ) );

    net->addLinker( l );

    Updatable* ord[] = { in, out, l };
    net->setOrder( ord, 3 );

    net->randomize( -1.0, 1.0 );

    time_t start, end;

    // Steps !!!
    cout << "Starting ... ";
    time( &start );
    for ( int i = 0; i<1000000; i++ ) {
        net->step();
    }
    time( &end );
    cout << " done in " << difftime( end, start ) << endl;



    // Loading and trying the mint version 0.1
    FILE* archfile;
    struct mint_network* mintNet;
    archfile = fopen( "mint.net", "r" );
    mintNet = mint_network_load( archfile );
    fclose( archfile );

    // Steps !!!
    cout << "Starting Mint ... " ;
    time( &start );
    for ( int i = 0; i<1000000; i++ ) {
        mint_network_nupdate( mintNet );
    }
    time( &end );
    cout << " done in " << difftime( end, start ) << endl;

    return 0;
}
