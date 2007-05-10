/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
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
#include "dotlinker.h"
#include "normlinker.h"
#include "copylinker.h"
#include "outputfunction.h"
#include "liboutputfunctions.h"
#include "libradialfunctions.h"

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

OutputFunction* Factory::createOutputFunction( const char* type, PropertySettings& p ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if( outfuntypes.count( key ) ) {
        return (OutputFunction*)( outfuntypes[key]->create( p ) );
    }
    return 0;
}

Propertized* Factory::createPropertized( const char* type, PropertySettings& p ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if( proptypes.count( key ) ) {
        return ( proptypes[key]->create( p ) );
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

bool Factory::registerOutputFunction( const AbstractCreator& c, const char* type ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( outfuntypes.count( key ) == 0 ) {
        outfuntypes[key] = c.clone();
        return true;
    }
    return false;
}

bool Factory::registerPropertized( const AbstractCreator& c, const char* type ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( proptypes.count( key ) == 0 ) {
        proptypes[key] = c.clone();
        return true;
    }
    return false;
}

void Factory::initFactory() {
    clustertypes["SimpleCluster"] = new Creator<SimpleCluster>();
    clustertypes["BiasedCluster"] = new Creator<BiasedCluster>();
    clustertypes["DDECluster"] = new Creator<DDECluster>();
    clustertypes["FakeCluster"] = new Creator<FakeCluster>();

    linkertypes["SparseMatrixLinker"] = new Creator<SparseMatrixLinker>();
    linkertypes["CopyLinker"] = new Creator<CopyLinker>();
    linkertypes["DotLinker"] = new Creator<DotLinker>();
    linkertypes["NormLinker"] = new Creator<NormLinker>();
	//--- for backward compatibility
    linkertypes["MatrixLinker"] = new Creator<DotLinker>();

    outfuntypes["FakeSigmoidFunction"] = new Creator<FakeSigmoidFunction>();
    outfuntypes["IdentityFunction"] = new Creator<IdentityFunction>();
    outfuntypes["LinearFunction"] = new Creator<LinearFunction>();
    outfuntypes["ScaledSigmoidFunction"] = new Creator<ScaledSigmoidFunction>();
    outfuntypes["SigmoidFunction"] = new Creator<SigmoidFunction>();
    outfuntypes["StepFunction"] = new Creator<StepFunction>();
	outfuntypes["LeakyIntegratorFunction"] = new Creator<LeakyIntegratorFunction>();
    outfuntypes["PoolFunction"] = new Creator<PoolFunction>();
	outfuntypes["CompositeFunction"] = new Creator<CompositeFunction>();
    outfuntypes["GaussFunction"] = new Creator<GaussFunction>();

    isInit = true;
}

bool Factory::isInit = false;
std::map<std::string, AbstractCreator*> Factory::clustertypes;
std::map<std::string, AbstractCreator*> Factory::linkertypes;
std::map<std::string, AbstractCreator*> Factory::outfuntypes;
std::map<std::string, AbstractCreator*> Factory::proptypes;

}

