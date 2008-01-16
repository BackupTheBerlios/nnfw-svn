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

#include "clusterplotter2.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>
#include <QAction>
#include <QLinearGradient>
#include <QMenu>
#include <cmath>

using namespace nnfw;

//--- margin used to separate a little the data plotted
const int ymarg = 1;
//--- dimension of colored-squares
const int rside = 5;

ClusterPlotter2::ClusterPlotter2( Cluster* cl ) {
	this->cl = cl;
	this->dim = cl->numNeurons();
	setFlag(ItemIsMovable, false);
	setFlag(ItemIsSelectable, false);
	setAcceptsHoverEvents( true );
	setZValue(1);
	//setToolTip( QString("Plotting of %1").arg(cl->name()) );

	ins.resize( dim );
	lowins.resize( dim );
	highins.resize( dim );

	outs.resize( dim );
	lowouts.resize( dim );
	highouts.resize( dim );

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
}

ClusterPlotter2::~ClusterPlotter2() {
	delete menu;
}
	
QRectF ClusterPlotter2::boundingRect() const {
	return brect;
}

QPainterPath ClusterPlotter2::shape() const {
	QPainterPath path;
	path.addRect( brect );
	return path;
}

#include <QTime>
QTime crono;

void ClusterPlotter2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen( Qt::black );
	painter->setBrush( Qt::transparent );
	painter->drawRect( brect );

	// if no plots are required print a message
	if ( !showouts && !showins ) {
		painter->drawText( 0, 0, (int)brect.width(), (int)brect.height(), Qt::AlignLeft | Qt::AlignVCenter,
			"No Plot Request" );
	}

	painter->setPen( QPen(Qt::NoPen) );
	int steps = qMax( 1, (int)round(10.0f/(1.0f+option->levelOfDetail))-3 );
	// plot outputs
	for( int i=0; i<dim && showouts; i++ ) {
		qreal m = 255.0f/( highouts[i]-lowouts[i] );
		qreal q = -m*lowouts[i];
		for( int r=0; r<lastPoint; r=r+steps ) {
			qreal v = 0.0f;
			int eff = qMin( lastPoint-r, steps );
			for( int s=0; s<eff; s++ ) {
				v += outs[i][r+s];
			}
			v /= eff;
			painter->fillRect( r*rside, i*(rside+ymarg), rside*eff, rside, QColor( (int)(m*v+q), 0, 0 ) );
		}
	}
	int offset = showouts*dim*(rside+ymarg) /*+ 2*ymarg*/;
	//plot inputs
	for( int i=0; i<dim && showins; i++ ) {
		qreal m = 255.0f/( highins[i]-lowins[i] );
		qreal q = -m*lowins[i];
		for( int r=0; r<lastPoint; r=r+steps ) {
			qreal v = 0.0f;
			int eff = qMin( lastPoint-r, steps );
			for( int s=0; s<eff; s++ ) {
				v += ins[i][r+s];
			}
			v /= eff;
			painter->fillRect( r*rside, i*(rside+ymarg)+offset, rside*eff, rside, QColor( 0, (int)(m*v+q), 0 ) );
		}
	}
	// Legend
	if ( legendActive && steps==1/*option->levelOfDetail>2.0f*/ ) {
		//--- setup Gradients and Text to display
		int h = (int)brect.height() - 13;
		QLinearGradient linearGrad( QPointF(-20, 0), QPointF(-20, h) );
		QString top; QString bot; QString val;
		if ( legendTop ) {
			if ( showins && !showouts ) {
				top = QString("%1").arg( highins[legendId] );
				bot = QString("%1").arg( lowins[legendId] );
				val = QString("%1").arg( ins[legendId][legendT] );
				linearGrad.setColorAt(0, Qt::green);
				linearGrad.setColorAt(1, Qt::black);
			} else {
				top = QString("%1").arg( highouts[legendId] );
				bot = QString("%1").arg( lowouts[legendId] );
				val = QString("%1").arg( outs[legendId][legendT] );
				linearGrad.setColorAt(0, Qt::red);
				linearGrad.setColorAt(1, Qt::black);
			}
		} else {
			//--- can be only an inputs row
			top = QString("%1").arg( highins[legendId] );
			bot = QString("%1").arg( lowins[legendId] );
			val = QString("%1").arg( ins[legendId][legendT] );
			linearGrad.setColorAt(0, Qt::green);
			linearGrad.setColorAt(1, Qt::black);
		}
		//--- draw the color-scale
		painter->fillRect( -20, 0, 20, h, linearGrad );
		//--- setup font & draw the range of color-scale
		QFont f = painter->font();
		f.setPixelSize( 6 );
		painter->setFont( f );
		painter->setPen( Qt::blue );
		painter->drawText( -20, -1, top );
		painter->drawText( -20, (int)brect.bottom()-1, bot );
		QFontMetrics fm = painter->fontMetrics();
		QRect rec = fm.boundingRect( val );
		rec.moveTo( legendT*rside, (legendId+1)*(rside+ymarg)+(!legendTop)*dim*(rside+ymarg)-1 );
		rec.adjust( -1, +1, +1, -1 );
		painter->fillRect( rec.x()+1, rec.y()+1, rec.width(), rec.height(), Qt::gray );
		painter->fillRect( rec, Qt::white );
		painter->drawRect( rec );
		rec.setY( rec.y()+1 );
		painter->drawText( rec, Qt::AlignCenter | Qt::AlignVCenter, val );
	}
}

