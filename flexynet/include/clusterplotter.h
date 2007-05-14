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

#ifndef CLUSTERPLOTTER_H
#define CLUSTERPLOTTER_H

#include "nnfw/cluster.h"
#include "fnnwrapper.h"
#include <QObject>
#include <QGraphicsItem>

class ClusterPlotter : public QObject, public QGraphicsItem {
	Q_OBJECT
public:
	ClusterPlotter( FNNWrapper* net, nnfw::Cluster* cl );
	
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	//! reset the plot zeroing all data registered so far
	void resetToZero();

protected:
// 	void mousePressEvent( QGraphicsSceneMouseEvent *event );
// 	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
// 	void mouseMoveEvent( QGraphicsSceneMouseEvent* event );

private slots:
	//! update the plot
	void updatePlot();

private:
	//! used to connect the signal stepped to slot updatePlot
	FNNWrapper* net;
	//! cluster to plot
	nnfw::Cluster* cl;
	//! bounding rect
	QRectF brect;
	//! input data plotted
	QVector<QPolygonF> ins;
	//! output data plotted
	QVector<QPolygonF> outs;
	//! lastPoint, or last valid index of QPolygonFs
	int lastPoint;
	//! low Y value of ins data plotted
	QVector<qreal> lowins;
	//! high Y value of ins data plotted
	QVector<qreal> highins;
	//! low Y value of outs data plotted
	QVector<qreal> lowouts;
	//! high Y value of outs data plotted
	QVector<qreal> highouts;
};

#endif
