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

#ifndef NNPLOTTER_H
#define NNPLOTTER_H

#include <QGraphicsView>
#include <QVector>

#include "nnfw/nnfw.h"
#include "fnnwrapper.h"

class ClusterPlotter2;

class NNPlotter : public QGraphicsView {
	Q_OBJECT
public:
	NNPlotter( QWidget* parent = 0 );

	//! set the neuralnet to display
	void setNeuralNet( FNNWrapper* nn );
	//! return the neuralnet displayed
	FNNWrapper* getNeuralNet() {
		return nn;
	};
public slots:
	//! update the plots
	void updatePlots();
	//! update positions
	void updatePositions();

protected:
	void keyPressEvent(QKeyEvent *event);
	void wheelEvent(QWheelEvent *event);
	void drawBackground(QPainter *painter, const QRectF &rect);
	
	void scaleView(qreal scaleFactor);
	
private:
	//! when there is no neuralnet to display the zoom-in/out is disabled
	bool scaleDisabled;
	FNNWrapper* nn;
	//! ClusterPlotter inserted
	QVector<ClusterPlotter2*> pls;
};

#endif
