
#ifndef LINKERRENDERER_H
#define LINKERRENDERER_H

#include "linkerrenderer.h"
#include <QGraphicsItem>
#include <QPainterPath>

class ClusterRenderer;

class LinkerRenderer : public QGraphicsItem {
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
