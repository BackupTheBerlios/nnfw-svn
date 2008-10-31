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

#ifndef NEURALNETVIEW_H
#define NEURALNETVIEW_H
#include "types.h"

#include <QGraphicsView>
#include <QMap>

class GraphicsCluster;
class GraphicsLinker;
class BaseNeuralNetModel;
class ClusterModel;
class LinkerModel;

/*! \brief QWidget for visualize a NeuralNetworkModel
 *
 * \par Description
 * \par Warnings
 */
class FLEX_API NeuralNetView : public QGraphicsView {
	Q_OBJECT
public:
	/*! Constructor */
	NeuralNetView( QWidget* parent = 0 );
	
	/*! set the neuralnet to display */
	void setNeuralNet( BaseNeuralNetModel* nn );
	/*! return the neuralnet displayed */
	BaseNeuralNetModel* neuralNet() {
		return nn;
	};
public slots:
	/*! calculate the position of Clusters in order to see inputs to the bottom and output in top */
	void defaultPositioning();
	/*! adjust SceneRect */
	void adjustSceneRect();

protected:
	/*! Re-implemented for ... */
	void keyPressEvent(QKeyEvent *event);
	/*! Re-implemented for ... */
	void wheelEvent(QWheelEvent *event);
	/*! ... */
	void drawBackground(QPainter *painter, const QRectF &rect);
	/*! ... */
	void scaleView(qreal scaleFactor);

private slots:
	/*! Create and place a new GraphicsCluster for ClusterModel added */
	void addGraphicsCluster( ClusterModel*, bool, bool );
	/*! Remove the ClusterGraphics associated to the ClusterModel passed */
	void delGraphicsCluster( ClusterModel* );

	/*! Create and place a new GraphicsCluster for ClusterModel added */
	void addGraphicsLinker( LinkerModel* );
	/*! Remove the ClusterGraphics associated to the ClusterModel passed */
	void delGraphicsLinker( LinkerModel* );

private:
	/*! when there is no neuralnet to display the zoom-in/out is disabled */
	bool scaleDisabled;
	BaseNeuralNetModel* nn;
	//--- mappings
	QMap<ClusterModel*, GraphicsCluster*> clmap;
	QMap<LinkerModel*, GraphicsLinker*> lkmap;
};

#endif
