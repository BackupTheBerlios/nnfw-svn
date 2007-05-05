
#include "nnrenderer.h"
#include "linkerrenderer.h"
#include "clusterrenderer.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <cmath>

using namespace nnfw;

NNRenderer::NNRenderer( QWidget* parent ): QGraphicsView(parent), clmap(), lkmap() {
	QGraphicsScene* scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	setScene(scene);
 	setCacheMode(CacheBackground);
	setDragMode(RubberBandDrag);
 	setRenderHint(QPainter::Antialiasing);
 	setTransformationAnchor(NoAnchor);
 	setResizeAnchor(NoAnchor);
	setBackgroundBrush( QColor( 250, 240, 230 ) );

	scaleDisabled = true;
	setMinimumSize(400, 400);
	setWindowTitle(tr("NNFW Neural Network Renderer"));
}

void NNRenderer::setNeuralNet( BaseNeuralNet* net ) {
	QList<QGraphicsItem *> list = scene()->items();
	QList<QGraphicsItem *>::Iterator it = list.begin();
	for (; it != list.end(); ++it) {
		if ( *it ) {
			delete *it;
		}
	}
	scene()->setSceneRect( QRectF() );
	this->nn = net;
	if ( net == 0 ) {
		setMatrix( QMatrix() );
		scaleDisabled = true;
		update();
		return;
	}
	const ClusterVec& cls = nn->clusters();
	int xpos = -170; int ypos = -170;
	for( unsigned int i=0; i<cls.size(); i++ ) {
		ClusterRenderer* cs = new ClusterRenderer( this, cls[i] );
		scene()->addItem( cs );
		cs->setPos( xpos, ypos );
		xpos += 30;
		ypos += 30;
		clmap[ cls[i] ] = cs;
	}

	const LinkerVec& lks = nn->linkers();
	for( unsigned int i=0; i<lks.size(); i++ ) {
		ClusterRenderer* src = clmap[ lks[i]->from() ];
		ClusterRenderer* dst = clmap[ lks[i]->to() ];
		LinkerRenderer* lr = new LinkerRenderer( src, dst );
		scene()->addItem( lr );
		lkmap[ lks[i] ] = lr;
	}
	setMatrix( QMatrix() );
	scaleDisabled = false;
	defaultPositioning();
	update();
}

void NNRenderer::defaultPositioning() {
	if ( nn == 0 ) {
		return;
	}
	float xspace = 15;
	float yspace = 70;
	float xpos = 0;
	float ypos = 0;
	const ClusterVec& cls = nn->inputClusters();
	QVector<ClusterRenderer*> lays;
	for( unsigned int i=0; i<cls.size(); i++ ) {
		lays.append( clmap[cls[i]] );
	}
	// --- start to set positions
	bool ended = false;
	int idstart = 0;
	int idend = lays.size();
	while( !ended ) {
		ended = true;
		for( int i=idstart; i<idend; i++ ) {
			ClusterRenderer* r = lays[i];
			QRectF rect = r->boundingRect();
			r->setPos( xpos+rect.width()/2.0f, ypos+rect.height()/2.0f );
			xpos += rect.width()+xspace;
			// add to next layers
			const LinkerVec& lks = nn->linkers( clmap.key(lays[i]), true );
			for( unsigned int j=0; j<lks.size(); j++ ) {
				ClusterRenderer* tor = clmap[lks[j]->to()];
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
	QList<LinkerRenderer*> lsr = lkmap.values();
	for( int i=0; i<lsr.size(); i++ ) {
		lsr[i]->adjust();
	};
	scene()->setSceneRect( scene()->itemsBoundingRect() );
}

void NNRenderer::keyPressEvent(QKeyEvent *event) {
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

void NNRenderer::wheelEvent(QWheelEvent *event) {
	scaleView(pow((double)2, -event->delta() / 240.0));
}

void NNRenderer::drawBackground(QPainter *painter, const QRectF &rect) {
	QGraphicsView::drawBackground( painter, rect );

	painter->setBrush(Qt::black);
	painter->drawLine( -30, 0, +30, 0 );
	painter->drawLine( 0, -30, 0, +30 );

	// draw SceneRect
/*	painter->setBrush(Qt::transparent);
	QRectF sceneRect = this->sceneRect();
	painter->drawRect( sceneRect );*/
	
/*	// Shadow
	QRectF sceneRect = this->sceneRect();
	QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
	QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
	if (rightShadow.intersects(rect) || rightShadow.contains(rect))
		painter->fillRect(rightShadow, Qt::darkGray);
	if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
		painter->fillRect(bottomShadow, Qt::darkGray);*/

}

void NNRenderer::scaleView( qreal scaleFactor ) {
	if ( scaleDisabled ) {
		return;
	}
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100) {
		return;
	}
	scale(scaleFactor, scaleFactor);
}
