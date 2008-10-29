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

#include <QApplication>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QDialog>
#include <QBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QDockWidget>
#include <QSlider>
#include <QCheckBox>
#include <QSpinBox>
#include <QScrollArea>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "mainWindow.h"
#include "neuralnetview.h"

using namespace nnfw;

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ), hasChanges(false), filename(), infoFile(), timer() {

    fileNewA = new QAction( QIcon(":/fileNew.png"), tr( "New" ), this );
    fileNewA->setShortcut( tr("Ctrl+N") );
    fileNewA->setStatusTip( tr("Create a New NeuralNetwork") );
    fileNewA->setCheckable( false );
    connect( fileNewA, SIGNAL( triggered() ),
            this, SLOT( fileNew() ) );

    fileLoadA = new QAction( QIcon(":/fileOpen.png"), tr( "Open" ), this );
    fileLoadA->setShortcut( tr("Ctrl+O") );
    fileLoadA->setStatusTip( tr("Load a NeuralNetwork from file") );
    fileLoadA->setCheckable( false );
    connect( fileLoadA, SIGNAL( triggered() ),
            this, SLOT( fileLoad() ) );

    fileCloseA = new QAction( QIcon(":/fileClose.png"), tr( "Close" ), this );
    fileCloseA->setShortcut( tr("Ctrl+W") );
    fileCloseA->setStatusTip( tr("Close NeuralNetwork") );
    fileCloseA->setCheckable( false );
    connect( fileCloseA, SIGNAL( triggered() ),
            this, SLOT( fileClose() ) );

    fileSaveA = new QAction( QIcon(":/fileSave.png"), tr( "Save" ), this );
    fileSaveA->setShortcut( tr("Ctrl+S") );
    fileSaveA->setStatusTip( tr("Save NeuralNetwork") );
    fileSaveA->setCheckable( false );
    connect( fileSaveA, SIGNAL( triggered() ),
            this, SLOT( fileSave() ) );

    fileSaveasA = new QAction( QIcon(":/fileSaveas.png"), tr( "Save as ..." ), this );
    fileSaveasA->setShortcut( tr("Ctrl+S") );
    fileSaveasA->setStatusTip( tr("Save NeuralNetwork with differente name") );
    fileSaveasA->setCheckable( false );
    connect( fileSaveasA, SIGNAL( triggered() ),
            this, SLOT( fileSaveas() ) );

    QAction* showCreditsA = new QAction( tr( "Credits" ), this );
    connect( showCreditsA, SIGNAL( triggered() ),
            this, SLOT( credits() ) );

    // ------- TOOLBAR
    fileT = addToolBar( tr("Views") );
    fileT->addAction( fileNewA );
    fileT->addAction( fileLoadA );
    fileT->addAction( fileCloseA );
    fileT->addAction( fileSaveA );
    fileT->addAction( fileSaveasA );
	fileT->setMovable( false );

    // ------- MENU
    QMenuBar* bar = menuBar();
    QMenu* menu = bar->addMenu( tr("File") );
    menu->addAction( fileNewA );
    menu->addAction( fileLoadA );
    menu->addAction( fileCloseA );
    menu->addAction( fileSaveA );
    menu->addAction( fileSaveasA );

    QMenu* mviews = bar->addMenu( tr("Views") );

    menu = bar->addMenu( tr("&About") );
    menu->addAction( showCreditsA );

	statusBar()->setStyleSheet( "::item { border: 0px }" );
	statusBar()->showMessage( tr("Ready") );

	centre = new NeuralNetView( this );
	nn = 0;
	centre->setNeuralNet( 0 );
	setCentralWidget( centre );

	createBrowser();
	createPlotter();

	// --- add view/hide checkboxes in the view menu for Toolbars/DockWidgets
	QMenu* tmp = createPopupMenu();
	QList<QAction*> acts = tmp->actions();
	foreach( QAction* a, acts ) {
		mviews->addAction( a );
	}

    fileSaveA->setEnabled( false );
    fileSaveasA->setEnabled( false );
    fileCloseA->setEnabled( false );

	connect( &timer, SIGNAL( timeout() ),
			this, SLOT( randStep() ) );
}

void MainWindow::fileNew() {
    if ( hasChanges ) {
        switch( QMessageBox::warning( this, tr("NNFW Editor"), tr("Save changes to the current neural network?? <br><br>"), tr("Save"), tr("Cancel"), tr("Don't save"), 0, 1 ) ) {
        case 0:
            if ( fileSave() ) {
                break;
            }
            return;
        case 1:
            return;
        case 2:
            break;
        }
    }
	filename = QString();
	if ( !nn ) delete nn;
	nn = new BaseNeuralNetModel( new BaseNeuralNet() );
	// --- display it
	centre->setNeuralNet( nn );
	browse->setNeuralNet( nn );
	plotter->setNeuralNet( nn );
    fileSaveA->setEnabled( true );
    fileSaveasA->setEnabled( true );
    fileCloseA->setEnabled( true );
}

