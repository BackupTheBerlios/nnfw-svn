
#ifndef CLUSTERRENDERER_H
#define CLUSTERRENDERER_H

#include "nnfw/cluster.h"
#include <QObject>
#include <QGraphicsItem>

class LinkerRenderer;

class ClusterRenderer : public QObject, public QGraphicsItem {
	Q_OBJECT
public:
	ClusterRenderer( nnfw::Cluster* cl );
	
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void addLinker( LinkerRenderer* lr );

signals:
	//! emitted when is moving
	void moved();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	nnfw::Cluster* cl;
	int dim;
	int recw;
	int rech;
	int rectlx;
	int rectly;
	QVector<LinkerRenderer*> lks;
};

#endif
