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

#include "clusterplotter.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <cmath>

using namespace nnfw;

ClusterPlotter::ClusterPlotter( FNNWrapper* net, Cluster* cl ) {
	this->net = net;
	this->cl = cl;
	setFlag(ItemIsMovable, false);
	setFlag(ItemIsSelectable, false);
	setZValue(1);
	setToolTip( QString("Plotting of %1").arg(cl->getName()) );
	ins.resize( cl->numNeurons() );
	lowins.resize( cl->numNeurons() );
	highins.resize( cl->numNeurons() );

	outs.resize( cl->numNeurons() );
	lowouts.resize( cl->numNeurons() );
	highouts.resize( cl->numNeurons() );

	resetToZero();
	connect( net, SIGNAL( stepped() ),
			this, SLOT( updatePlot() ) );
}
	
QRectF ClusterPlotter::boundingRect() const {
	return brect;
}

QPainterPath ClusterPlotter::shape() const {
	QPainterPath path;
	path.addRect( brect );
	return path;
}

//--- margin used to separate a little the data plotted
const qreal ymarg = 2.0f;

void ClusterPlotter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();

	painter->setPen( Qt::black );
	painter->setBrush( Qt::transparent );
	painter->drawRect( brect );

	painter->setPen( Qt::transparent );
	qreal prevoff = 0.0f;
	for( int i=0; i<ins.size(); i++ ) {
		painter->translate( 0, prevoff - lowouts[i] + ymarg );
		painter->setBrush( Qt::red );
		painter->drawPolygon( outs[i] );
		prevoff = +highouts[i];

		painter->translate( 0, prevoff - lowins[i] + ymarg );
		painter->setBrush( Qt::blue );
		painter->drawPolygon( ins[i] );
		prevoff = +highins[i];
	}

	painter->restore();
}

void ClusterPlotter::resetToZero() {
	QPolygonF zero;
	zero << QPointF(0.0f, 0.0f) << QPointF(0.0f, 0.0f);
	ins.fill( zero );
	lowins.fill( 0.0f );
	highins.fill( 0.0f );

	outs.fill( zero );
	lowouts.fill( 0.0f );
	highouts.fill( 0.0f );
	lastPoint = 1;
}

void ClusterPlotter::updatePlot() {
	//---offset along t-axis
	const float dx = 2.0f;
	//---how many pixel for one real
	const float my = 10.0f;
	//--- all heights summed-up
	qreal toth = 0.0f;
	//--- pushing a new value
	for( int i=0; i<ins.size(); i++ ) {
		Real v = cl->inputs()[i];
		//--- the Y-axis is mirrored respect to the normal cartesian-axis in plotting
		qreal pv = -v*my;
		qreal cx = ins[i][lastPoint].x();
		ins[i][lastPoint].setY( pv );
		ins[i] << QPointF( cx+dx, pv ) << QPointF( cx+dx, 0.0f );
		toth += ins[i].boundingRect().height() + ymarg;
		highins[i] = qMax( pv, highins[i] );
		lowins[i] = qMin( pv, lowins[i] );
	}
	//--- outputs
	for( int i=0; i<outs.size(); i++ ) {
		Real v = cl->outputs()[i];
		//--- the Y-axis is mirrored respect to the normal cartesian-axis in plotting
		qreal pv = -v*my;
		qreal cx = outs[i][lastPoint].x();
		outs[i][lastPoint].setY( pv );
		outs[i] << QPointF( cx+dx, pv ) << QPointF( cx+dx, 0.0f );
		toth += outs[i].boundingRect().height() + ymarg;
		highouts[i] = qMax( pv, highouts[i] );
		lowouts[i] = qMin( pv, lowouts[i] );
	}
	lastPoint += 2;
	brect = QRectF( 0, 0, outs[0][lastPoint].x(), toth );
	update();
}

