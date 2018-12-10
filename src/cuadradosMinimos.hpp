#include "matriz_aux.hpp"
#include "eigen.hpp"
#include <cassert>
#include <math.h>

void pseudoinverse(Matriz<double>& sigma){

	sigma = sigma.traspuesta();
	for (int i = 0; i < sigma.filas(); ++i)
	{
		sigma[i][i] = (double)1.0/sigma[i][i];
		//cout << sigma[i][i] << endl;
	}
}


void svd(Matriz<double>& A, Matriz<double>& U, Matriz<double>& sigma, Matriz<double>& Vt){

	Matriz<double> AtA = (A.traspuesta())*A;
	//cout << AtA << endl;
	cout << "post AtA" << " size: " << AtA.filas() << " X " << AtA.columnas() << endl;
	Matriz<double> autoval (A.columnas(), 1, 0.0);
	Matriz<double> V = (obtener_autovectores(AtA, autoval, 100000, 0.000000001));

	/*for (int i = 0; i < A.columnas(); ++i)
	{
		Matriz<double> Ui =(A*V.copy_col(i))*autoval.copy_col(i);
		U.set_col(i,Ui);
	}*/

	cout << " Autoval" << autoval << endl;;

	for (int i = 0; i < sigma.columnas(); ++i)
	{
		sigma[i][i] = sqrt(autoval[i][0]);
		//cout << sigma[i][i] << endl;
	}
	cout << "hola" << endl;
	pseudoinverse(sigma);
	cout << "hola1" << endl;
	U = A*(V*sigma);
	cout << "hola2" << endl;
	Vt = V.traspuesta();
	cout << "hol3" << endl;
	//cout << sigma << endl;

}



Matriz<double> resolverEN(Matriz<double>& A, Matriz<double>& b)
{
	//assert(A.rango() < A.columnas()); //como es SDP es inversible 
	//
	Matriz<double> U (A.filas(), A.filas(), 0.0);
	Matriz<double> sigma (A.filas(), A.columnas());
	Matriz<double> Vt (A.columnas(), A.columnas());
	cout << "svd" << endl;
	svd(A,U,sigma,Vt);

	//cout << "U" << U << endl;
	//cout << "Vt" << Vt << endl;
	//cout << sigma << endl;

	Matriz<double> res (A.columnas(), 1, 0.0);

	cout << "hola4" << endl;
	res = (U.traspuesta())*b;

	//pseudoinverse(sigma);
	cout << "hol5" << endl;

	res = sigma*res;
	cout << "hola6" << endl;

	res = Vt.traspuesta()*res;
	cout << "hola7" << endl;


	//Matriz<double> At = A.traspuesta();
	//Matriz<double> Ata = At*(A);
	//Matriz<double> Atb = At*(b);
	//cout << "Ata " << Ata << endl;
	//cout << "Atb " << Atb << endl; 
	//Matriz<double> x = resolver_gauss(Ata, Atb);
	return res;
}



