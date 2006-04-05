/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2006 Gianluca Massera <emmegian@yahoo.it>                     *
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

#include "ddecluster.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

DDECluster::DDECluster( const RealVec& c, u_int numNeurons, const char* name )
    : Cluster( numNeurons, name ) {
    setCoeff( c );
}

virtual DDECluster::~DDECluster() {
    /* Nothing to do */
}

void DDECluster::setCoeff( const RealVec& c ) {
    coeff.resize( c.size() );
    coeff.assign( c );
    story.resize( );
}

void DDECluster::update() {
    /* Complicato da Implementare !! :-) */
}

}

