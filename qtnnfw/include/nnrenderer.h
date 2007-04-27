#ifndef NNRENDERER_H
#define NNRENDERER_H

#include <QGraphicsView>
#include <QMap>

#include "nnfw/nnfw.h"
using namespace nnfw;

class ClusterRenderer;
class LinkerRenderer;

class NNRenderer : public QGraphicsView {
	Q_OBJECT
public:
	NNRenderer( QWidget* parent = 0 );

	//! set the neuralnet to display
	void setNeuralNet( BaseNeuralNet* nn );
	//! return the neuralnet displayed
	BaseNeuralNet* getNeuralNet() {
		return nn;
	};
public slots:
	//! calculate the position of Clusters in order to see inputs to the bottom and output in top
	void defaultPositioning();

protected:
	void keyPressEvent(QKeyEvent *event);
	void wheelEvent(QWheelEvent *event);
	void drawBackground(QPainter *painter, const QRectF &rect);
	
	void scaleView(qreal scaleFactor);
	
private:
	//! when there is no neuralnet to display the zoom-in/out is disabled
	bool scaleDisabled;
	BaseNeuralNet* nn;
	//--- mappings
	QMap<Cluster*, ClusterRenderer*> clmap;
	QMap<Linker*, LinkerRenderer*> lkmap;
};

#endif
