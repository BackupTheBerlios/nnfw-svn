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

#ifndef CLUSTERPLOTTER2_H
#define CLUSTERPLOTTER2_H
#include "types.h"

#include "nnfw/cluster.h"
#include "fnnwrapper.h"
#include <QGraphicsItem>
#include <QObject>
#include <QVarLengthArray>

class QAction;
class QMenu;

class FLEX_API ClusterPlotter2 : public QObject, public QGraphicsItem {
	Q_OBJECT
public:
	ClusterPlotter2( nnfw::Cluster* cl );
	~ClusterPlotter2();
	
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	//! reset the plot zeroing all data registered so far
	void resetToZero();
	//! update the plot
	void updatePlot();

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
	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
	void hoverMoveEvent( QGraphicsSceneHoverEvent * event );

private:
	//! Recalculate the BoundingRect
	void recalcBRect();

	typedef QVarLengthArray< QVarLengthArray<qreal,1024> > DataVec;
	//! cluster to plot
	nnfw::Cluster* cl;
	//! dimension of Cluster and DataVec
	int dim;
	//! bounding rect
	QRectF brect;
	//! input data plotted
	DataVec ins;
	//! output data plotted
	DataVec outs;
	//! low Y value of ins data plotted
	QVarLengthArray<qreal> lowins;
	//! high Y value of ins data plotted
	QVarLengthArray<qreal> highins;
	//! low Y value of outs data plotted
	QVarLengthArray<qreal> lowouts;
	//! high Y value of outs data plotted
	QVarLengthArray<qreal> highouts;
	//! lastPoint; the dimension of DataVecs
	int lastPoint;

	//! Action for hide/show outputs
	QAction* showOutsA;
	//! Action for hide/show inputs
	QAction* showInsA;
	//! Popup menu
	QMenu* menu;

	bool showouts;
	bool showins;

	//--- for displaying the legend
	int legendId;
	int legendT;
	bool legendTop;
	bool legendActive;
};

#endif
