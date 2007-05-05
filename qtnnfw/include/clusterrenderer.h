
#ifndef CLUSTERRENDERER_H
#define CLUSTERRENDERER_H

#include "nnfw/cluster.h"
#include <QObject>
#include <QGraphicsItem>
#include <QPainterPath>

class LinkerRenderer;
class NNRenderer;

class ClusterRenderer : public QObject, public QGraphicsItem {
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
