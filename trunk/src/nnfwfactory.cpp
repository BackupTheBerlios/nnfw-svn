/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                *
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
#include "libperiodicfunctions.h"
#include "libcompetitivefunctions.h"

namespace nnfw {

/*! \brief Dummy Modifier ... or in other words that don't modify anything
 *
 *  \par Motivation
 *  Updatable object as SimpleCluster needs a Modifier in order to have a complet map factory
 *  and avoid checks
 */
class DummyModifier : public AbstractModifier {
public:
    /*! \name Interface */
    //@{

    /*! apply the rule changing the Updatable object */
    virtual void rule( Real, const RealVec&, const RealVec& ) const { /* nothing to do */ };

    /*! Virtual Copy-Constructor */
    virtual DummyModifier* clone() const {
		return new DummyModifier();
	};
    //@}
};

/*! \brief BiasedClusterModifier
 */
class BiasedClusterModifier : public AbstractModifier {
public:
    /*! \name Interface */
    //@{

	/*! set the learnable object */
	virtual void setUpdatable( Updatable* tolearn ) {
		learnable = tolearn;
		cl = (BiasedCluster*)tolearn;
	};

    /*! apply the rule changing the Updatable object */
    virtual void rule( Real learn_rate, const RealVec& x, const RealVec& y ) const {
		cl->biases().deltarule( learn_rate, x, y );
	};

    /*! Virtual Copy-Constructor */
    virtual BiasedClusterModifier* clone() const {
		return new BiasedClusterModifier();
	};
    //@}
private:
	BiasedCluster* cl;
};

/*! \brief MatrixLinkerModifier
 */
class MatrixLinkerModifier : public AbstractModifier {
public:
    /*! \name Interface */
    //@{

	/*! set the learnable object */
	virtual void setUpdatable( Updatable* tolearn ) {
		learnable = tolearn;
		ml = (MatrixLinker*)tolearn;
	};

    /*! apply the rule changing the Updatable object */
    virtual void rule( Real learn_rate, const RealVec& x, const RealVec& y ) const {
		ml->matrix().deltarule( learn_rate, x, y );
	};

    /*! Virtual Copy-Constructor */
    virtual MatrixLinkerModifier* clone() const {
		return new MatrixLinkerModifier();
	};
    //@}
private:
	MatrixLinker* ml;
};

/*! \brief SparseMatrixLinkerModifier
 */
class SparseMatrixLinkerModifier : public AbstractModifier {
public:
    /*! \name Interface */
    //@{

	/*! set the learnable object */
	virtual void setUpdatable( Updatable* tolearn ) {
		learnable = tolearn;
		sml = (SparseMatrixLinker*)tolearn;
	};

    /*! apply the rule changing the Updatable object */
    virtual void rule( Real learn_rate, const RealVec& x, const RealVec& y ) const {
		sml->matrix().deltarule( learn_rate, x, y );
		sml->matrix().cover( sml->getMask() );
	};

    /*! Virtual Copy-Constructor */
    virtual SparseMatrixLinkerModifier* clone() const {
		return new SparseMatrixLinkerModifier();
	};
    //@}
private:
	SparseMatrixLinker* sml;
};

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
		proptypes[key] = clustertypes[key];
        return true;
    }
    return false;
}

bool Factory::registerLinker( const AbstractCreator& c, const char* type ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( linkertypes.count( key ) == 0 ) {
        linkertypes[key] = c.clone();
		proptypes[key] = linkertypes[key];
        return true;
    }
    return false;
}

bool Factory::registerOutputFunction( const AbstractCreator& c, const char* type ) {
    if ( !isInit ) { initFactory(); };
    std::string key(type);
    if ( outfuntypes.count( key ) == 0 ) {
        outfuntypes[key] = c.clone();
		proptypes[key] = outfuntypes[key];
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

AbstractModifier* Factory::createModifierFor( Updatable* objectToLearn ) {
	if ( !isInit ) { initFactory(); };
	std::string key( objectToLearn->getTypename().getString() );
	if ( modtypes.count( key ) ) {
		AbstractModifier* ret = modtypes[key]->clone();
		ret->setUpdatable( objectToLearn );
		return ret;
	}
	return 0;
}

bool Factory::registerModifier( const AbstractModifier& m, const char* type ) {
	if ( !isInit ) { initFactory(); };
	std::string key(type);
	if ( modtypes.count( key ) == 0 ) {
		modtypes[key] = m.clone();
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
	outfuntypes["RampFunction"] = new Creator<RampFunction>();
	outfuntypes["ScaleFunction"] = new Creator<ScaleFunction>();
	outfuntypes["ScaledSigmoidFunction"] = new Creator<ScaledSigmoidFunction>();
	outfuntypes["SigmoidFunction"] = new Creator<SigmoidFunction>();
	outfuntypes["StepFunction"] = new Creator<StepFunction>();
	outfuntypes["LeakyIntegratorFunction"] = new Creator<LeakyIntegratorFunction>();
	outfuntypes["PoolFunction"] = new Creator<PoolFunction>();
	outfuntypes["CompositeFunction"] = new Creator<CompositeFunction>();
	outfuntypes["LinearComboFunction"] = new Creator<LinearComboFunction>();
	outfuntypes["GaussFunction"] = new Creator<GaussFunction>();
	outfuntypes["PseudoGaussFunction"] = new Creator<PseudoGaussFunction>();
	outfuntypes["SawtoothFunction"] = new Creator<SawtoothFunction>();
	outfuntypes["SinFunction"] = new Creator<SinFunction>();
	outfuntypes["TriangleFunction"] = new Creator<TriangleFunction>();
	outfuntypes["WinnerTakeAllFunction"] = new Creator<WinnerTakeAllFunction>();

	// Replicate all data above in generic Propertized
	std::map<std::string, AbstractCreator*>::iterator it;
	for( it = clustertypes.begin(); it!=clustertypes.end(); it++ ) {
		proptypes[ (*it).first ] = (*it).second;
	}
	for( it = linkertypes.begin(); it!=linkertypes.end(); it++ ) {
		proptypes[ (*it).first ] = (*it).second;
	}
	for( it = outfuntypes.begin(); it!=outfuntypes.end(); it++ ) {
		proptypes[ (*it).first ] = (*it).second;
	}

	// --- Standard Modifiers
	modtypes["SimpleCluster"] = new DummyModifier();
	modtypes["BiasedCluster"] = new BiasedClusterModifier();
	modtypes["DDECluster"] = new DummyModifier();
	modtypes["FakeCluster"] = new DummyModifier();
	modtypes["SparseMatrixLinker"] = new SparseMatrixLinkerModifier();
	modtypes["CopyLinker"] = new DummyModifier();
	modtypes["DotLinker"] = new MatrixLinkerModifier();
	modtypes["NormLinker"] = new MatrixLinkerModifier();
	modtypes["MatrixLinker"] = new MatrixLinkerModifier();

    isInit = true;
}

bool Factory::isInit = false;
std::map<std::string, AbstractCreator*> Factory::clustertypes;
std::map<std::string, AbstractCreator*> Factory::linkertypes;
std::map<std::string, AbstractCreator*> Factory::outfuntypes;
std::map<std::string, AbstractCreator*> Factory::proptypes;
std::map<std::string, AbstractModifier*> Factory::modtypes;

}

