
#include "nnfw/nnfw.h"
#include "nnfw/types.h"
#include "simpletimer.h"
using namespace nnfw;

#include <iostream>
using namespace std;

int main( int , char** ) {
	SimpleTimer timer;

	timer.tic();
	DoubleVector a(5);
	a[0] = 1;
	a[1] = 3;
	a[2] = 5;
	a[3] = 7;
	a[4] = 9;
	DoubleVector b = a;
	b.detach();
	a.detach();
	a.steady(1);
	DoubleVector c = a+b+a*a+a;
	a += b+a*a+a;
	qDebug() << "------------" << timer.tic();
	qDebug() << a[0] << a[1] << a[2] << a[3] << a[4] ;
	qDebug() << c[0] << c[1] << c[2] << c[3] << c[4] ;

	qDebug() << "------------";
	int row = 2;
	int col = 3;
	DoubleMatrix m1( row, col );
	for( int i=0; i<row; i++ ) {
		for( int j=0; j<col; j++ ) {
			m1[i][j] = i+j;
		}
	}
	for( int i=0; i<row; i++ ) {
		for( int j=0; j<col; j++ ) {
			cout << m1[i][j] << " ";
		}
		cout << endl;
	}
	timer.tic();
	DoubleMatrix m2 = m1 + m1;
	m1.steady( row-1, col-1 );
	m1 %= m2;
	qDebug() << "------------" << timer.tic();

	for( int i=0; i<row; i++ ) {
		for( int j=0; j<col; j++ ) {
			cout << m2[i][j] << " ";
		}
		cout << endl;
	}
	qDebug() << "------------";
	for( int i=0; i<row; i++ ) {
		for( int j=0; j<col; j++ ) {
			cout << m1[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}
