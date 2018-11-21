#ifndef MATRIZ_AUX_HPP
#define MATRIZ_AUX_HPP

#include "matriz.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <stdlib.h>

#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

//Auxiliares de matrices

Matriz<double> vectorizar(const Matriz<double>& a)
{
	//Devuelve la matriz como un vector columna
	int dimensionVector = a.filas() * a.columnas();
	Matriz<double> res(dimensionVector, 1);
	for(int i = 0; i < a.filas(); i++)
	{
		for(int j = 0; j < a.columnas(); j++)
		{
			res[(i*a.columnas())+j][0] = a[i][j];
		}
	}
	return res;
}

//Compara dos doubles, viendo que sean menores que un cierto multiplo de
//una constante de error provista por la STL
bool comp_doubles(double a, double b,double mul_tolerancia)
{
   return (std::abs(a-b) < mul_tolerancia*std::numeric_limits<double>::epsilon());
}


bool comp_doubles_epsilon(double a, double b,double epsilon)
{
   return (std::abs(a-b) < epsilon);
}
//Compara dos matrices, posición por posición, utilizando comp_doubles
bool son_parecidas(const Matriz<double>& a, const Matriz<double>& b, 
                   double mul_tolerancia = 1.0f)
{
  if (a.filas() != b.filas() || a.columnas() != b.columnas())
     return false;

  for(int i = 0;i < a.filas();i++)
  {
     for(int j = 0;j < a.columnas();j++)
     {
        if (!comp_doubles(a[i][j], b[i][j], mul_tolerancia))
           return false;
     }
  }  
  return true;
}
//Compara dos matrices, posición por posición, utilizando comp_doubles
bool son_parecidas_epsilon(const Matriz<double>& a, const Matriz<double>& b, 
                   double epsilon)
{
  if (a.filas() != b.filas() || a.columnas() != b.columnas())
     return false;

  for(int i = 0;i < a.filas();i++)
  {
     for(int j = 0;j < a.columnas();j++)
     {
        if (!comp_doubles_epsilon(a[i][j], b[i][j], epsilon))
           return false;
     }
  }  
  return true;
}
//Muestra una matriz vector como una matriz de cant_col columnas
void mostrar_vec_como_matriz(const Matriz<double>& a,int cant_col)
{
   
   for(int i = 0;i < a.filas()/cant_col;i++)
   {
      std::cout<<"[ ";
      for(int j = 0;j < cant_col;j++)
      {
        std::cout<<a[j+i*cant_col][0]<<" ";
      }
      std::cout<<"]\n";
   }

}

bool es_ts(const Matriz<double>& a){
  for(int i = 0;i < a.filas();i++)
  {
    for(int j = 0;j <a.columnas();j++)
    {
    if(!comp_doubles_epsilon(a[i][j],0,0.0000001))
      if(j < i){
        return false;
      }
    }
  }
  return true; 
}
//Muestra una matriz, ignorando las posiciones que valen 0.
//Los elementos mostrados tienen la forma (valor_elemento, columna_elemento)
void mostrar_matriz_sin_ceros(const Matriz<double>& a)
{
    for(int i = 0;i < a.filas();i++)
    {
       std::cout<<"[";
       for(int j = 0;j <a.columnas();j++)
       {
 	    if(!comp_doubles_epsilon(a[i][j],0,0.0000001))
	       std::cout<<"("<<a[i][j]<<","<<j<<") ";
       }
       std::cout<<"]\n";
    }
}

//Devuelve una matriz nxn diagonal, armada con los valores de datos
Matriz<double> diag(int n, double datos[])
{
    assert(n > 0);
    Matriz<double> res(n,n,0);
    for(int i = 0;i < n;i++)
          res[i][i] = datos[i];
    return res;
}

//Devuelve la matriz identidad nxn
Matriz<double> id(int n)
{
    assert(n > 0);
    Matriz<double> res(n,n,0);
    for(int i = 0;i < res.filas();i++)
        res[i][i] = 1.0;
    return res;
}

