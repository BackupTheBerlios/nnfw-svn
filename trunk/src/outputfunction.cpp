
#include "outputfunction.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

void OutputFunction::apply( RealVec& inputs, RealVec& outputs ) {
    outputs.assign( inputs );
}

OutputFunction* OutputFunction::clone() const {
    return new OutputFunction();
}

}
