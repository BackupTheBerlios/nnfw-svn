
#include <QtGui>

#include "mainWindow.h"

int main( int argc, char* argv[] ) {
	QApplication app(argc, argv);
	
	MainWindow* main = new MainWindow();
	main->show();
	main->resize( 800, 800 );
	return app.exec();
}

