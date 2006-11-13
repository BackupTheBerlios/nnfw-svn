
#include "outputfunction.h"


namespace nnfw {

OutputFunction::OutputFunction()
    : Clonable(), Propertized(), tmp1(1), tmp2(1) {
    /* Nothing else to do */
    setTypename( "OutputFunction" );
}

OutputFunction::~OutputFunction() {
    /* Nothing to do */
}

void OutputFunction::apply( RealVec& inputs, RealVec& outputs ) {
    outputs.assign( inputs );
}

OutputFunction* OutputFunction::clone() const {
    return new OutputFunction();
}

}
