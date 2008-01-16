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

#ifndef LINKERRENDERER_H
#define LINKERRENDERER_H
#include "types.h"

#include "linkerrenderer.h"
#include <QGraphicsItem>
#include <QPainterPath>

class ClusterRenderer;

class FLEX_API LinkerRenderer : public QGraphicsItem {
public:
	LinkerRenderer(ClusterRenderer *sourceNode, ClusterRenderer *destNode);
	void adjust();
	QRectF boundingRect() const;
	QPainterPath shape() const;

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
	void adjustArrow( QLineF );

    ClusterRenderer *source, *dest;

	QPainterPath path;
	QPainterPath arrowPath;
	qreal arrowSize; 
};

#endif
