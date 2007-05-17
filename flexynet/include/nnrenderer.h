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

#ifndef NNRENDERER_H
#define NNRENDERER_H
#include "types.h"

#include <QGraphicsView>
#include <QMap>

#include "nnfw/nnfw.h"
#include "fnnwrapper.h"

class ClusterRenderer;
class LinkerRenderer;

class FLEX_API NNRenderer : public QGraphicsView {
	Q_OBJECT
public:
	NNRenderer( QWidget* parent = 0 );

	//! set the neuralnet to display
	void setNeuralNet( FNNWrapper* nn );
	//! return the neuralnet displayed
	FNNWrapper* getNeuralNet() {
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
	FNNWrapper* nn;
	//--- mappings
	QMap<nnfw::Cluster*, ClusterRenderer*> clmap;
	QMap<nnfw::Linker*, LinkerRenderer*> lkmap;
};

#endif
