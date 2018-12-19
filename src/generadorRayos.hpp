#include <cmath>
#include <cstdlib>
#include <tuple>
#include <cassert>
#include "Recta.hpp"
#include <random>
//#include <algorithm>

using namespace std;

/*int randomEntre(int l, int u)
{
	int r = std::trunc(std::rand());
	int res = (r % (u - l)) + l;
	assert(res <= u);
	assert(res >= l);
	return res;
}*/


double randomEntre(std::default_random_engine generator, double l, double u)
{
   
   //generator.seed(time(NULL));
   std::uniform_real_distribution<double> distribution(l,u);
   double res = distribution(generator);
   //cout << res << endl;
   return res;
}


tuple<double, double> randPos(default_random_engine generator, int lado, int filas, int columnas)
{
	double f, c;
	switch(lado)
	{
		case(0): // izquierdo
		{
			f = randomEntre(generator, 0, filas-1);
			c = 0;
			break;
		}
		case(1): // arriba
		{
			f = 0;
			c = randomEntre(generator, 0, columnas-1);
			break;
		}
		case(2): // derecha
		{
			f = randomEntre(generator, 0,filas-1);
			c = columnas-1;
			break;
		}
		case(3): // abajo
		{
			f = filas-1;
			c = randomEntre(generator,0,columnas-1);
			break;
		}
		default: //lado no esta en un valor valido
			break;
	}
	assert(f < filas);
	assert(c < columnas);
	return std::make_tuple(f,c);
}


tuple< double, Matriz<double> > simularRayo(Matriz<double>& imagen, int n, int m, double fe, double ce, double fs, double cs)
{
    // [t, D] = simularRayo(I, n, m, f_ini, c_ini, f_fin, c_fin, [dibujar])
    //
    // Simula un rayo del tomografo a traves de la imagen I, discretizada en
    // n filas y m columnas. El rayo tiene como punto inicial el punto
    // (f_ini, c_ini) y como punto final el punto (f_ini, c_ini).
    // Retorna: el tiempo total t de la emision del rayo y las distancias
    // recorridas D(i,j) en cada punto de la discretizacion.
    // El parametro dibujar es opcional y si vale 1 se dibuja el rayo sobre
    // la imagen.

	int filas = imagen.filas();
	int columnas = imagen.columnas();
	double df = filas/(double)n;
	double dc = columnas/(double)m;
	double t = 0;
	Matriz<double> D(n,m,0);
	
	//Calculo la recta que representa el rayo (y su inversa)

	if (fe == fs)
	{
		fe = fe - 0.5;
		fs = fs + 0.5;
	}
	if (ce == cs)
	{
		ce = ce - 0.5;
		cs = cs + 0.5;
	}

	Recta pi(fe, ce, fs, cs); 	
	Recta pj(ce, fe, cs, fs); 		

	double i1 = pj.evaluar(0);
	double i2 = pj.evaluar(columnas-1);
    int i_min = std::max(0, std::min(filas-2, (int)std::ceil(std::min(i1,i2) ) ) ); //cambie el 1 por un 0
    int i_max = std::max(0, std::min(filas-2, (int)std::ceil(std::max(i1,i2) ) ) ); //cambie el 1 por un 0

	// recorro cada fila (entre i_min e i_max) y veo que columnas toca el rayo

	for(int i = i_min; i <= i_max; i++)
	{
		double j1 = pi.evaluar(i-1);
		double j2 = pi.evaluar(i);
		int j_min = std::max(0, std::min(columnas-1, (int) std::ceil(std::min(j1,j2)))); //cambie el 1 por un 0
		int j_max = std::max(0, std::min(columnas-1, (int) std::ceil(std::max(j1,j2)))); //cambie el 1 por un 0
		for(int j = j_min; j <= j_max; j++)
		{
			t += imagen[i][j]+1;
			int n_i = min(n-1, (int) trunc((double)i/df));
			int m_j = min(m-1, (int) trunc((double)j/dc));
			D[n_i][m_j] = D[n_i][m_j] + 1; 
		}
	}
	
	return std::make_tuple(t, vectorizar(D));
}



