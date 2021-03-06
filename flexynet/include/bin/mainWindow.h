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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QFileInfo>
#include <QTimer>

class QAction;
class QToolBar;
class QLabel;
class QProgressBar;
class QBoxLayout;
class QSlider;
class QCheckBox;
class QActionGroup;
class NeuralNetView;
class BaseNeuralNetModel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow( QWidget* parent = 0 );
	//~MainWindow();

public slots:
	void fileNew();
	void fileLoad();
	bool fileSave();
	bool fileSaveas();
	void fileClose();

	void addCluster();
	void addLinker();

    void credits();

private:
	bool askSaveFilename();
	void createBrowser();
	void createPlotter();

	QAction* fileNewA;
	QAction* fileLoadA;
	QAction* fileSaveA;
	QAction* fileSaveasA;
	QAction* fileCloseA;

	QAction* addClusterA;
	QAction* addLinkerA;

	QAction* showCreditsA;

	//--- central Widget
	NeuralNetView* centre;

	//--- DockWidgets
	
	//--- current NeuralNet opened
	BaseNeuralNetModel* nn;

	QString filename;
	QFileInfo infoFile;
};

#endif
