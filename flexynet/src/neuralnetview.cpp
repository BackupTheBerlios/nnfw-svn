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

#include "neuralnetview.h"
#include "graphicslinker.h"
#include "graphicscluster.h"
#include "baseneuralnetmodel.h"
#include "clustermodel.h"
#include "linkermodel.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QEvent>
#include <QRect>
#include <cmath>

NeuralNetView::NeuralNetView( QWidget* parent ): QGraphicsView(parent), clmap(), lkmap() {
	QGraphicsScene* scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	setScene(scene);
	setDragMode(RubberBandDrag);
 	setRenderHint(QPainter::Antialiasing);
	setBackgroundBrush( QColor( 250, 240, 230 ) );
	setViewportUpdateMode( FullViewportUpdate );
	scaleDisabled = true;
	setMinimumSize(400, 200);
	setWindowTitle(tr("NNFW Neural Network View"));
	connect( scene, SIGNAL( changed( const QList<QRectF>& ) ),
			this, SLOT( adjustSceneRect() ) );
	nn = 0;
}

void NeuralNetView::setNeuralNet( BaseNeuralNetModel* net ) {
	QList<QGraphicsItem *> list = scene()->items();
	QList<QGraphicsItem *>::Iterator it = list.begin();
	for (; it != list.end(); ++it) {
		if ( *it ) {
			delete *it;
		}
	}
	clmap.clear();
	lkmap.clear();
	if ( nn != 0 ) {
		disconnect( nn, SIGNAL( clusterAdded( ClusterModel*, bool, bool ) ),
				this, SLOT( addGraphicsCluster( ClusterModel*, bool, bool ) ) );
		disconnect( nn, SIGNAL( linkerAdded( LinkerModel* ) ),
				this, SLOT( addGraphicsLinker( LinkerModel* ) ) );
	}
	this->nn = net;
	if ( net == 0 ) {
		setMatrix( QMatrix() );
		scaleDisabled = true;
		scene()->update();
		return;
	}
	QVector<ClusterModel*> cls = nn->clusters();
	QVector<ClusterModel*> ins = nn->inputClusters();
	QVector<ClusterModel*> outs = nn->outputClusters();
	for( int i=0; i<cls.size(); i++ ) {
		addGraphicsCluster( cls[i], ins.contains(cls[i]), outs.contains(cls[i]) );
	}

	QVector<LinkerModel*> lks = nn->linkers();
	for( int i=0; i<lks.size(); i++ ) {
		addGraphicsLinker( lks[i] );
	}
	setMatrix( QMatrix() );
	scaleDisabled = false;
	defaultPositioning();
	scene()->update();
	connect( nn, SIGNAL( clusterAdded( ClusterModel*, bool, bool ) ),
			this, SLOT( addGraphicsCluster( ClusterModel*, bool, bool ) ) );
	connect( nn, SIGNAL( linkerAdded( LinkerModel* ) ),
			this, SLOT( addGraphicsLinker( LinkerModel* ) ) );
}

void NeuralNetView::defaultPositioning() {
	if ( nn == 0 ) {
		return;
	}
	float xspace = 15;
	float yspace = 70;
	float xpos = 0;
	float ypos = 0;
	QVector<ClusterModel*> cls = nn->inputClusters();
	QVector<GraphicsCluster*> lays;
	for( int i=0; i<cls.size(); i++ ) {
		lays.append( clmap[cls[i]] );
	}
	// --- start to set positions
	bool ended = false;
	int idstart = 0;
	int idend = lays.size();
	while( !ended ) {
		ended = true;
		for( int i=idstart; i<idend; i++ ) {
			GraphicsCluster* r = lays[i];
			QRectF rect = r->boundingRect();
			r->setPos( xpos+rect.width()/2.0f, ypos+rect.height()/2.0f );
			xpos += rect.width()+xspace;
			// add to next layers
			QVector<LinkerModel*> lks = nn->linkers( clmap.key(lays[i]), true );
			for( int j=0; j<lks.size(); j++ ) {
				GraphicsCluster* tor = clmap[lks[j]->to()];
				if ( lays.indexOf( tor ) != -1 ) continue;
				lays.append( tor );
				ended = false;
			}
		}
		xpos = 0;
		ypos -= yspace;
		idstart = idend;
		idend = lays.size();
	}
	QList<GraphicsLinker*> lsr = lkmap.values();
	for( int i=0; i<lsr.size(); i++ ) {
		lsr[i]->adjust();
	};
	scene()->setSceneRect( scene()->itemsBoundingRect() );
}

