
#include "clusterupdater.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

void ClusterUpdater::update( RealVec& inputs, RealVec& outputs ) {
    outputs.assign( inputs );
}

void ClusterUpdater::update( Real input, Real &output ) {
    output = input;
}

ClusterUpdater* ClusterUpdater::clone() const {
    return new ClusterUpdater();
}

}
