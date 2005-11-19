
#include "clusterupdater.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

void ClusterUpdater::update( Real* inputs, Real* outputs, u_int numNeuron ) {
    for( u_int i = 0; i<numNeuron; i++ ) {
        update( inputs[i], outputs[i] );
    }
}

void ClusterUpdater::update( Real input, Real &output ) {
        output = input;
}

const char* ClusterUpdater::className() const {
    return "ClusterUpdater";
}

}
