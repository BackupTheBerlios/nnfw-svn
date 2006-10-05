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

#include "nnfwfactory.h"
#include "simplecluster.h"
#include "biasedcluster.h"
#include "ddecluster.h"
#include "fakecluster.h"
#include "matrixlinker.h"
#include "sparsematrixlinker.h"
#include "copylinker.h"

namespace nnfw {

Cluster* Factory::createCluster( const char* type, PropertySettings& p ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( clustertypes.count( key ) ) {
        return (Cluster*)( clustertypes[key]->create( p ) );
    }
    return 0;
}

Linker* Factory::createLinker( const char* type, PropertySettings& p ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if( linkertypes.count( key ) ) {
        return (Linker*)( linkertypes[key]->create( p ) );
    }
    return 0;
}

bool Factory::registerCluster( const AbstractCreator& c, const char* type ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( clustertypes.count( key ) == 0 ) {
        clustertypes[key] = c.clone();
        return true;
    }
    return false;
}

bool Factory::registerLinker( const AbstractCreator& c, const char* type ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( linkertypes.count( key ) == 0 ) {
        linkertypes[key] = c.clone();
        return true;
    }
    return false;
}

void Factory::initFactory() {
    clustertypes["SimpleCluster"] = new Creator<SimpleCluster>();
    clustertypes["BiasedCluster"] = new Creator<BiasedCluster>();
    clustertypes["DDECluster"] = new Creator<DDECluster>();
    clustertypes["FakeCluster"] = new Creator<FakeCluster>();
    linkertypes["MatrixLinker"] = new Creator<MatrixLinker>();
    linkertypes["SparseMatrixLinker"] = new Creator<SparseMatrixLinker>();
    linkertypes["CopyLinker"] = new Creator<CopyLinker>();
    isInit = true;
}

bool Factory::isInit = false;
std::map<std::string, AbstractCreator*> Factory::clustertypes;
std::map<std::string, AbstractCreator*> Factory::linkertypes;

}

