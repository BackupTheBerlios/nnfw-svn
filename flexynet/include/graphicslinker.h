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

#ifndef GRAPHICSLINKER_H
#define GRAPHICSLINKER_H
#include "types.h"

#include <QObject>
#include <QGraphicsItem>
#include <QPainterPath>

class LinkerModel;
class GraphicsCluster;

/*!  \brief GraphicsLinker draw a LinkerModel
 *
 *  \par Description
 *  \par Warnings
 */
class FLEX_API GraphicsLinker : public QObject, public QGraphicsItem {
	Q_OBJECT
public:
	/*! Constructor */
	GraphicsLinker( LinkerModel* lk, GraphicsCluster* from, GraphicsCluster* to );
	/*! Return the LinkerModel represented */
	LinkerModel* linker() {
		return lk;
	};
	/*! Return the cluster model From */
	GraphicsCluster* from() {
		return fromv;
	};
	/*! Return the cluster model To */
	GraphicsCluster* to() {
		return tov;
	};
	/*! \return the bounding rect */
	QRectF boundingRect() const;
	/*! \return the shape drawed */
	QPainterPath shape() const;

public slots:
	/*! Adjust the line drawed after the movement of GraphicsCluster */
	void adjust();

protected:
	/*! reimplemted */
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
	/*! Rotate the arrow aligning it with the QLineF passed */
	void adjustArrow( QLineF );
	
	LinkerModel* lk;
	GraphicsCluster* fromv;
	GraphicsCluster* tov;

	/*! Only the curve Path */	
	QPainterPath path;
	/*! The triangle composing the arrow */
	QPainterPath arrowPath;
	/*! the size of the arrow */
	qreal arrowSize; 
};

#endif
