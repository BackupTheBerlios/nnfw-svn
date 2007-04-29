
#include "clusterrenderer.h"
#include "linkerrenderer.h"

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

ClusterRenderer::ClusterRenderer( Cluster* cl ) {
	this->cl = cl;
	dim = (int)cl->size();
	//--- calculate the rectangle
	recw = dim*14;
	rech = 14;
	rectlx = -recw/2;
	rectly = -rech/2;
	setFlag(ItemIsMovable);
	setZValue(1);
}
	
QRectF ClusterRenderer::boundingRect() const {
	return QRectF( rectlx, rectly, recw, rech );
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
	if (option->state & QStyle::State_Sunken) {
		painter->setBrush( Qt::gray );
	} else {
		painter->setBrush( Qt::white );
	}
	painter->setPen(QPen(Qt::black, 1));
	painter->drawRect(rectlx, rectly, recw, rech);
	if ( option->levelOfDetail > 2 ) {
		//--- draw the name
		painter->setPen(QPen(Qt::red, 1));
		painter->drawText( QRect(rectlx, rectly, recw, rech), Qt::AlignHCenter, QString(cl->getName()) );
		//--- draw neurons
		painter->setPen(QPen( Qt::black, 1));
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
	update();
	QGraphicsItem::mousePressEvent(event);
}

void ClusterRenderer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	update();
	QGraphicsItem::mouseReleaseEvent(event);
}

QVariant ClusterRenderer::itemChange(GraphicsItemChange change, const QVariant &value) {
	switch (change) {
	case ItemPositionChange:
		foreach( LinkerRenderer *lr, lks ) {
			lr->adjust();
		}
		emit moved();
		break;
	default:
		break;
	};
	return QGraphicsItem::itemChange(change, value);
}

