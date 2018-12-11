
Matriz<double> generarRuido(Matriz<double>& limpio)
{
	Matriz<double> conRuido(limpio);
	for(int i = 0; i < conRuido.filas(); i++)
	{
		int ruido = conRuido[i][0] + (std::rand() % 256);
/*
		if(ruido > 255)
		{
			ruido = 255;
		}
*/
		conRuido[i][0] = ruido;

	}
	return conRuido;
}
