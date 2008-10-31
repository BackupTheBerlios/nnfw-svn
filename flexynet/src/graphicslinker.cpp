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

#include "graphicslinker.h"
#include "graphicscluster.h"

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

GraphicsLinker::GraphicsLinker( LinkerModel* lk, GraphicsCluster* fromv, GraphicsCluster* tov )
	: path(), arrowSize(10) {
	setFlag(ItemIsSelectable);
	this->lk = lk;
	this->fromv = fromv;
	this->tov = tov;
	adjust();
	connect( fromv, SIGNAL( positionChanged() ),
			this, SLOT( adjust() ) );
	connect( tov, SIGNAL( positionChanged() ),
			this, SLOT( adjust() ) );
}

void GraphicsLinker::adjust() {
	if (!fromv || !tov) {
		return;
	}
	QRectF srcRect = mapFromItem( fromv, fromv->boundingRect() ).boundingRect();
	QRectF dstRect = mapFromItem( tov, tov->boundingRect() ).boundingRect();
	if ( srcRect.adjusted( -arrowSize, -arrowSize, arrowSize, arrowSize).intersects( dstRect.adjusted( -arrowSize, -arrowSize, arrowSize, arrowSize) ) ) {
		removeFromIndex();
		//--- it's a loop
		QRectF rec = srcRect.united( dstRect );
		QPointF fromPoint = mapFromItem( fromv, -srcRect.width()/4.0, -srcRect.height()/2.0 );
		QPointF toPoint = mapFromItem( tov, -dstRect.width()/4.0, +dstRect.height()/2.0 );
		QPointF c1 = QPointF( -rec.width()/2.0, -rec.height()*1.5f ) + rec.center();
		QPointF c2 = QPointF( -rec.width()/2.0-20.0, -rec.height()*1.5f ) + rec.center();
		QPointF mid = QPointF( -rec.width()/2.0-20.0, 0 ) + rec.center();
		QPointF c3 = QPointF( -rec.width()/2.0-20.0, +rec.height()*1.5f ) + rec.center();
		QPointF c4 = QPointF( -rec.width()/2.0, +rec.height()*1.5f ) + rec.center();
		path = QPainterPath();
		path.moveTo( fromPoint );
		path.cubicTo( c1, c2, mid );
		path.cubicTo( c3, c4, toPoint );
		// Draw the arrows 
		adjustArrow( QLineF(c4, toPoint) );
		addToIndex();
		return;
	}
	//--- normal condition		
	QLineF line(mapFromItem(fromv, 0, 0), mapFromItem(tov, 0, 0));
	//--- find the SourcePoint of path
	QRectF srcRect2 = srcRect.adjusted( -0.6, -0.6, 0.6, 0.6 );
	QPointF fromPoint;
	if ( line.intersect( QLineF(srcRect2.topLeft(), srcRect2.topRight()), &fromPoint ) ==
		 QLineF::BoundedIntersection ) {
	} else if ( line.intersect( QLineF(srcRect2.topRight(), srcRect2.bottomRight()), &fromPoint ) ==
		 QLineF::BoundedIntersection ) {
	} else if ( line.intersect( QLineF(srcRect2.bottomLeft(), srcRect2.bottomRight()), &fromPoint ) ==
		 QLineF::BoundedIntersection ) {
	} else {
		line.intersect( QLineF(srcRect2.bottomLeft(), srcRect2.topLeft()), &fromPoint );
	}
	//--- find the DestPoint of path
	QRectF dstRect2 = dstRect.adjusted( -0.6, -0.6, 0.6, 0.6 );
	QPointF toPoint;
	if ( line.intersect( QLineF(dstRect2.topLeft(), dstRect2.topRight()), &toPoint ) ==
		 QLineF::BoundedIntersection ) {
	} else if ( line.intersect( QLineF(dstRect2.topRight(), dstRect2.bottomRight()), &toPoint ) ==
		 QLineF::BoundedIntersection ) {
	} else if ( line.intersect( QLineF(dstRect2.bottomLeft(), dstRect2.bottomRight()), &toPoint ) ==
		 QLineF::BoundedIntersection ) {
	} else {
		line.intersect( QLineF(dstRect2.bottomLeft(), dstRect2.topLeft()), &toPoint );
	}
	removeFromIndex();
	//--- creating the cubic curve from fromPoint to toPoint
	line = QLineF( fromPoint, toPoint );
	//--- calculating c1 control-point
	QPointF c1;
	//qreal minctrl = 1.0f;
	qreal coeffctrl = 2.0f;
	if ( fromPoint.y() <= srcRect.top() ) {
		//--- is up
		c1 = QPointF( fromPoint.x(), fromPoint.y()+line.dy()/coeffctrl );
	} else if ( fromPoint.y() >= srcRect.bottom() ) {
		//--- is bottom
		c1 = QPointF( fromPoint.x(), fromPoint.y()+line.dy()/coeffctrl );
	} else if ( fromPoint.x() <= srcRect.left() ) {
		//--- is left
		c1 = QPointF( fromPoint.x()+line.dx()/coeffctrl, fromPoint.y() );
	} else if ( fromPoint.x() >= srcRect.right() ) {
		//--- is right
		c1 = QPointF( fromPoint.x()+line.dx()/coeffctrl, fromPoint.y() );
	}
	//--- calculating c2 control-point
	QPointF c2;
	QPointF offset;
	if ( toPoint.y() < dstRect.top() ) {
		//--- is up
		c2 = QPointF( toPoint.x(), toPoint.y()-line.dy()/coeffctrl );
		offset = QPointF( 0, -arrowSize );
	} else if ( toPoint.y() > dstRect.bottom() ) {
		//--- is bottom
		c2 = QPointF( toPoint.x(), toPoint.y()-line.dy()/coeffctrl );
		offset = QPointF( 0, +arrowSize );
	} else if ( toPoint.x() < dstRect.left() ) {
		//--- is left
		c2 = QPointF( toPoint.x()-line.dx()/coeffctrl, toPoint.y() );
		offset = QPointF( -arrowSize, 0 );
	} else if ( toPoint.x() > dstRect.right() ) {
		//--- is right
		c2 = QPointF( toPoint.x()-line.dx()/coeffctrl, toPoint.y() );
		offset = QPointF( +arrowSize, 0 );
	}

	path = QPainterPath();
	path.moveTo( fromPoint );
	path.cubicTo( c1, c2, toPoint+offset );
	// Draw the arrows 
	adjustArrow( QLineF(c2, toPoint) );
	addToIndex();
}

