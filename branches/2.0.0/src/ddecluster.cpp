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

#include "ddecluster.h"

namespace nnfw {

DDECluster::DDECluster( const DoubleVector& c, unsigned int numNeurons, QString name )
	: Cluster( numNeurons, name ), tmpdata(numNeurons), tmpdata2(numNeurons) {
	setCoeff( c );
}

DDECluster::DDECluster( ConfigurationParameters& params, QString prefix )
	: Cluster( params, prefix ), tmpdata(numNeurons()), tmpdata2(numNeurons()) {
	// coefficients is a vector, that is a list of space-separated values
	QString str = params.getValue(prefix + "coefficients");
	if (!str.isNull()) {
		QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		coeff.resize( list.size() );
		for( int i=0; i<list.size(); i++) {
			bool ok;
			coeff[i] = list[i].toDouble(&ok);
			if (!ok) {
				coeff[i] = 0.0;
			}
		}
	}
}

DDECluster::~DDECluster() {
	/* Nothing to do */
}

void DDECluster::setCoeff( const DoubleVector& c ) {
	coeff = c;
	ds.resize( (c.size()>3) ? c.size()-3 : 0 );
	for( int i=0; i<ds.size(); i++ ) {
		ds[i].resize( numNeurons() );
		ds[i].zeroing();
	}
}

void DDECluster::update() {
	unsigned int csize = coeff.size();
	if ( csize == 0 ) {
		// uscita un po' strana!
		outputs().zeroing();
		setNeedReset( true );
		return;
	}
	// --- tmp <- a0
	tmpdata.setAll( coeff[0] );
	if ( csize == 1 ) {
		breakUpdate();
		return;
	}
	// --- tmp <- a0 + a1*f(x)
	outFunction()->apply( inputs(), tmpdata2 );
	tmpdata2 *= coeff[1];
	tmpdata += tmpdata2;
	if ( csize == 2 ) {
		breakUpdate();
		return;
	}
	// --- tmp <- a0 + a1*f(x) + a2*x
	tmpdata2.copyValues( inputs() );
	tmpdata2 *= coeff[2];
	tmpdata += tmpdata2;
	if ( csize == 3 ) {
		breakUpdate();
		return;
	}
	// --- tmp <- a0 + a1*f(x) + a2*x + a3*y ... aN*y^(n-3)
	for( int i=0; i<ds.size(); i++ ) {
		tmpdata2.copyValues( ds[i] );
		tmpdata2 *= coeff[i+3];
		tmpdata += tmpdata2;
	}
	breakUpdate();
	return;
}

void DDECluster::breakUpdate() {
	outputs().copyValues( tmpdata );
	updateDs();
	setNeedReset( true );
}

void DDECluster::updateDs() {
	if ( ds.size() == 0 ) return;
	// --- Le derivate sono semplici differenze:
	// --- y'  = y(t) - y(t-1)
	// --- y'' = y'(t) - y'(t-1)
	// ---  ... and so on
	if ( ds.size() == 1 ) {
		ds[0].copyValues( outputs() );
	} else if ( ds.size() == 2 ) {
		subtract( ds[1], outputs(), ds[0] );
		ds[0].copyValues( outputs() );
	} else {
		// ----- Problemi di questo tipo di calcolo:
		// *** calcola, cmq, anche la derivata ds.size()+1... l'ultimo tmpdata calcolato prima di uscire.
		tmpdata.copyValues( outputs() );
		for( int i=0; i<ds.size(); i++ ) {
			if ( i%2 == 0 ) {
				// calcola la derivata i+1 per il ciclo successivo
				subtract( tmpdata2, tmpdata, ds[1] );
				// memorizza il valore della derivata i calcolata al ciclo precedente i-1
				ds[i].copyValues( tmpdata );
			} else {
				// calcola la derivata i+1 per il ciclo successivo
				subtract( tmpdata, tmpdata2, ds[i] );
				// memorizza il valore della derivata i calcolata al ciclo precedente i-1
				ds[i].copyValues( tmpdata2 );
			}
		}
	}
}

void DDECluster::save(ConfigurationParameters& params, QString prefix)
{
	Cluster::save( params, prefix );
	params.startObjectParameters(prefix, "DDECluster", this);
	// First creating a string list, then transforming to a single string
	QStringList list;
	for (unsigned int i = 0; i < coeff.size(); i++) {
		list.push_back(QString::number(coeff[i]));
	}
	params.createParameter(prefix, "coefficients", list.join(" "));
}

}