//matriz m*n de unos en la diagonal
Matriz<double> unos(int m,int n)
{
    assert(m > 0);
    assert(n > 0);
    Matriz<double> res(m,n,0);
    for(int i = 0;i < res.filas();i++)
        if(i < n)
          res[i][i] = 1.0;
    return res;
}

//Devuelve la matriz (A:b) de dimenensiones nx(m1+m2)
Matriz<double> augmentar(const Matriz<double>& a, const Matriz<double>& b)
{
    assert(a.filas() == b.filas());
    Matriz<double> res(a.filas(), a.columnas()+b.columnas());
    for(int i = 0;i < a.filas();i++)
    {
        for(int j = 0;j < a.columnas();j++)
        {
           res[i][j] = a[i][j];
        }
	for(int j = 0;j < b.columnas();j++)
        {
           res[i][j+a.columnas()] = b[i][j];
        }
    }
    return res;
}

//Metodos de triangulación

//Suma a la fila a[to] la fila a[from] multiplicada por coef y dividida
//por coef2
void suma_filas(Matriz<double>& a, int to, int from, double coef,double coef2)
{
    for(int j = 0;j < a.columnas();j++)
        a[to][j] += (coef*(a[from][j]/coef2));
}

//Triangula superiormente la matriz a, modificandola
void triangular_sup(Matriz<double>& a)
{
    for(int i = 0;i <= a.filas()-1;i++)
    {
        for(int j = i+1;j < a.filas();j++)
        {
           suma_filas(a,j,i,-a[j][i],a[i][i]); 
        }
    }
}

//Triangular superiormente la matriz a,modificandola, y 
//devuelve la triangulacion inferior de a
Matriz<double> triangular_sup_inf(Matriz<double>& a)
{
    assert(a.filas() == a.columnas());
    Matriz<double> inf = id(a.filas());
    double coef = 0.0;
    for(int i = 0;i <= a.filas()-1;i++)
    {
        for(int j = i+1;j < a.filas();j++)
        {
           coef = -(a[j][i]/a[i][i]);
	   suma_filas(a,j,i,-a[j][i],a[i][i]); 
	   inf[j][i] = -coef;
        }
    }
    return inf;
}

//Resuelve el sistema, devolviendo un vector(Matriz<double> n x 1) que contiene el valor
//de cada incognita. Asume que la matriz de entrada ya esta 
//triangulada superiormente y que es una matriz augmentada de la forma (A:b)
Matriz<double> resolver_sistema(const Matriz<double>& a)
{
  cout << "resolviendo" << endl;
   Matriz<double> incognitas(a.columnas()-1,1,0.0);
   for(int j = a.filas()-1;j >= 0;j--)
   {
      double to_insert = a[j][a.columnas()-1];//b_i
      double suma = 0.0;
      for(int i = j+1;i < a.columnas()-1;i++)
      {
          suma += (a[j][i]* incognitas[i][0]);
      }
      to_insert -= suma;
      if (a[j][j] < 0.00001)
      {
        cout << a[j][j] << " " << endl;
      }
      to_insert /= a[j][j];
      incognitas[j][0] = to_insert;
   }   
   cout << endl;
   return incognitas;
}

//Resuelve el sistema, devolviendo un vector que contiene el valor
//de cada incognita.
//Asume que la matriz A ya esta triangulada superiormente
//y que la matriz b es un vector con los resultados
Matriz<double> resolver_sistema(const Matriz<double>& a, const Matriz<double>& b)
{
   Matriz<double> incognitas(a.columnas(),1,0.0);
   for(int j = a.filas()-1;j >= 0;j--)
   {
      double to_insert = b[j][0];//b_i
      double suma = 0.0;
      for(int i = j+1;i < a.columnas();i++)
      {
          suma += (a[j][i]* incognitas[i][0]);
      }
      to_insert -= suma;
      to_insert /= a[j][j];
      incognitas[j][0] = to_insert;
   }   
   return incognitas;
}

