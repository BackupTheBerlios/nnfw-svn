
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

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

LinkerRenderer::LinkerRenderer(ClusterRenderer *sourceNode, ClusterRenderer *destNode )
	: path(), arrowSize(10) {
	setAcceptedMouseButtons(0);
	source = sourceNode;
	dest = destNode;
	source->addLinker( this );
	dest->addLinker( this );
	adjust();
}

void LinkerRenderer::adjust() {
	if (!source || !dest) {
		return;
	}
	if ( source == dest ) {
		removeFromIndex();
		//--- it's a self-loop
		QRectF rec = source->boundingRect();
		QPointF sourcePoint = mapFromItem( source, -rec.width()/4.0, -rec.height()/2.0 );
		QPointF destPoint = mapFromItem( source, -rec.width()/4.0, +rec.height()/2.0 );
		QPointF c1  = mapFromItem( source, -rec.width()/2.0, -rec.height()*1.5f );
		QPointF c2  = mapFromItem( source, -rec.width(), -rec.height()*1.5f );
		QPointF mid = mapFromItem( source, -rec.width(), 0 );
		QPointF c3  = mapFromItem( source, -rec.width(), +rec.height()*1.5f );
		QPointF c4  = mapFromItem( source, -rec.width()/2.0, +rec.height()*1.5f );
		path = QPainterPath();
		path.moveTo( sourcePoint );
		path.cubicTo( c1, c2, mid );
		path.cubicTo( c3, c4, destPoint );
		// Draw the arrows if there's enough room
		QLineF line = QLineF(c4, destPoint);
		double angle = ::acos(line.dx() / line.length());
		if (line.dy() >= 0) {
			angle = TwoPi - angle;
		}
		QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
												cos(angle - Pi / 3) * arrowSize);
		QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
												cos(angle - Pi + Pi / 3) * arrowSize);		
		path.addPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2 << line.p2());
		addToIndex();
		return;
	}
	//--- normal condition		
	qreal mstep = 0.01f;
	QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
	qreal ts = 0.0f;
	QRectF r = mapFromItem( source, source->boundingRect() ).boundingRect();
	while( r.contains( line.pointAt( ts ) ) ) {
		ts += mstep;
		if ( ts > 1.0f ) {
			ts = 0.0f;
			break;
		}
	}
	qreal te = 1.0f;
	r = mapFromItem( dest, dest->boundingRect() ).boundingRect();
	while( r.contains( line.pointAt( te ) ) ) {
		te -= mstep;
		if ( te < 0.0f ) {
			te = 1.0f;
			break;
		}
	}
	removeFromIndex();
	QPointF sourcePoint = line.pointAt(ts);
	QPointF destPoint = line.pointAt(te);
	path = QPainterPath();
	path.moveTo( sourcePoint );
	path.lineTo( destPoint );
	// Draw the arrows if there's enough room
	line = QLineF(sourcePoint, destPoint);
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0) {
		angle = TwoPi - angle;
	}
	QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
											cos(angle - Pi / 3) * arrowSize);
	QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
											cos(angle - Pi + Pi / 3) * arrowSize);		
	path.addPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2 << line.p2());
	addToIndex();
}

QRectF LinkerRenderer::boundingRect() const {
	if (!source || !dest) {
		return QRectF();
	}
	qreal penWidth = 1;
	qreal extra = (penWidth + arrowSize) / 2.0;
	return path.boundingRect().adjusted(-extra, -extra, extra, extra);
/*
	if ( source == dest ) {
		//---its a self-loop
		QRectF rec = source->boundingRect();
		QPointF c1  = mapFromItem( source, -rec.width()/2.0, -rec.height()*1.5f );
		QPointF c2  = mapFromItem( source, -rec.width(), -rec.height()*1.5f );
		QPointF mid = mapFromItem( source, -rec.width(), 0 );
		QPointF c3  = mapFromItem( source, -rec.width(), +rec.height()*1.5f );
		QPointF c4  = mapFromItem( source, -rec.width()/2.0, +rec.height()*1.5f );
		QPainterPath path;
		path.moveTo( sourcePoint );
		path.cubicTo( c1, c2, mid );
		path.cubicTo( c3, c4, destPoint );
		return path.boundingRect().adjusted(-extra, -extra, extra, extra);
	}
	return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
										destPoint.y() - sourcePoint.y()) )
		.normalized()
		.adjusted(-extra, -extra, extra, extra);*/
}

void LinkerRenderer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->setBrush(QColor(122, 163, 39));
	painter->drawPath( path );
/*
	if (!source || !dest) {
		return;
	}
	QLineF line;
	if ( source == dest ) {
		//--- it's a self-loop
		QRectF rec = source->boundingRect();
		QPointF c1  = mapFromItem( source, -rec.width()/2.0, -rec.height()*1.5f );
		QPointF c2  = mapFromItem( source, -rec.width(), -rec.height()*1.5f );
		QPointF mid = mapFromItem( source, -rec.width(), 0 );
		QPointF c3  = mapFromItem( source, -rec.width(), +rec.height()*1.5f );
		QPointF c4  = mapFromItem( source, -rec.width()/2.0, +rec.height()*1.5f );
		QPainterPath path;
		path.moveTo( sourcePoint );
		path.cubicTo( c1, c2, mid );
		path.cubicTo( c3, c4, destPoint );
		painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter->drawPath( path );
		//--- angle of Arrow
		line = QLineF( c4, destPoint );
	} else {		
		//--- normal condition
		line = QLineF(sourcePoint, destPoint);
		painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter->drawLine(line);
	}
	
	// Draw the arrows if there's enough room
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0) {
		angle = TwoPi - angle;
	}
	QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
											cos(angle - Pi / 3) * arrowSize);
	QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
											cos(angle - Pi + Pi / 3) * arrowSize);		
	painter->setBrush(Qt::black);
	painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);*/
}

