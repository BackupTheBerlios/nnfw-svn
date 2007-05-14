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

#include "fbrowser.h"
#include <QTreeWidget>
#include <QBoxLayout>
#include <QStringList>
#include <QFont>

using namespace nnfw;

FBrowser::FBrowser( QWidget* parent )
	: QWidget( parent ), net(0) {

	QBoxLayout* lay = new QVBoxLayout( this );
	tree = new QTreeWidget( this );
	lay->addWidget( tree );

	// ---\ Name \ Type \---
	tree->setColumnCount( 2 );
	tree->setHeaderLabels( QStringList() << "Name" << "Type" );

	rootCluster = new QTreeWidgetItem( tree, QStringList() << "Clusters" << "" );
	QFont f = QFont(rootCluster->font(0));
	f.setBold( true );
	rootCluster->setFont( 0, f );
	rootLinker = new QTreeWidgetItem( tree, QStringList() << "Linkers" << "" );
	rootLinker->setFont( 0, f );

}

void FBrowser::setNeuralNet( FNNWrapper* nn ) {
	tree->clear();
	rootCluster = new QTreeWidgetItem( tree, QStringList() << "Clusters" << "" );
	QFont f = QFont(rootCluster->font(0));
	f.setBold( true );
	rootCluster->setFont( 0, f );
	rootLinker = new QTreeWidgetItem( tree, QStringList() << "Linkers" << "" );
	rootLinker->setFont( 0, f );
	if ( nn == 0 ) {
		net = 0;
		return;
	}
	net = nn;
	const ClusterVec& cls = net->clusters();
	for( unsigned int i=0; i<cls.size(); i++ ) {
		QTreeWidgetItem* item = new QTreeWidgetItem( rootCluster, QStringList() << cls[i]->getName() << cls[i]->getTypename().getString() );
	}
	const LinkerVec& lks = nn->linkers();
	for( unsigned int i=0; i<lks.size(); i++ ) {
		QTreeWidgetItem* item = new QTreeWidgetItem( rootLinker, QStringList() << lks[i]->getName() << lks[i]->getTypename().getString() );
	}
}

