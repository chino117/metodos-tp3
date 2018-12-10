#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <limits>
#include "matriz_aux.hpp"
#include <cmath>
#include <ctime>
#include <algorithm>// max y transform
#include "IOUtils.hpp"
#include "cuadradosMinimos.hpp"
#include "generadorRayos.hpp"
#include "ruido.hpp"

using namespace std;

int main(int argc, char* argv[]){
	//Paso 1: Leer imagen
	IOUtils io(argc,argv);
	Matriz<double> imagen = io.leer_imagen();

    //cout << imagen << endl;


	//Paso 2: Simular el proceso tomografico
	auto tiemposRayos = generarRayos(imagen, io.discretizadoFilas, io.discretizadoColumnas, io.cantidadRayos); 

	Matriz<double> tiempos = std::get<0>(tiemposRayos);
	Matriz<double> D = std::get<1>(tiemposRayos);
  //cout << "D :" << D << endl;
	//Paso 3: Ruido aleatorio
	
	//Matriz<double> tiempoConRuido = generarRuido(tiempos);

	//Paso 4: Reconstruir el cuerpo original
	
	//cout << "Tiempos:"<<tiempos<< endl;
	//cout << "Tiempo con ruido:"<<tiempoConRuido<< endl;
	//cout << "D :" << D<< endl;

	Matriz<double> reconstruccion = resolverEN(D, tiempos);//TODO volver a tiemposConRuido
	
	cout << "Reconstruccion: \n" << reconstruccion<< endl;
    cout << "reconstruccion" << endl;

	io.escribir_imagen(reconstruccion);

    cout << "escribio_imagen" << endl;


/*
	Matriz<double> A(3,2,0);
    Matriz<double> b(3,1,0);
     
    Matriz<double> temp1(2, 1, 0);
    Matriz<double> temp2(2, 1, 0);
    Matriz<double> temp3(2, 1, 0); 
     
    temp1[0][0] = 2;
    temp1[1][0] = -1;
    temp2[0][0] = 1;
    temp2[1][0] = 2;
    temp3[0][0] = 1;
    temp3[1][0] = 1;
 
 
    A.set_fil(0,temp1);
    A.set_fil(1,temp2);
    A.set_fil(2,temp3);
 
    b[0][0]=2;
    b[1][0]=1;
    b[2][0]=4;

    cout << "|" << A[0][0] << " " << A[0][1] <<  " |"<< b[0][0]<< "|" << endl;
    cout << "|" << A[1][0] << " " << A[1][1] <<  " |" << b[1][0]<< "|" << endl;
    cout << "|" << A[2][0] << " " << A[2][1] <<  " |" << b[2][0]<< "|" << endl;

    Matriz<double> res = resolverEN(A,b);

    cout << "res:       " << endl;
    cout << "|" << res[0][0] << "|" << endl;
    cout << "|" << res[1][0] << "|" << endl;
*/

return 0;	
}
