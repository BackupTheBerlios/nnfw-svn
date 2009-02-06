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

#include "ddecluster.h"

namespace nnfw {

DDECluster::DDECluster( const DoubleVector& c, unsigned int numNeurons, QString name )
	: Cluster( numNeurons, name ), tmpdata(numNeurons), tmpdata2(numNeurons) {
	setCoeff( c );
}

DDECluster::~DDECluster() {
	/* Nothing to do */
}

void DDECluster::setCoeff( const DoubleVector& c ) {
	coeff.resize( c.size() );
	coeff.copy( c );
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
	getFunction()->apply( inputs(), tmpdata2 );
	tmpdata2 *= coeff[1];
	tmpdata += tmpdata2;
	if ( csize == 2 ) {
		breakUpdate();
		return;
	}
	// --- tmp <- a0 + a1*f(x) + a2*x
	tmpdata2.copy( inputs() );
	tmpdata2 *= coeff[2];
	tmpdata += tmpdata2;
	if ( csize == 3 ) {
		breakUpdate();
		return;
	}
	// --- tmp <- a0 + a1*f(x) + a2*x + a3*y ... aN*y^(n-3)
	for( int i=0; i<ds.size(); i++ ) {
		tmpdata2.copy( ds[i] );
		tmpdata2 *= coeff[i+3];
		tmpdata += tmpdata2;
	}
	breakUpdate();
	return;
}

void DDECluster::breakUpdate() {
	outputs().copy( tmpdata );
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
		ds[0].copy( outputs() );
	} else if ( ds.size() == 2 ) {
		minus( ds[1], outputs(), ds[0] );
		ds[0].copy( outputs() );
	} else {
		// ----- Problemi di questo tipo di calcolo:
		// *** calcola, cmq, anche la derivata ds.size()+1... l'ultimo tmpdata calcolato prima di uscire.
		tmpdata.copy( outputs() );
		for( int i=0; i<ds.size(); i++ ) {
			if ( i%2 == 0 ) {
				// calcola la derivata i+1 per il ciclo successivo
				minus( tmpdata2, tmpdata, ds[1] );
				// memorizza il valore della derivata i calcolata al ciclo precedente i-1
				ds[i].copy( tmpdata );
			} else {
				// calcola la derivata i+1 per il ciclo successivo
				minus( tmpdata, tmpdata2, ds[i] );
				// memorizza il valore della derivata i calcolata al ciclo precedente i-1
				ds[i].copy( tmpdata2 );
			}
		}
	}
}

DDECluster* DDECluster::clone() const {
	DDECluster* newclone = new DDECluster( coeff, numNeurons(), name() );
	newclone->setAccumulate( this->isAccumulate() );
	newclone->inputs().copy( this->inputs() );
	newclone->outputs().copy( this->outputs() );
	newclone->setFunction( *(this->getFunction()) );
	return newclone;
}

}

