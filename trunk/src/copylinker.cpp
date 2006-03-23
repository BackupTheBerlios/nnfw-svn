/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005 Gianluca Massera <emmegian@yahoo.it>                     *
 *                                                                              *
 *  This program is free software; you can redistribute it and/or modify        *
 *  it under the terms of the GNU General Public License as published by        *
 *  the Free Software Foundation; either version 2 of the License, or           *
 *  (at your option) any later version.                                         *
 *                                                                              *
 *  This program is distributed in the hope that it will be useful,             *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 *  GNU General Public License for more details.                                *
 *                                                                              *
 *  You should have received a copy of the GNU General Public License           *
 *  along with this program; if not, write to the Free Software                 *
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA  *
 ********************************************************************************/

#include "copylinker.h"
#include "random.h"
#include <cstdio>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/**********************************************
 *  Implementation of CopyLinker Class        *
 **********************************************/

CopyLinker::CopyLinker( Cluster* from, Cluster* to, CopyMode mode, const char* name )
    : Linker(name) {
    if ( from->size() < to->size() ) {
        dimData = from->size();
    } else {
        dimData = to->size(); 
    }
    this->from = from;
    this->to = to;
    // Follow initialization force setMode method to setting datas, otherwise strange automatic compile-time
    // initialization may results in unpredictable behaviour
    this->mode = (CopyMode)-1;
    setMode( mode );
}

CopyLinker::~CopyLinker() {
}

void CopyLinker::setMode( CopyMode cm ) {
    if ( this->mode == cm ) return;

    this->mode = cm;
    switch( mode ) {
    case In2In:
        dataFrom = &Cluster::inputs; //from->inputs; //from->getInputs();
        dataTo = &Cluster::inputs; //to->inputs; //to->getInputs();
        break;
    case In2Out:
        dataFrom = &Cluster::inputs; //from->getInputs();
        dataTo = &Cluster::outputs; //to->getOutputs();
        break;
    case Out2In:
        dataFrom = &Cluster::outputs; //from->getOutputs();
        dataTo = &Cluster::inputs; //to->getInputs();
        break;
    case Out2Out:
        dataFrom = &Cluster::outputs; //from->getOutputs();
        dataTo = &Cluster::outputs; //to->getOutputs();
        break;
    }
    return;
}

CopyLinker::CopyMode CopyLinker::getMode() {
    return mode;
}

void CopyLinker::update() {
    memcpy( (from->*dataFrom)(), (to->*dataTo)(), dimData*sizeof( Real ) );
    return;
}

Cluster* CopyLinker::getFrom() const {
    return from;
}

Cluster* CopyLinker::getTo() const {
    return to;
}

u_int CopyLinker::size() {
    return dimData;
}

}