//Resuelve el sistema, devolviendo un vector que contiene el valor
//de cada incognita.
//Asume que la matriz A ya esta triangulada inferiormente
//y que la matriz b es un vector con los resultados
Matriz<double> resolver_sistema_inf(const Matriz<double>& a, const Matriz<double>& b)
{
   Matriz<double> incognitas(a.columnas(),1,0.0);
   for(int j = 0;j < a.filas();j++)
   {
      double to_insert = b[j][0];//b_i
      double suma = 0.0;
      for(int i = j-1;i >= 0;i--)
      {
          suma += (a[j][i]* incognitas[i][0]);
      }
      to_insert -= suma;
      to_insert /= a[j][j];
      incognitas[j][0] = to_insert;
   }   
   return incognitas;
}



Matriz<double> resolver_sistema_pivot(const Matriz<double>& ab)
{
   Matriz<double> incognitas(ab.columnas()-1,1,0.0);
   /*Matriz<double> copy = Matriz<double>(ab);
   double mayor; 
   int fil_mayor;
   int filas = copy.filas();
   if(filas > copy.columnas()){
    filas = copy.columnas();
   } 
   for(int i = 0;i < filas;i++)
   {

      mayor = std::abs(copy[i][i]);
      fil_mayor = i;

      for(int j = i+1;j < copy.filas();j++)
      {
          if( std::abs(copy[j][i]) > mayor ){
            mayor = std::abs(copy[j][i]);
            fil_mayor = j;
          }
      }
//      assert(mayor == 0.0); //el sistema no tiene soucion unica
      if(mayor == 0.0){
        continue;
      }
      if(fil_mayor != i)
      {
        double aux;
        for(int j = i; j < copy.columnas(); j++)
        {
          aux = copy[fil_mayor][j];
          copy[fil_mayor][j] = copy[i][j];
          copy[i][j] = aux;
        }
      }
//      cout << "copy :\n" << copy << endl;
      Matriz<double> m = unos(copy.filas(),copy.columnas()); //VER ESTO      
      for(int j = i+1; j < copy.filas();j++) //revisar
      {
        m[j][i] = copy[j][i] / copy[i][i];  
        for (int k = i; k < copy.columnas(); k++)
        {
          copy[j][k] = copy[j][k]-(m[j][i] * copy[i][k]);  
        }
  
      }

  }
  bool ts = es_ts(copy);
  cout << "es ts? :\n " << ts << endl;
  //mostrar_matriz_sin_ceros(copy);
  for (int i = 0; i < copy.filas(); ++i)
  {
    for (int j = 0; j < copy.columnas(); ++j)
    {
      if (copy[i][j] != 0)
      {
        cout << copy[i][j] << " " ;
      }
    }
  }
  cout << endl;
  //cout << "MATRIZ DSP DE PIVOTEAR :\n " << copy << endl;  
  incognitas = resolver_sistema(copy);
  //cout << incognitas << endl;*/
  return incognitas;
}


//Resuelve el sistema Ax=b, usando eliminacion gaussiana.
//No asume nada sobre A
//B es el vector resultados
Matriz<double> resolver_gauss(const Matriz<double>& a, const Matriz<double>& b)
{
    Matriz<double> ab = augmentar(a,b);
    //cout << "ab : "<< ab << endl; 
    return resolver_sistema_pivot(ab);
}

//Resulve el sistema utilizando factorizacion LU
//Asume que sup es triangular superior e inf triangular inferior
//B es el vector de resultados
Matriz<double> resolver_LU(const Matriz<double>& sup, const Matriz<double>& inf,
                   const Matriz<double>& b)
{
  Matriz<double> y = resolver_sistema_inf(inf,b);
  Matriz<double> x = resolver_sistema(sup,y);
  return x;
}



Matriz<double> gen_matriz_random(int filas, int columnas)
{
    Matriz<double> res(filas, columnas, 0);
    for(int i = 0;i < filas;i++)
        for(int j = 0;j < columnas;j++)
            res[i][j] = double(rand() % 100000);
    return std::move(res);
}

Matriz<double> gen_matriz_random(int filas, int columnas, int modulo)
{
    Matriz<double> res(filas, columnas, 0);
    for(int i = 0;i < filas;i++)
        for(int j = 0;j < columnas;j++)
            res[i][j] = double(rand() % modulo);
    return res;
}




#endif
