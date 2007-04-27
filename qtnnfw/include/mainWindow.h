
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QFileInfo>

class QAction;
class QToolBar;
class QLabel;
class QProgressBar;
class QBoxLayout;
class QSlider;
class QCheckBox;
class QActionGroup;
class NNRenderer;

#include "nnfw/nnfw.h"
using namespace nnfw;

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

    void credits();

private:
	bool askSaveFilename();

	QAction* fileNewA;
	QAction* fileLoadA;
	QAction* fileSaveA;
	QAction* fileSaveasA;
	QAction* fileCloseA;

	QAction* showCreditsA;

    // --- Toolbar file
    QToolBar* fileT;

	//--- central Widget
	NNRenderer* centre;
	
	//--- current NeuralNet opened
	BaseNeuralNet* nn;

	bool hasChanges;
	QString filename;
	QFileInfo infoFile;
};

#endif
