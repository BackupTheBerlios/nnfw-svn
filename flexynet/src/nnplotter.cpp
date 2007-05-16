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

#include "nnplotter.h"
#include "clusterplotter2.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QGLWidget>
#include <QGLFormat>
#include <cmath>

using namespace nnfw;

NNPlotter::NNPlotter( QWidget* parent ): QGraphicsView(parent) {
	QGraphicsScene* scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	setScene(scene);
 	setCacheMode(CacheBackground);
	setDragMode(ScrollHandDrag);
 	setRenderHint(QPainter::Antialiasing);
 	setTransformationAnchor(NoAnchor);
 	setResizeAnchor(NoAnchor);
	//setBackgroundBrush( QColor( 250, 250, 250 ) );
//	setViewport(new QGLWidget( QGLFormat(QGL::SampleBuffers) ) );

	scaleDisabled = true;
	nn = 0;
	setMinimumSize(400, 200);
	setWindowTitle(tr("NNFW Neural Network Plotter"));
}

void NNPlotter::setNeuralNet( FNNWrapper* net ) {
	QList<QGraphicsItem *> list = scene()->items();
	QList<QGraphicsItem *>::Iterator it = list.begin();
	for (; it != list.end(); ++it) {
		if ( *it ) {
			delete *it;
		}
	}
	pls.clear();
	disconnect( nn, SIGNAL( stepped() ),
			this, SLOT( updatePlots() ) );
	this->nn = net;
	if ( net == 0 ) {
		setMatrix( QMatrix() );
		scaleDisabled = true;
		update();
		return;
	}

	const ClusterVec& cls = nn->clusters();
	for( unsigned int i=0; i<cls.size(); i++ ) {
		ClusterPlotter2* cp = new ClusterPlotter2( cls[i] );
		scene()->addItem( cp );
		pls.append( cp );
		connect( cp, SIGNAL( heightChanged() ),
				this, SLOT( updatePositions() ) );
	}
	setMatrix( QMatrix() );
	scaleDisabled = false;
	connect( nn, SIGNAL( stepped() ),
			this, SLOT( updatePlots() ) );
	scene()->update();
}

void NNPlotter::updatePlots() {
	if ( nn == 0 ) {
		return;
	}
	qreal prevoff = 0.0f;
	for( int i=0; i<pls.size(); i++ ) {
		pls[i]->updatePlot();
		pls[i]->setPos( 0, prevoff );
		prevoff += pls[i]->boundingRect().height()+10;
	}
	scene()->setSceneRect( scene()->itemsBoundingRect() );
	scene()->update();
}

void NNPlotter::updatePositions() {
	if ( nn == 0 ) {
		return;
	}
	qreal prevoff = 0.0f;
	for( int i=0; i<pls.size(); i++ ) {
		pls[i]->setPos( 0, prevoff );
		prevoff += pls[i]->boundingRect().height()+10;
	}
	scene()->setSceneRect( scene()->itemsBoundingRect().adjusted(-20,-20,+20,+20) );
	scene()->update();
}


void NNPlotter::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Plus:
		scaleView(1.2);
		break;
	case Qt::Key_Minus:
		scaleView(1 / 1.2);
		break;
     default:
		QGraphicsView::keyPressEvent(event);
	}
}

void NNPlotter::wheelEvent(QWheelEvent *event) {
	scaleView(pow((double)2, -event->delta() / 240.0));
}

void NNPlotter::drawBackground(QPainter *painter, const QRectF &rect) {
	QGraphicsView::drawBackground( painter, rect );
}

void NNPlotter::scaleView( qreal scaleFactor ) {
	if ( scaleDisabled ) {
		return;
	}
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100) {
		return;
	}
	scale(scaleFactor, scaleFactor);
}
