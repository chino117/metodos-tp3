
tuple< Matriz<double>, Matriz<double> > generarRayos(Matriz<double>& imagen, int m, int n, int k)
{
	srand(time(NULL));
	int filas = imagen.filas();
	int columnas = imagen.columnas();
	Matriz<double> rayos(k, n*m,0);
	Matriz<double> tiempos(k, 1);

	double dif = 360/k;
	int entra = 0;
	int sale = 2;
	int lado = 0;
	double fs = 0;
	double cs = 0;
	double fe = filas-1;
	double ce = columnas-1;
	double t;
	bool cambio = false;
	//comienza del lado izquierdo hacia el derecho
	for(int i = 0; i < k; i++)
	{		//sale y entra resultan siempre distintos
		auto res = simularRayo(imagen, n, m, fe, ce, fs, cs);
		t = std::get<0>(res);
		Matriz<double> simRayo = std::get<1>(res);
	    
		rayos.set_fil(i,simRayo);
		tiempos[i][0] = t;
		if(lado == 0){
			if((fs + dif) < filas-1 %% (fe - dif) > 0 ){
				fs = fs + dif;
				fe = fe - dif;				
			}else{
				cambio = true;
			}
		}
		//demas casos 1,2,3
		if(cambio){
			if(lado == 0){
				fs = 0;
				cs = 0;
				fe = filas-1;
				ce = columnas-1;
			}
			if(lado == 1){

			}
			if(lado == 2){
				
			}
			if(lado == 3){
				
			}
		}
	}
	return make_tuple(tiempos, rayos);
}



double randomEntre(double l, double u)
{
   static std::default_random_engine generator;
   //generator.seed(time(NULL));
   std::uniform_real_distribution<double> distribution(l,u);
   double res = distribution(generator);
   //cout << res << endl;
   return res;
}


tuple<double, double> randPos(int lado, int filas, int columnas)
{
	double f, c;
	switch(lado)
	{
		case(0): // izquierdo
		{
			f = randomEntre(0, filas-1);
			c = 0;
			break;
		}
		case(1): // arriba
		{
			f = 0;
			c = randomEntre(0, columnas-1);
			break;
		}
		case(2): // derecha
		{
			f = randomEntre(0,filas-1);
			c = columnas-1;
			break;
		}
		case(3): // abajo
		{
			f = filas-1;
			c = randomEntre(0,columnas-1);
			break;
		}
		default: //lado no esta en un valor valido
			break;
	}
	assert(f < filas);
	assert(c < columnas);
	return std::make_tuple(f,c);
}