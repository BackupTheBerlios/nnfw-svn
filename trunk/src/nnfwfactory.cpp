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
#include "matrixlinker.h"

namespace nnfw {

class SimpleClusterCreator : public ClusterCreator {
public:
    virtual Cluster* create( u_int numNeurons, const char* name ) const {
        return (new SimpleCluster( numNeurons, name ));
    };
    virtual Cluster* create( u_int numNeurons, const CreatorParameters& ) const {
        return (new SimpleCluster( numNeurons ));
    };
    virtual ClusterCreator* clone() const {
        return (new SimpleClusterCreator(*this));
    };
};

class BiasedClusterCreator : public ClusterCreator {
public:
    virtual Cluster* create( u_int numNeurons, const char* name ) const {
        return (new BiasedCluster( numNeurons, name ));
    };
    virtual Cluster* create( u_int numNeurons, const CreatorParameters& ) const {
        return (new BiasedCluster( numNeurons ));
    };
    virtual ClusterCreator* clone() const {
        return (new BiasedClusterCreator(*this));
    };
};

class DDEClusterCreator : public ClusterCreator {
public:
    virtual Cluster* create( u_int numNeurons, const char* name ) const {
        return (new DDECluster( RealVec(), numNeurons, name ));
    };
    virtual Cluster* create( u_int numNeurons, const CreatorParameters& ) const {
        return (new DDECluster( RealVec(), numNeurons ));
    };
    virtual ClusterCreator* clone() const {
        return (new DDEClusterCreator(*this));
    };
};

class MatrixLinkerCreator : public LinkerCreator {
public:
    virtual Linker* create( Cluster* from, Cluster* to, const char* name ) const {
        return (new MatrixLinker( from, to, name ));
    };
    virtual Linker* create( Cluster* from, Cluster* to, const CreatorParameters& ) const {
        return (new MatrixLinker( from, to ));
    };
    virtual LinkerCreator* clone() const {
        return (new MatrixLinkerCreator(*this));
    };
};

Cluster* Factory::createCluster( const char* type, u_int numNeurons, const char* name ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( clustertypes.count( key ) ) {
        return clustertypes[key]->create( numNeurons, name );
    }
    return 0;
}

Linker* Factory::createLinker( const char* type, Cluster* from, Cluster* to, const char* name ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if( linkertypes.count( key ) ) {
        return linkertypes[key]->create( from, to, name );
    }
    return 0;
}

bool Factory::registerCluster( const ClusterCreator& c, const char* type ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( clustertypes.count( key ) == 0 ) {
        clustertypes[key] = c.clone();
        return true;
    }
    return false;
}

bool Factory::registerLinker( const LinkerCreator& c, const char* type ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( linkertypes.count( key ) == 0 ) {
        linkertypes[key] = c.clone();
        return true;
    }
    return false;
}

void Factory::initFactory() {
    clustertypes["SimpleCluster"] = new SimpleClusterCreator();
    clustertypes["BiasedCluster"] = new BiasedClusterCreator();
    clustertypes["DDECluster"] = new BiasedClusterCreator();
    linkertypes["MatrixLinker"] = new MatrixLinkerCreator();
    isInit = true;
}

bool Factory::isInit = false;
std::map<std::string, ClusterCreator*> Factory::clustertypes;
std::map<std::string, LinkerCreator*> Factory::linkertypes;

}