tuple< Matriz<double>, Matriz<double> > generarRayosAzar(Matriz<double>& imagen, int m, int n, int k)
{
	srand(time(NULL));
	int filas = imagen.filas();
	int columnas = imagen.columnas();
	Matriz<double> rayos(k, n*m,0);
	Matriz<double> tiempos(k, 1);

	static std::default_random_engine generator;
	generator.seed(5446);


	for(int i = 0; i < k; i++)
	{
		//sale y entra resultan siempre distintos
		int entra = trunc(randomEntre(generator,0,3));

		int sale = (entra + (int)trunc(randomEntre(generator,0,2)) + 1) % 4;
		//cout << "entra " << entra << " sale " << sale << endl;
	
		auto posEntrada = randPos(generator, entra, filas, columnas);
		auto posSalida = randPos(generator, sale, filas, columnas);

		double fe = std::get<0>(posEntrada);
		double ce = std::get<1>(posEntrada);
		double fs = std::get<0>(posSalida);
		double cs = std::get<1>(posSalida);

		auto res = simularRayo(imagen, n, m, fe, ce, fs, cs);
		double t = std::get<0>(res);
		Matriz<double> simRayo = std::get<1>(res);
	    
		rayos.set_fil(i,simRayo);
		tiempos[i][0] = t;

	}
	return make_tuple(tiempos, rayos);
}


tuple< Matriz<double>, Matriz<double> > generarRayosCirculo(Matriz<double>& imagen, int m, int n, int k)
{
	srand(time(NULL));
	int filas = imagen.filas();
	int columnas = imagen.columnas();

	int cantXlados = k/4;
	Matriz<double> rayos(cantXlados*4, n*m,0);
	Matriz<double> tiempos(k, 1);

	double difCols = columnas/cantXlados;
	double difFils = filas/cantXlados;

	int entra = 0;
	int sale = 2;
	int rayo;

	cout << "ciruclo" << endl;

	double fs,cs,fe,ce,t;
	//comienza del lado izquierdo hacia el derecho
	for(int lado = 0; lado < 4; lado++)
	{
		if(lado == 0){
			fs = 0;
			cs = 0;
			fe = filas-1;
			ce = columnas-1;
			sale = 1;
			entra = 3;
		}
		if(lado == 1){
			fs = 0;
			cs = columnas-1;
			fe = filas-1;
			ce = 0;
			sale = 1;
			entra = 3;
		}
		if(lado == 2){
			fs = 0;
			cs = 0;
			fe = filas-1;
			ce = columnas-1;
			sale = 2;
			entra = 0;				
		}
		if(lado == 3){
			fs = filas-1;
			cs = columnas-1;
			fe = 0;
			ce = 0;
			sale = 3;
			entra = 1;
		}
		for(int j = 0; j < cantXlados; j++)
		{
			//sale y entra resultan siempre distintos
			auto res = simularRayo(imagen, n, m, fe, ce, fs, cs);
			t = std::get<0>(res);
			Matriz<double> simRayo = std::get<1>(res);
		    rayo = j + (lado*cantXlados);
			rayos.set_fil(rayo,simRayo);
			tiempos[rayo][0] = t;
			if(lado == 0){
				fs = fs - difFils;
				fe = fe + difFils;
			}
			if(lado == 1){
				cs = cs + difCols;
				ce = ce - difCols;
			}
			if(lado == 2){
				fe = fe - difFils;
				fs = fs + difFils;
			}
			if(lado == 3){
				ce = ce + difCols;
				cs = cs - difCols;
			}
		}
	}
	return make_tuple(tiempos, rayos);
}


tuple< Matriz<double>, Matriz<double> > generarRayos(Matriz<double>& imagen, int m, int n, int k, int metodo){

	if (metodo == 0)
	{
		return generarRayosAzar(imagen,m,n,k);
	}else{
		return generarRayosCirculo(imagen,m,n,k);
	}

}