void ClusterPlotter2::resetToZero() {
	for( int i=0; i<dim; i++ ) {
		ins[i].resize( 0 );
		lowins[i] = -0.1f;
		highins[i] = +0.1f;
		outs[i].resize(0);
		lowouts[i] = -0.1f;
		highouts[i] = +0.1f;
	}
	lastPoint = 0;
}

void ClusterPlotter2::updatePlot() {
	//--- pushing a new value
	for( int i=0; i<dim; i++ ) {
		qreal v = cl->inputs()[i];
		ins[i].append( v );
		highins[i] = qMax( v, highins[i] );
		lowins[i] = qMin( v, lowins[i] );
	}
	//--- outputs
	for( int i=0; i<outs.size(); i++ ) {
		qreal v = cl->outputs()[i];
		outs[i].append( v );
		highouts[i] = qMax( v, highouts[i] );
		lowouts[i] = qMin( v, lowouts[i] );
	}
	lastPoint += 1;
	recalcBRect();
	update();
}

void ClusterPlotter2::showOutputs( bool b ) {
	showouts = b;
	recalcBRect();
	emit heightChanged();
	update();
}

void ClusterPlotter2::showInputs( bool b ) {
	showins = b;
	recalcBRect();
	emit heightChanged();
	update();
}

void ClusterPlotter2::contextMenuEvent( QGraphicsSceneContextMenuEvent* event ) {
	menu->popup( event->screenPos() );
}

void ClusterPlotter2::hoverLeaveEvent( QGraphicsSceneHoverEvent* ) {
	legendActive = false;
	update();
}

void ClusterPlotter2::hoverMoveEvent( QGraphicsSceneHoverEvent * event ) {
	// --- calculate the row-Id (or the index of neuron)
	int y = event->pos().toPoint().y();
	legendId = y/(rside+ymarg);
	if ( ! (legendTop = (legendId < dim)) ) {;
		legendId -= dim;
	}
	// --- calculate the t-index, the index of data where the mouse is (the colored-rectangle)
	int x = event->pos().toPoint().x();
	legendT = x/rside;
	legendActive = ( legendId >= 0 && legendId < dim && 
					 y<((showouts*dim + showins*dim)*(rside+ymarg)) &&
					 x>0 && x<(rside*lastPoint) );
	update();
}

void ClusterPlotter2::recalcBRect() {
	if ( !showouts && !showins ) {
		brect = QRectF( 0, 0, lastPoint*rside, qMin( 50, 2*dim*(rside+ymarg) )/*+2*ymarg*/ )
					.adjusted(-23,-6,+3,+6);
	} else {
		brect = QRectF( 0, 0, lastPoint*rside, (showouts*dim + showins*dim)*(rside+ymarg)/*+2*ymarg*/ )
					.adjusted(-23,-6,+3,+6);
	}
}
