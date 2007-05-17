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

#ifndef CLUSTERRENDERER_H
#define CLUSTERRENDERER_H
#include "types.h"

#include "nnfw/cluster.h"
#include <QGraphicsItem>
#include <QPainterPath>

class LinkerRenderer;
class NNRenderer;

class FLEX_API ClusterRenderer : public QGraphicsItem {
public:
	ClusterRenderer( NNRenderer* nnrenderer, nnfw::Cluster* cl );
	
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void addLinker( LinkerRenderer* lr );

protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	nnfw::Cluster* cl;
	int dim;
	int recw;
	int rech;
	int rectlx;
	int rectly;
	QVector<LinkerRenderer*> lks;
	NNRenderer* nnr;
	bool grabbed;
};

#endif
