
#include "nnfw/nnfw.h"
#include "nnfw/types.h"
#include "nnfw/vectors.h"
#include "nnfw/utils.h"

using namespace nnfw;

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

	timer.tic();
	RealVec A(5);
	A[0] = 1;
	A[1] = 3;
	A[2] = 5;
	A[3] = 7;
	A[4] = 9;
	RealVec B = A;
	RealVec C = A;
	C *= A;
	A += A;
	A += C;
	A += B;
	qDebug() << "------------" << timer.tic();
	qDebug() << A[0] << A[1] << A[2] << A[3] << A[4] ;
	qDebug() << C[0] << C[1] << C[2] << C[3] << C[4] ;

	return 0;
}
