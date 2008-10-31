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

#ifndef GRAPHICSCLUSTER_H
#define GRAPHICSCLUSTER_H
#include "types.h"

#include <QObject>
#include <QGraphicsItem>
#include <QPainterPath>

class ClusterModel;

/*!  \brief GraphicsCluster draw a ClusterModel
 *
 *  \par Description
 *  \par Warnings
 */
class FLEX_API GraphicsCluster : public QObject, public QGraphicsItem {
	Q_OBJECT
public:
	/*! Constructor */
	GraphicsCluster( ClusterModel* cl );
	/*! Return the model represented */
	ClusterModel* cluster() {
		return cl;
	};
	/*! \return the bounding rect of Cluster drawed */
	QRectF boundingRect() const;
	/*! \return the shape of the drawed Cluster */
	QPainterPath shape() const;
	/*! Paint the ClusterModel */
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	/*! set this Graphics to show the input or not */
	void markAsInput( bool );
	/*! set this Graphics to show the output or not */
	void markAsOutput( bool );

signals:
	/*! emitted when the position of this GraphicsCluster change */
	void positionChanged();

protected:
	/*! Reimplemented for ... */
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	/*! Reimplemented for ... */
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	/*! Reimplemented for ... */
	void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
	/*! Reimplemented for emitting corresponding signals */
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	ClusterModel* cl;
	int dim;
	int recw;
	int rech;
	int rectlx;
	int rectly;
	bool grabbed;
	/*! true if the Cluster is an input of the net */
	bool isInput;
	/*! true if the Cluster is an output of the net */
	bool isOutput;
};

#endif
