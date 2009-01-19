/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>                *
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
class NNFW_INTERNAL DummyModifier : public AbstractModifier {
public:
	/*! \name Interface */
	//@{
	/*! apply the rule changing the Updatable object */
	virtual void rule( double, const RealVec&, const RealVec& ) const { /* nothing to do */ };
	/*! Virtual Copy-Constructor */
	virtual DummyModifier* clone() const {
		return new DummyModifier();
	};
	//@}
};

/*! \brief BiasedClusterModifier */
class NNFW_INTERNAL BiasedClusterModifier : public AbstractModifier {
public:
	/*! \name Interface */
	//@{
	/*! set the learnable object */
	virtual void setUpdatable( Updatable* tolearn ) {
		learnable = tolearn;
		cl = (BiasedCluster*)tolearn;
	};
	/*! apply the rule changing the Updatable object */
	virtual void rule( double learn_rate, const RealVec& x, const RealVec& y ) const {
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

/*! \brief MatrixLinkerModifier */
class NNFW_INTERNAL MatrixLinkerModifier : public AbstractModifier {
public:
	/*! \name Interface */
	//@{
	/*! set the learnable object */
	virtual void setUpdatable( Updatable* tolearn ) {
		learnable = tolearn;
		ml = (MatrixLinker*)tolearn;
	};
	/*! apply the rule changing the Updatable object */
	virtual void rule( double learn_rate, const RealVec& x, const RealVec& y ) const {
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

/*! \brief SparseMatrixLinkerModifier */
class NNFW_INTERNAL SparseMatrixLinkerModifier : public AbstractModifier {
public:
	/*! \name Interface */
	//@{
	/*! set the learnable object */
	virtual void setUpdatable( Updatable* tolearn ) {
		learnable = tolearn;
		sml = (SparseMatrixLinker*)tolearn;
	};
	/*! apply the rule changing the Updatable object */
	virtual void rule( double learn_rate, const RealVec& x, const RealVec& y ) const {
		sml->matrix().deltarule( learn_rate, x, y );
		sml->matrix().cover( sml->mask() );
	};
	/*! Virtual Copy-Constructor */
	virtual SparseMatrixLinkerModifier* clone() const {
		return new SparseMatrixLinkerModifier();
	};
	//@}
private:
	SparseMatrixLinker* sml;
};

AbstractModifier* Factory::createModifierFor( Updatable* objectToLearn ) {
	if ( !isInit ) { initFactory(); };
	QString key = objectToLearn->typeName();
	if ( modtypes.contains( key ) ) {
		AbstractModifier* ret = modtypes[key]->clone();
		ret->setUpdatable( objectToLearn );
		return ret;
	}
	return 0;
}

bool Factory::registerModifier( const AbstractModifier& m, const char* type ) {
	if ( !isInit ) { initFactory(); };
	QString key(type);
	if ( modtypes.count( key ) == 0 ) {
		modtypes[key] = m.clone();
		return true;
	}
	return false;
}

void Factory::initFactory() {
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
QMap<QString, AbstractModifier*> Factory::modtypes;

}

