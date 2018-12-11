#ifndef IOUTILS_H_
#define IOUTILS_H_

#include "ppmloader/ppmloader.h"
#include "matriz.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <limits>
#include <algorithm>
#include <getopt.h>

using namespace std;
/*
struct info_archivo
{
	std::string inPath;
	std::string outPath;
	int cantidadRayos;
};
*/

class IOUtils {
public:
	IOUtils(int argc, char** argv);
	~IOUtils();
	Matriz<double> leer_imagen();
	void escribir_imagen(Matriz<double>& imagen);
	
	//info_archivo info;
	std::string inPath;
	std::string outPath;
	int cantidadRayos;
	int discretizadoFilas = 10;
	int discretizadoColumnas = 10;
};

//IOUtils::IOUtils(){
//}

IOUtils::~IOUtils(){
}


IOUtils::IOUtils(int argc, char** argv){

    if (argc < 4)
    {
        std::cout<<"Parametros de entrada insuficientes"<<'\n';
    }
	cantidadRayos = std::atoi(argv[1]);
	int discretizado = std::atoi(argv[2]);
	discretizadoFilas = discretizado;
	discretizadoColumnas = discretizado;
	inPath = argv[3];
	outPath = argv[4];
}   

Matriz<double> IOUtils::leer_imagen()
{
	//Por ahora asumo imagen ppm en formato raw con profundidad
	//de color de 8 bits.
    uchar* imagen;
	int alto; int ancho;
    PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;
    bool ret = LoadPPMFile(&imagen, &ancho, &alto, &pt, inPath.c_str());


  if (!ret || ancho == 0|| alto == 0|| pt!=PPM_LOADER_PIXEL_TYPE_GRAY_8B)
  {
    throw std::runtime_error("test_load failed");
  }
    //Pixeles en RGB de una imagen en escala de grises, 
	// R = G = B = intensidad de gris
	
    Matriz<double> temp(alto, ancho);

    for(int i = 0;i < alto; i++)
    {
		for(int j = 0; j < ancho; j++)
		{
        	temp[i][j] = double(imagen[(i*ancho)+j]);
		}
    }

    delete [] imagen;    
    return temp;
}


void IOUtils::escribir_imagen(Matriz<double>& imagen)
{
	char comments[10];
	sprintf(comments, "%s", "TP3");
	int col = discretizadoColumnas;
	int fil = discretizadoFilas;
	uchar* data = new uchar[fil*col];
	for(int i = 0; i < fil; i++)
	{
		for(int j = 0; j < col; j++)
		{	double xij = imagen[(i*col)+j][0];
			xij = std::max(0,std::min(255,(int)xij));
			//uchar lol = uchar((int)imagen[(i*col)+j][0]);

			cout << "|" << xij << "|" << endl;
			data[(i*col)+j] = (uchar)xij;
		}
	}
	bool ret = SavePPMFile(outPath.c_str(),data,col,fil,PPM_LOADER_PIXEL_TYPE_GRAY_16B, comments);
	if (!ret)
	{
		std::cout << "ERROR: couldn't save Image to ppm file" << std::endl;
	}
}
#endif
