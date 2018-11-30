#include "matriz_aux.hpp"
#include "eigen.hpp"
#include <cassert>
#include <cmath>

void svd(Matriz<double>& A, Matriz<double>& U, Matriz<double>& sigma, Matriz<double>& Vt){

	Matriz<double> AtA = (A.traspuesta())*A;
	cout << "post AtA" << " size: " << AtA.filas() << " X " << AtA.columnas() << endl;
	Matriz<double> autoval (A.columnas(), 1, 0.0);
	Matriz<double> V = (obtener_autovectores(AtA, autoval, 5000, 0.000000001));
	Vt = V.traspuesta();
	
	for (int i = 0; i < A.columnas(); ++i)
	{
		Matriz<double> Ui =(A*V.copy_col(i))*autoval.copy_col(i);
		U.set_col(i,Ui);
	}

	for (int i = 0; i < sigma.columnas(); ++i)
	{
		sigma[i][i] = sqrt(autoval[i][1]);
	}



}

void pseudoinverse(Matriz<double>& sigma){

	sigma = sigma.traspuesta();
	for (int i = 0; i < sigma.columnas(); ++i)
	{
		sigma[i][i] = 1/sigma[i][i];
	}
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

	Matriz<double> res (A.columnas(), 1, 0.0);

	res = (U.traspuesta())*b;

	pseudoinverse(sigma);

	res = sigma*res;
	res = Vt.traspuesta()*res;


	//Matriz<double> At = A.traspuesta();
	//Matriz<double> Ata = At*(A);
	//Matriz<double> Atb = At*(b);
	//cout << "Ata " << Ata << endl;
	//cout << "Atb " << Atb << endl; 
	//Matriz<double> x = resolver_gauss(Ata, Atb);
	return res;
}



