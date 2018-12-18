
tuple< Matriz<double>, Matriz<double> > generarRayos(Matriz<double>& imagen, int m, int n, int k)
{
	srand(time(NULL));
	int filas = imagen.filas();
	int columnas = imagen.columnas();
	Matriz<double> rayos(k, n*m,0);
	Matriz<double> tiempos(k, 1);

	double dif = 360/k;
	int sale = 0;
	int entra = 2;
	int lado = 0;
	double cs = 0;
	double fs = filas-1;
	double ce = columnas-1;
	double fe = 0;
	double t;
	bool cambio = false;
	//comienza del lado izquierdo hacia el derecho
	for(int i = 0; i < k; i++) //faltaria alguna variable que me cuente la cantidad de rayos por lado que voy tirando!!
	{		//sale y entra resultan siempre distintos
		auto res = simularRayo(imagen, n, m, fe, ce, fs, cs);
		t = std::get<0>(res);
		Matriz<double> simRayo = std::get<1>(res);
	    
		rayos.set_fil(i,simRayo);
		tiempos[i][0] = t;
		if(lado == 0){
			if((fe + dif) < filas-1 && (fs - dif) > -1 ){
				fs = fs - dif;
				fe = fe + dif;
			}else{
				cambio = true;
			}
		}
		if(lado == 1){
			if((cs + dif) < columnas-1 && (ce - dif) > -1 ){
				cs = cs + dif;
				ce = ce - dif;
			}else{
				cambio = true;
			}
		}
		if(lado == 2){
			if((fs + dif) < filas-1 && (fe - dif) > -1 ){
				fe = fe - dif;
				fs = fs + dif;
			}else{
				cambio = true;
			}
		}
		if(lado == 3){
			if((ce + dif) < columnas-1 && (cs - dif) > -1 ){
				ce = ce + dif;
				cs = cs - dif;
			}else{
				cambio = true;
			}
		}
		//demas casos 1,2,3
		if(cambio){
			if(lado == 0){ //seria el mismo caso para el ultimo del lado 0, es omite?
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
			lado = sale;
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
