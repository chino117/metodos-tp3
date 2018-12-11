#ifndef RECTA_HPP
#define RECTA_HPP
#include <cassert>
class Recta
{
	public:
		Recta(double x1, double y1, double x2, double y2);
		double evaluar(double x);
	private:
		double coeficiente;
		double independiente;
};

Recta::Recta(double x1, double y1, double x2, double y2)
{

	coeficiente = (y1 - y2)/(x1 - x2);
	independiente = y1 - (coeficiente*x1);
	
}

double Recta::evaluar(double x)
{
	return (coeficiente*x)+independiente;
}

#endif
