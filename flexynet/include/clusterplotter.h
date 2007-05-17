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
#include "types.h"

#include "nnfw/cluster.h"
#include "fnnwrapper.h"
#include <QGraphicsItem>
#include <QObject>

class QAction;
class QMenu;

class FLEX_API ClusterPlotter : public QObject, public QGraphicsItem {
	Q_OBJECT
public:
	ClusterPlotter( nnfw::Cluster* cl );
	~ClusterPlotter();
	
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	//! reset the plot zeroing all data registered so far
	void resetToZero();
	//! update the plot
	void updatePlot();
	//! resize the plots in such way that each plots fit the height specified
	//void fitToHeight( qreal hf );

public slots:
	//! show/hide outputs plot
	void showOutputs( bool );
	//! show/hide inputs plot
	void showInputs( bool );

signals:
	//! emitted when the height change
	void heightChanged();

protected:
// 	void mousePressEvent( QGraphicsSceneMouseEvent *event );
// 	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
// 	void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
	void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );

private:
	//! cluster to plot
	nnfw::Cluster* cl;
	//! bounding rect
	mutable QRectF brect;
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

	//! scale factor of data plotted
	QVector<qreal> myins;
	//! scale factor of data plotted
	QVector<qreal> myouts;

	//! Action for hide/show outputs
	QAction* showOutsA;
	//! Action for hide/show inputs
	QAction* showInsA;
	//! Popup menu
	QMenu* menu;

	bool showouts;
	bool showins;
	//! true when data is changed (useful for speedup boundingRect calculations)
	bool changed;
};

#endif