void GraphicsLinker::adjustArrow( QLineF line ) {
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0) {
		angle = TwoPi - angle;
	}
	QPointF toArrowP1 = line.p2() + QPointF(sin(angle - Pi / 3) * arrowSize,
											cos(angle - Pi / 3) * arrowSize);
	QPointF toArrowP2 = line.p2() + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
											cos(angle - Pi + Pi / 3) * arrowSize);
	arrowPath = QPainterPath();
	arrowPath.addPolygon(QPolygonF() << line.p2() << toArrowP1 << toArrowP2 << line.p2());
}

QRectF GraphicsLinker::boundingRect() const {
	if (!fromv || !tov) {
		return QRectF();
	}
	qreal penWidth = 1;
	qreal extra = (penWidth + arrowSize) / 2.0;
	return path.boundingRect().united( arrowPath.boundingRect() ).adjusted(-extra, -extra, extra, extra);
}

QPainterPath GraphicsLinker::shape() const {
	QPainterPath rp;
	rp.addPath( path );
	rp.addPath( arrowPath );
	QPainterPathStroker stroke;
	stroke.setWidth( 10 );
	return stroke.createStroke( rp );
}

void GraphicsLinker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
	if ( (option->state & QStyle::State_Sunken) || isSelected() ) {
		painter->setPen(QPen(Qt::gray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	} else {
		painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	}
	painter->drawPath( path );

	painter->setBrush(QColor(122, 163, 39));
	painter->drawPath( arrowPath );
}

