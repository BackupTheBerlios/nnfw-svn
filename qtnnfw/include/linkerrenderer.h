
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

protected:
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
    ClusterRenderer *source, *dest;

	QPainterPath path;
	qreal arrowSize; 
};

#endif
