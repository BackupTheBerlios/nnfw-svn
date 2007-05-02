
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
	QRectF srcRect = mapFromItem( source, source->boundingRect() ).boundingRect();
	QRectF dstRect = mapFromItem( dest, dest->boundingRect() ).boundingRect();
	if ( srcRect.adjusted( -arrowSize, -arrowSize, arrowSize, arrowSize).intersects( dstRect.adjusted( -arrowSize, -arrowSize, arrowSize, arrowSize) ) ) {
		removeFromIndex();
		//--- it's a loop
		QRectF rec = srcRect.united( dstRect );
		QPointF sourcePoint = mapFromItem( source, -srcRect.width()/4.0, -srcRect.height()/2.0 );
		QPointF destPoint = mapFromItem( dest, -dstRect.width()/4.0, +dstRect.height()/2.0 );
		QPointF c1 = QPointF( -rec.width()/2.0, -rec.height()*1.5f ) + rec.center();
		QPointF c2 = QPointF( -rec.width()/2.0-20.0, -rec.height()*1.5f ) + rec.center();
		QPointF mid = QPointF( -rec.width()/2.0-20.0, 0 ) + rec.center();
		QPointF c3 = QPointF( -rec.width()/2.0-20.0, +rec.height()*1.5f ) + rec.center();
		QPointF c4 = QPointF( -rec.width()/2.0, +rec.height()*1.5f ) + rec.center();
		path = QPainterPath();
		path.moveTo( sourcePoint );
		path.cubicTo( c1, c2, mid );
		path.cubicTo( c3, c4, destPoint );
		// Draw the arrows 
		adjustArrow( QLineF(c4, destPoint) );
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
	//--- creating the cubic curve from sourcePoint to destPoint
	line = QLineF( sourcePoint, destPoint );
	//--- calculating c1 control-point
	r = mapFromItem( source, source->boundingRect() ).boundingRect();
	QPointF c1;
	//qreal minctrl = 1.0f;
	qreal coeffctrl = 2.0f;
	if ( sourcePoint.y() < r.top() ) {
		//--- is up
		c1 = QPointF( sourcePoint.x(), sourcePoint.y()+line.dy()/coeffctrl );
	} else if ( sourcePoint.y() > r.bottom() ) {
		//--- is bottom
		c1 = QPointF( sourcePoint.x(), sourcePoint.y()+line.dy()/coeffctrl );
	} else if ( sourcePoint.x() < r.left() ) {
		//--- is left
		c1 = QPointF( sourcePoint.x()+line.dx()/coeffctrl, sourcePoint.y() );
	} else if ( sourcePoint.x() > r.right() ) {
		//--- is right
		c1 = QPointF( sourcePoint.x()+line.dx()/coeffctrl, sourcePoint.y() );
	}
	//--- calculating c2 control-point
	r = mapFromItem( dest, dest->boundingRect() ).boundingRect();
	QPointF c2;
	QPointF offset;
	if ( destPoint.y() < r.top() ) {
		//--- is up
		c2 = QPointF( destPoint.x(), destPoint.y()-line.dy()/coeffctrl );
		offset = QPointF( 0, -arrowSize );
	} else if ( destPoint.y() > r.bottom() ) {
		//--- is bottom
		c2 = QPointF( destPoint.x(), destPoint.y()-line.dy()/coeffctrl );
		offset = QPointF( 0, +arrowSize );
	} else if ( destPoint.x() < r.left() ) {
		//--- is left
		c2 = QPointF( destPoint.x()-line.dx()/coeffctrl, destPoint.y() );
		offset = QPointF( -arrowSize, 0 );
	} else if ( destPoint.x() > r.right() ) {
		//--- is right
		c2 = QPointF( destPoint.x()-line.dx()/coeffctrl, destPoint.y() );
		offset = QPointF( +arrowSize, 0 );
	}

	path = QPainterPath();
	path.moveTo( sourcePoint );
	path.cubicTo( c1, c2, destPoint+offset );
	// Draw the arrows 
	adjustArrow( QLineF(c2, destPoint) );
	addToIndex();
}

void LinkerRenderer::adjustArrow( QLineF line ) {
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0) {
		angle = TwoPi - angle;
	}
	QPointF destArrowP1 = line.p2() + QPointF(sin(angle - Pi / 3) * arrowSize,
											cos(angle - Pi / 3) * arrowSize);
	QPointF destArrowP2 = line.p2() + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
											cos(angle - Pi + Pi / 3) * arrowSize);
	arrowPath = QPainterPath();
	arrowPath.addPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2 << line.p2());
}

QRectF LinkerRenderer::boundingRect() const {
	if (!source || !dest) {
		return QRectF();
	}
	qreal penWidth = 1;
	qreal extra = (penWidth + arrowSize) / 2.0;
	return path.boundingRect().united( arrowPath.boundingRect() ).adjusted(-extra, -extra, extra, extra);
}

void LinkerRenderer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawPath( path );

	painter->setBrush(QColor(122, 163, 39));
	painter->drawPath( arrowPath );
}

