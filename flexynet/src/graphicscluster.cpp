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

#include "graphicscluster.h"
#include "clustermodel.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <cmath>

GraphicsCluster::GraphicsCluster( ClusterModel* cl ) {
	this->cl = cl;
	dim = (int)cl->numNeurons();
	//--- calculate the rectangle
	recw = dim*14;
	rech = 14;
	rectlx = -recw/2;
	rectly = -rech/2;
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setZValue(1);
	setToolTip( cl->name() );
	grabbed = false;
	isInput = false;
	isOutput = false;
}

void GraphicsCluster::markAsInput( bool b ) {
	isInput = b;
}

void GraphicsCluster::markAsOutput( bool b ) {
	isOutput = b;
}

QRectF GraphicsCluster::boundingRect() const {
	return QRectF( rectlx, rectly, recw, rech ).adjusted( -3, -3, 3, 3 );
}

QPainterPath GraphicsCluster::shape() const {
	QPainterPath path;
	path.addRect(rectlx, rectly, recw, rech);
	return path;
}

void GraphicsCluster::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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
	QRectF rect = QRectF(rectlx, rectly, recw, rech).adjusted(-2,-2,2,2);
	painter->drawRect( rect );
	if ( isInput ) {
		painter->save();
		painter->setPen( Qt::red );
		painter->drawLine( rect.bottomLeft(), rect.bottomRight() );
		painter->restore();
	}
	if ( isOutput ) {
		painter->save();
		painter->setPen( Qt::magenta );
		painter->drawLine( rect.topLeft(), rect.topRight() );
		painter->restore();
	}
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

void GraphicsCluster::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	grabbed = true;
	setZValue( 5 );
	update();
	QGraphicsItem::mousePressEvent(event);
}

void GraphicsCluster::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	if ( !grabbed ) return;
	QGraphicsItem::mouseMoveEvent(event);
	update();
}

void GraphicsCluster::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	grabbed = false;
	setZValue( 1 );
	update();
	QGraphicsItem::mouseReleaseEvent(event);
}

QVariant GraphicsCluster::itemChange(GraphicsItemChange change, const QVariant &value) {
	switch (change) {
	case ItemPositionChange: {
		emit positionChanged();
		break;
	}
	default:
		break;
	};
	return QGraphicsItem::itemChange(change, value);
}