void MainWindow::fileLoad() {
    if ( hasChanges ) {
        switch( QMessageBox::warning( this, tr("NNFW Editor"), tr("Save changes to the current neural network?? <br><br>"), tr("Save"), tr("Cancel"), tr("Don't save"), 0, 1 ) ) {
        case 0:
            if ( fileSave() ) {
                break;
            }
            return;
        case 1:
            return;
        case 2:
            break;
        }
    }
    // --- ask for a file
    QString newf = QFileDialog::getOpenFileName( this, tr("Open Document from..."), infoFile.absolutePath(), tr("Neural Network (*.nnx);;All Types (*)" ) );
    if ( newf.isNull() ) {
        return;
    }
    filename = newf;
    infoFile = QFileInfo( filename );
	if ( !nn ) delete nn;
	// --- open the neural network
	nn = new BaseNeuralNetModel( filename );
	// --- display it
	centre->setNeuralNet( nn );
	browse->setNeuralNet( nn );
	plotter->setNeuralNet( nn );
    fileSaveA->setEnabled( true );
    fileSaveasA->setEnabled( true );
    fileCloseA->setEnabled( true );

	//--- codice temporaneo per prova di ClusterPlotter
	steps = 0;
	timer.start( 0 );
	//--- fine codice temporaneo per prova di ClusterPlotter

}

void MainWindow::fileClose() {
    if ( hasChanges ) {
        switch( QMessageBox::warning( this, tr("NNFW Editor"), tr("Save changes to the current neural network?? <br><br>"), tr("Save"), tr("Cancel"), tr("Don't save"), 0, 1 ) ) {
        case 0:
            if ( fileSave() ) {
                break;
            }
            return;
        case 1:
            return;
        case 2:
            break;
        }
    }
	// --- empty the display
	centre->setNeuralNet( 0 );
	browse->setNeuralNet( 0 );
	plotter->setNeuralNet( 0 );
    fileSaveA->setEnabled( false );
    fileSaveasA->setEnabled( false );
    fileCloseA->setEnabled( false );
}

bool MainWindow::fileSave() {
    if ( filename.isNull() ) {
        if ( !askSaveFilename() ) {
            return false;
        }
    }
	return nn->saveTo( filename );
}

bool MainWindow::fileSaveas() {
	if ( !askSaveFilename() ) {
		return false;
	}
	return nn->saveTo( filename );
}

void MainWindow::credits() {
    QDialog* dialog = new QDialog( this );
    QBoxLayout* lay = new QVBoxLayout( dialog );
    QLabel* lb = new QLabel( dialog );
    //lb->setPixmap( QPixmap( ":/Credits.png" ).scaled( 640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
	lb->setText( "<h1>NNFW Visual Editor</h1> <br> by G. Massera, copyright 2007-2008 <br>" );
    lay->addWidget( lb );
    QPushButton* bt = new QPushButton( tr("Ok"), dialog );
    lay->addWidget( bt );
    connect( bt, SIGNAL( clicked() ),
            dialog, SLOT( accept() ) );
    dialog->exec();
}

bool MainWindow::askSaveFilename() {
    // --- ask for a file 
    QString newf;
    QFileDialog* filed;
    QString path = infoFile.absolutePath();
    if ( path == QString("") ) {
        path = QCoreApplication::applicationDirPath();
    }
	filed = new QFileDialog( this, tr("Save Neural Network in..."), path, "Neural Network (*.nnx)" );
	filed->setFileMode( QFileDialog::AnyFile );
	filed->setDefaultSuffix( "nnx" );
	filed->setAcceptMode( QFileDialog::AcceptSave );
	if ( filed->exec() ) {
		newf = filed->selectedFiles()[0];
	}
	delete filed;
    if ( newf.isNull() ) {
        return false;
    }
    filename = newf;
    infoFile = QFileInfo( filename );
    return true;
}

void MainWindow::createBrowser() {
/*	QDockWidget* fb = new QDockWidget( tr("Browser"), this );
	fb->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	addDockWidget( Qt::RightDockWidgetArea, fb );
	// --- create the content
	browse = new FBrowser( fb );
	fb->layout()->addWidget( browse );*/
}

void MainWindow::createPlotter() {
/*	QDockWidget* fb = new QDockWidget( tr("Plotter"), this );
	addDockWidget( Qt::BottomDockWidgetArea, fb );
	// --- create the content
	plotter = new NNPlotter( fb );
	fb->layout()->addWidget( plotter );*/
}