void NeuralNetView::keyPressEvent(QKeyEvent *event) {
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

void NeuralNetView::adjustSceneRect() {
	//--- fit prefectly after movements
	QPointF oldcentre = scene()->sceneRect().center();
	QRectF newrect = scene()->itemsBoundingRect();
	QPointF actcentre = newrect.center();
	qreal dx1 = qMax( 0.0, actcentre.x() - oldcentre.x() );
	qreal dx2 = qMax( 0.0, oldcentre.x() - actcentre.x() );
	qreal dy1 = qMax( 0.0, actcentre.y() - oldcentre.y() );
	qreal dy2 = qMax( 0.0, oldcentre.y() - actcentre.y() );
	newrect.adjust( -dx1*2, -dy1*2, dx2*2, dy2*2 );
	scene()->setSceneRect( newrect );
}

void NeuralNetView::wheelEvent(QWheelEvent *event) {
	scaleView(pow((double)2, -event->delta() / 240.0));
}

void NeuralNetView::drawBackground(QPainter *painter, const QRectF &rect) {
	QGraphicsView::drawBackground( painter, rect );

// 	painter->setBrush(Qt::black);
// 	painter->drawLine( -30, 0, +30, 0 );
// 	painter->drawLine( 0, -30, 0, +30 );

	// draw SceneRect
/*	painter->setBrush(Qt::transparent);
	QRectF sceneRect = this->sceneRect();
	painter->drawRect( sceneRect );*/
	
	// Shadow
// 	QRectF sceneRect = this->sceneRect();
// 	QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
// 	QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
// 	if (rightShadow.intersects(rect) || rightShadow.contains(rect))
// 		painter->fillRect(rightShadow, Qt::darkGray);
// 	if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
// 		painter->fillRect(bottomShadow, Qt::darkGray);

}

void NeuralNetView::scaleView( qreal scaleFactor ) {
	if ( scaleDisabled ) {
		return;
	}
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.1 || factor > 10) {
		return;
	}
	scale(scaleFactor, scaleFactor);
}

void NeuralNetView::addGraphicsCluster( ClusterModel* cl, bool in, bool out ) {
	//--- may be those should be attribute of NeuralNetView class (or not?!?!)
	static int xpos = -170;
	static int ypos = -170;
	GraphicsCluster* gc = new GraphicsCluster( cl );
	gc->markAsInput( in );
	gc->markAsOutput( out );
	scene()->addItem( gc );
	gc->setPos( xpos, ypos );
	gc->setZValue( 1 );
	xpos += 30;
	ypos += 30;
	clmap[ cl ] = gc;
}

void NeuralNetView::delGraphicsCluster( ClusterModel* cl ) {
	GraphicsCluster* gc = clmap[cl];
	delete gc;
}

void NeuralNetView::addGraphicsLinker( LinkerModel* lk ) {
	GraphicsCluster* src = clmap[ lk->from() ];
	GraphicsCluster* dst = clmap[ lk->to() ];
	GraphicsLinker* gl = new GraphicsLinker( lk, src, dst );
	scene()->addItem( gl );
	gl->setZValue( 2 );
	lkmap[ lk ] = gl;
}

void NeuralNetView::delGraphicsLinker( LinkerModel* lk ) {
	GraphicsLinker* gl = lkmap[lk];
	delete gl;
}

