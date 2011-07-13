/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2011 Gianluca Massera <emmegian@yahoo.it>                *
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

#include "neuralnet.h"
#include "learningalgorithm.h"
#include "factory/factory.h"

namespace nnfw {

void Pattern::setInputsOf( Cluster* cl, const DoubleVector& ins ) {
	pinfo[cl].inputs.resize( cl->numNeurons() );
	pinfo[cl].inputs.copyValues( ins );
};

void Pattern::setOutputsOf( Cluster* cl, const DoubleVector& ous ) {
	pinfo[cl].outputs.resize( cl->numNeurons() );
	pinfo[cl].outputs.copyValues( ous );
};

void Pattern::setInputsOutputsOf( Cluster* cl, const DoubleVector& ins, const DoubleVector& ous ) {
	pinfo[cl].inputs.resize( cl->numNeurons() );
	pinfo[cl].inputs.copyValues( ins );
	pinfo[cl].outputs.resize( cl->numNeurons() );
	pinfo[cl].outputs.copyValues( ous );
};

DoubleVector Pattern::inputsOf( Cluster* cl ) const {
	if ( pinfo.count(cl) != 0 ) {
		return pinfo[cl].inputs;
	} else {
		return DoubleVector();
	}
};

DoubleVector Pattern::outputsOf( Cluster* cl ) const {
	if ( pinfo.count(cl) != 0 ) {
		return pinfo[cl].outputs;
	} else {
		return DoubleVector();
	}
};

Pattern::PatternInfo& Pattern::operator[]( Cluster* cl ) {
	return pinfo[cl];
};

void Pattern::configure(ConfigurationParameters& params, QString prefix) {
	//--- get all parameters with the prefix 'cluster:'
	QStringList clusterList = params.getParametersWithPrefixList( prefix, "cluster:" );
	foreach( QString cluster, clusterList ) {
		QString id = cluster.split(':')[1];
		if ( id.isNull() || id.isEmpty() ) continue;
		//--- now, it check if there is a inputs and outputs parameter and load it
		QString str = params.getValue( prefix + "inputs:" + id );
		DoubleVector inputs;
		if (!str.isNull()) {
			QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			for( int i=0; i<list.size(); i++) {
				inputs.append( list[i].toDouble() );
			}
		}
		str = params.getValue( prefix + "outputs:" + id );
		DoubleVector outputs;
		if (!str.isNull()) {
			QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			for( int i=0; i<list.size(); i++) {
				outputs.append( list[i].toDouble() );
			}
		}
		if ( inputs.size() == 0 && outputs.size() == 0 ) continue;
		Cluster* cl = params.getObjectFromParameter<Cluster>( prefix+cluster, false, true );
		if ( inputs.size() > 0 ) {
			setInputsOf( cl, inputs );
		}
		if ( outputs.size() > 0 ) {
			setOutputsOf( cl, outputs );
		}
	}
}

void Pattern::save(ConfigurationParameters& params, QString prefix) {
	params.startObjectParameters(prefix, "Pattern", this);
	QString tmpl = "%1:%2";
	QList<Cluster*> cls = pinfo.keys();
	for( int i=0; i<cls.size(); i++ ) {
		PatternInfo& info = pinfo[ cls[i] ];
		params.createParameter(prefix, tmpl.arg("cluster").arg(i), cls[i]);
		if ( info.inputs.size() > 0 ) {
			QStringList list;
			for( unsigned int j=0; j<info.inputs.size(); j++ ) {
				list.push_back(QString::number(info.inputs[j]));
			}
			params.createParameter(prefix, tmpl.arg("inputs").arg(i), list.join(" "));
		}
		if ( info.outputs.size() > 0 ) {
			QStringList list;
			for( unsigned int j=0; j<info.outputs.size(); j++ ) {
				list.push_back(QString::number(info.outputs[j]));
			}
			params.createParameter(prefix, tmpl.arg("outputs").arg(i), list.join(" "));
		}
	}
}

LearningAlgorithm::LearningAlgorithm( NeuralNet* net )
	: ParameterSettableWithConfigureFunction() {
	this->netp = net;
}

LearningAlgorithm::LearningAlgorithm()
	: ParameterSettableWithConfigureFunction() {
	this->netp = NULL;
}

LearningAlgorithm::~LearningAlgorithm() {
}

PatternSet LearningAlgorithm::loadPatternSet( ConfigurationParameters& params, QString path, QString prefix ) {
	params.startRememberingGroupObjectAssociations();
	Factory& factory = Factory::getInstance();
	QVector<Pattern*> patternSetPtr = factory.createVectorFromParameter<Pattern>( params, path, prefix );
	//--- convert to PatternSet
	PatternSet patternSet;
	foreach( Pattern* pattern, patternSetPtr ) {
		patternSet << *pattern;
	}
	params.stopRememberingGroupObjectAssociations();
	return patternSet;
}

void LearningAlgorithm::savePatternSet( PatternSet& set, ConfigurationParameters& params, QString prefix ) {
	QString tmpl = prefix+":%1";
	for( int i=0; i<set.size(); i++ ) {
		QString group = tmpl.arg(i);
		params.createGroup( group );
		set[i].save( params, group );
	}
}

}
