#ifndef NNRENDERER_H
#define NNRENDERER_H

#include <QGraphicsView>
#include <QMap>

#include "nnfw/nnfw.h"

class ClusterRenderer;
class LinkerRenderer;

class NNRenderer : public QGraphicsView {
	Q_OBJECT
public:
	NNRenderer( QWidget* parent = 0 );

	//! set the neuralnet to display
	void setNeuralNet( nnfw::BaseNeuralNet* nn );
	//! return the neuralnet displayed
	nnfw::BaseNeuralNet* getNeuralNet() {
		return nn;
	};
public slots:
	//! calculate the position of Clusters in order to see inputs to the bottom and output in top
	void defaultPositioning();
	//! recalculate the bounding rect of scene
	void fitSceneRect();

protected:
	void keyPressEvent(QKeyEvent *event);
	void wheelEvent(QWheelEvent *event);
	void drawBackground(QPainter *painter, const QRectF &rect);
	
	void scaleView(qreal scaleFactor);
	
private:
	//! when there is no neuralnet to display the zoom-in/out is disabled
	bool scaleDisabled;
	nnfw::BaseNeuralNet* nn;
	//--- mappings
	QMap<nnfw::Cluster*, ClusterRenderer*> clmap;
	QMap<nnfw::Linker*, LinkerRenderer*> lkmap;
};

#endif
