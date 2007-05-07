
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
#include <QPainterPathStroker>
#include <cmath>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

LinkerRenderer::LinkerRenderer(ClusterRenderer *sourceNode, ClusterRenderer *destNode )
	: path(), arrowSize(10) {
	setFlag(ItemIsSelectable);
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
	QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
	//--- find the SourcePoint of path
	QRectF srcRect2 = srcRect.adjusted( -0.6, -0.6, 0.6, 0.6 );
	QPointF sourcePoint;
	if ( line.intersect( QLineF(srcRect2.topLeft(), srcRect2.topRight()), &sourcePoint ) ==
		 QLineF::BoundedIntersection ) {
	} else if ( line.intersect( QLineF(srcRect2.topRight(), srcRect2.bottomRight()), &sourcePoint ) ==
		 QLineF::BoundedIntersection ) {
	} else if ( line.intersect( QLineF(srcRect2.bottomLeft(), srcRect2.bottomRight()), &sourcePoint ) ==
		 QLineF::BoundedIntersection ) {
	} else {
		line.intersect( QLineF(srcRect2.bottomLeft(), srcRect2.topLeft()), &sourcePoint );
	}
	//--- find the DestPoint of path
	QRectF dstRect2 = dstRect.adjusted( -0.6, -0.6, 0.6, 0.6 );
	QPointF destPoint;
	if ( line.intersect( QLineF(dstRect2.topLeft(), dstRect2.topRight()), &destPoint ) ==
		 QLineF::BoundedIntersection ) {
	} else if ( line.intersect( QLineF(dstRect2.topRight(), dstRect2.bottomRight()), &destPoint ) ==
		 QLineF::BoundedIntersection ) {
	} else if ( line.intersect( QLineF(dstRect2.bottomLeft(), dstRect2.bottomRight()), &destPoint ) ==
		 QLineF::BoundedIntersection ) {
	} else {
		line.intersect( QLineF(dstRect2.bottomLeft(), dstRect2.topLeft()), &destPoint );
	}
	removeFromIndex();
	//--- creating the cubic curve from sourcePoint to destPoint
	line = QLineF( sourcePoint, destPoint );
	//--- calculating c1 control-point
	QPointF c1;
	//qreal minctrl = 1.0f;
	qreal coeffctrl = 2.0f;
	if ( sourcePoint.y() <= srcRect.top() ) {
		//--- is up
		c1 = QPointF( sourcePoint.x(), sourcePoint.y()+line.dy()/coeffctrl );
	} else if ( sourcePoint.y() >= srcRect.bottom() ) {
		//--- is bottom
		c1 = QPointF( sourcePoint.x(), sourcePoint.y()+line.dy()/coeffctrl );
	} else if ( sourcePoint.x() <= srcRect.left() ) {
		//--- is left
		c1 = QPointF( sourcePoint.x()+line.dx()/coeffctrl, sourcePoint.y() );
	} else if ( sourcePoint.x() >= srcRect.right() ) {
		//--- is right
		c1 = QPointF( sourcePoint.x()+line.dx()/coeffctrl, sourcePoint.y() );
	}
	//--- calculating c2 control-point
	QPointF c2;
	QPointF offset;
	if ( destPoint.y() < dstRect.top() ) {
		//--- is up
		c2 = QPointF( destPoint.x(), destPoint.y()-line.dy()/coeffctrl );
		offset = QPointF( 0, -arrowSize );
	} else if ( destPoint.y() > dstRect.bottom() ) {
		//--- is bottom
		c2 = QPointF( destPoint.x(), destPoint.y()-line.dy()/coeffctrl );
		offset = QPointF( 0, +arrowSize );
	} else if ( destPoint.x() < dstRect.left() ) {
		//--- is left
		c2 = QPointF( destPoint.x()-line.dx()/coeffctrl, destPoint.y() );
		offset = QPointF( -arrowSize, 0 );
	} else if ( destPoint.x() > dstRect.right() ) {
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

QPainterPath LinkerRenderer::shape() const {
	QPainterPath rp;
	rp.addPath( path );
	rp.addPath( arrowPath );
	QPainterPathStroker stroke;
	stroke.setWidth( 10 );
	return stroke.createStroke( rp );
}

void LinkerRenderer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
	if ( (option->state & QStyle::State_Sunken) || isSelected() ) {
		painter->setPen(QPen(Qt::gray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	} else {
		painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	}
	painter->drawPath( path );

	painter->setBrush(QColor(122, 163, 39));
	painter->drawPath( arrowPath );
}

