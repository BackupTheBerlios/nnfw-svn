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
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <cmath>

using namespace nnfw;

//--- margin used to separate a little the data plotted
const qreal ymarg = 2.0f;

ClusterPlotter::ClusterPlotter( Cluster* cl ) {
	this->cl = cl;
	setFlag(ItemIsMovable, false);
	setFlag(ItemIsSelectable, false);
	setZValue(1);
	setToolTip( QString("Plotting of %1").arg(cl->name()) );
	ins.resize( cl->numNeurons() );
	lowins.resize( cl->numNeurons() );
	highins.resize( cl->numNeurons() );
	myins.resize( cl->numNeurons() );

	outs.resize( cl->numNeurons() );
	lowouts.resize( cl->numNeurons() );
	highouts.resize( cl->numNeurons() );
	myouts.resize( cl->numNeurons() );

	resetToZero();

    showOutsA = new QAction( tr( "Outputs" ), this );
    showOutsA->setStatusTip( tr("Show/Hide outputs plot") );
    showOutsA->setCheckable( true );
	showOutsA->setChecked( true );
    connect( showOutsA, SIGNAL( toggled(bool) ),
            this, SLOT( showOutputs(bool) ) );

    showInsA = new QAction( tr( "Inputs" ), this );
    showInsA->setStatusTip( tr("Show/Hide inputs plot") );
    showInsA->setCheckable( true );
	showInsA->setChecked( false );
    connect( showInsA, SIGNAL( toggled(bool) ),
            this, SLOT( showInputs(bool) ) );

	menu = new QMenu();
	menu->addAction( showOutsA );
	menu->addAction( showInsA );

	showouts = true;
	showins = false;
	changed = false;
}

ClusterPlotter::~ClusterPlotter() {
	delete menu;
}
	
QRectF ClusterPlotter::boundingRect() const {
	if ( changed ) {
		qreal toth = 0.0f;
		for( int i=0; i<ins.size(); i++ ) {
			if ( showins ) {
				toth += qAbs( highins[i] - lowins[i] ) + ymarg;
				//toth += ins[i].boundingRect().height() + ymarg;
			}
			if ( showouts ) {
				toth += qAbs( highouts[i] - lowouts[i] ) + ymarg;
				//toth += outs[i].boundingRect().height() + ymarg;
			}
		}
		brect = QRectF( 0, 0, outs[0][lastPoint].x(), toth );
	}
	return brect;
}

QPainterPath ClusterPlotter::shape() const {
	QPainterPath path;
	path.addRect( brect );
	return path;
}

#include <QTime>
QTime timer;

void ClusterPlotter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	//timer.start();
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();

	painter->setPen( Qt::black );
	painter->setBrush( Qt::transparent );
	painter->drawRect( brect );

	painter->setPen( QPen(Qt::NoPen) );
	qreal prevoff = 0.0f;
	for( int i=0; i<ins.size(); i++ ) {
		if ( showouts ) {
			painter->translate( 0, prevoff - lowouts[i] + ymarg );
			painter->setBrush( Qt::red );
			painter->drawPolygon( outs[i] );
			prevoff = +highouts[i];
		}
		if ( showins ) {
			painter->translate( 0, prevoff - lowins[i] + ymarg );
			painter->setBrush( Qt::blue );
			painter->drawPolygon( ins[i] );
			prevoff = +highins[i];
		}
	}

	painter->restore();
	//qDebug() << timer.elapsed();
}

void ClusterPlotter::resetToZero() {
	QPolygonF zero;
	zero << QPointF(0.0f, 0.0f) << QPointF(0.0f, 0.0f);
	ins.fill( zero );
	lowins.fill( -10.0f );
	highins.fill( +10.0f );
	myins.fill( 10.0f );

	outs.fill( zero );
	lowouts.fill( -10.0f );
	highouts.fill( +10.0f );
	myouts.fill( 10.0f );

	lastPoint = 1;
}

void ClusterPlotter::updatePlot() {
	//---offset along t-axis
	const float dx = 1.0f;
	//--- pushing a new value
	for( int i=0; i<ins.size(); i++ ) {
		Real v = cl->inputs()[i];
		//--- the Y-axis is mirrored respect to the normal cartesian-axis in plotting
		qreal pv = -v*myins[i];
		qreal cx = ins[i][lastPoint].x();
 		ins[i][lastPoint].setY( pv );
 		ins[i] << QPointF( cx+dx, pv ) << QPointF( cx+dx, 0.0f );
		highins[i] = qMax( pv, highins[i] );
		lowins[i] = qMin( pv, lowins[i] );
	}
	//--- outputs
	for( int i=0; i<outs.size(); i++ ) {
		Real v = cl->outputs()[i];
		//--- the Y-axis is mirrored respect to the normal cartesian-axis in plotting
		qreal pv = -v*myouts[i];
		qreal cx = outs[i][lastPoint].x();
 		outs[i][lastPoint].setY( pv );
 		outs[i] << QPointF( cx+dx, pv ) << QPointF( cx+dx, 0.0f );
		highouts[i] = qMax( pv, highouts[i] );
		lowouts[i] = qMin( pv, lowouts[i] );
	}
	lastPoint += 2;
	changed = true;
}

// void ClusterPlotter::fitToHeight( qreal hf ) {
// 	// input data
// 	for( int i=0; i<ins.size(); i++ ) {
// 		qreal dy = qAbs( highins[i] - lowins[i] );
// 		qreal newmy = hf/dy;
// 		if ( dy == 0.0f ) {
// 			continue;
// 		}
// 		if ( newmy>100 ) {
// 			newmy = 100.0f;
// 		}
// 		// if the newmy is almost equal to previous one skip the calculation
// 		if ( qAbs(newmy-myins[i]) < 1.0f ) continue;
// 		qreal scale = newmy/myins[i];
// 		highins[i] = 0.0f;
// 		lowins[i] = 0.0f;
// 		for( int p=0; p<ins[i].size(); p++ ) {
// 			qreal newy = ins[i][p].y() * scale;
// 			ins[i][p].setY( newy );
// 			highins[i] = qMax( newy, highins[i] );
// 			lowins[i] = qMin( newy, lowins[i] );
// 		}
// 		myins[i] = newmy;
// 	}
// 	// output data
// 	for( int i=0; i<outs.size(); i++ ) {
// 		qreal dy = qAbs( highouts[i] - lowouts[i] );
// 		if ( dy == 0.0f ) {
// 			continue;
// 		}
// 		qreal newmy = hf/dy;
// 		if ( newmy>100 ) {
// 			newmy = 100.0f;
// 		}
// 		// if the newmy is almost equal to previous one skip the calculation
// 		if ( qAbs(newmy-myouts[i]) < 1.0f ) continue;
// 		qreal scale = newmy/myouts[i];
// 		highouts[i] = 0.0f;
// 		lowouts[i] = 0.0f;
// 		for( int p=0; p<outs[i].size(); p++ ) {
// 			qreal newy = outs[i][p].y() * scale;
// 			outs[i][p].setY( newy );
// 			highouts[i] = qMax( newy, highouts[i] );
// 			lowouts[i] = qMin( newy, lowouts[i] );
// 		}
// 		myouts[i] = newmy;
// 	}
// 	changed = true;
// }

void ClusterPlotter::showOutputs( bool b ) {
	showouts = b;
	changed = true;
	emit heightChanged();
}

void ClusterPlotter::showInputs( bool b ) {
	showins = b;
	changed = true;
	emit heightChanged();
}

void ClusterPlotter::contextMenuEvent( QGraphicsSceneContextMenuEvent* event ) {
	menu->popup( event->screenPos() );
}

