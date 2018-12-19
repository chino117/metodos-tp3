
tuple< Matriz<double>, Matriz<double> > generarRayos(Matriz<double>& imagen, int m, int n, int k)
{
	srand(time(NULL));
	int filas = imagen.filas();
	int columnas = imagen.columnas();

	int cantXlados = k\4;
	int sobran = k % 4;

	Matriz<double> rayos(cantXlados*4, n*m,0);
	Matriz<double> tiempos(k, 1);

	double difCols = columnas/cantXlados;
	double difFils = filas/cantXlados;

	int entra = 0;
	int sale = 2;
	int rayo;

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
				fs = fs - dif;
				fe = fe + dif;
			}
			if(lado == 1){
				cs = cs + dif;
				ce = ce - dif;
			}
			if(lado == 2){
				fe = fe - dif;
				fs = fs + dif;
			}
			if(lado == 3){
				ce = ce + dif;
				cs = cs - dif;
			}
		}
	}
	return make_tuple(tiempos, rayos);
}
