
#include "clusterrenderer.h"
#include "linkerrenderer.h"
#include "nnrenderer.h"

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

ClusterRenderer::ClusterRenderer( NNRenderer* nnrenderer, Cluster* cl ) {
	this->cl = cl;
	this->nnr = nnrenderer;
	dim = (int)cl->size();
	//--- calculate the rectangle
	recw = dim*14;
	rech = 14;
	rectlx = -recw/2;
	rectly = -rech/2;
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setZValue(1);
	setToolTip( cl->getName() );
	grabbed = false;
}
	
QRectF ClusterRenderer::boundingRect() const {
	return QRectF( rectlx, rectly, recw, rech ).adjusted( -3, -3, 3, 3 );
}

QPainterPath ClusterRenderer::shape() const {
	QPainterPath path;
	path.addRect(rectlx, rectly, recw, rech);
	return path;
}

void ClusterRenderer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(widget);
	//--- shadow
/*	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::darkGray);
	painter->drawRect(-7, -7, 20, 20);*/
	if ( (option->state & QStyle::State_Sunken) || isSelected() ) {
		painter->setBrush( Qt::gray );
	} else {
		painter->setBrush( Qt::white );
	}
	painter->setPen(QPen(Qt::black, 1));
	painter->drawRect( QRectF(rectlx, rectly, recw, rech).adjusted(-2,-2,2,2) );
	if ( option->levelOfDetail > 2 ) {
		//--- draw neurons
		painter->setPen(QPen( Qt::black, 0.6));
		painter->setBrush( Qt::transparent );
		for( int i=0; i<dim; i++ ) {
			painter->drawEllipse( rectlx+( i*14 )+2, rectly+2, 10, 10 );
		}
	} else {
		//--- draw neurons
		for( int i=0; i<dim; i++ ) {
			painter->drawEllipse( rectlx+( i*14 )+2, rectly+2, 10, 10 );
		}
	}
}

void ClusterRenderer::addLinker( LinkerRenderer* lr ) {
	lks.append( lr );
	lr->adjust();
}

void ClusterRenderer::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	grabbed = true;
	update();
	QGraphicsItem::mousePressEvent(event);
}

void ClusterRenderer::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	if ( !grabbed ) return;
	QGraphicsItem::mouseMoveEvent(event);
	if ( scene() ) {
		QRectF sr = scene()->sceneRect();
		QRectF ir = boundingRect();
		qreal dw = 0.0f;
		qreal dh = 0.0f;
		if ( sr.top() > ir.top() ) {
			dh = qAbs( sr.top() - ir.top() );
		} else if ( sr.bottom() < ir.bottom() ) {
			dh = qAbs( sr.bottom() - ir.bottom() );
		}
		if ( sr.left() > ir.left() ) {
			dw = qAbs( sr.left() - ir.left() );
		} else if ( sr.right() < ir.right() ) {
			dw = qAbs( sr.right() - ir.right() );
		}
		sr.adjust( -dw, -dh, dw, dh );
		scene()->setSceneRect( sr );
	}
	update();
}

void ClusterRenderer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	grabbed = false;
	update();
	QGraphicsItem::mouseReleaseEvent(event);
	if ( scene() ) {
		//--- fit prefectly after movements
		scene()->setSceneRect( scene()->itemsBoundingRect() );
	}
}

QVariant ClusterRenderer::itemChange(GraphicsItemChange change, const QVariant &value) {
	switch (change) {
	case ItemPositionChange: {
		foreach( LinkerRenderer *lr, lks ) {
			lr->adjust();
		}
		break;
	}
	default:
		break;
	};
	return QGraphicsItem::itemChange(change, value);
}

