#ifndef __RECTANGLE_H
#define __RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape
{
public:

	Rectangle(double, double, double, double);

	//Returns the geometric centre of gravity as X,Y pair
	std::pair<double, double> getCOG();

	//Obtain the svg representation of the shape
	void svgDraw(std::ofstream&, double, double, double, int, int, std::string);

	//Returns the X,Y pos. in local frame
	std::pair<double, double> getPos();

	//Returns height & width
	std::pair<double, double> getDim();

protected:

	//The position of the lower left corner in the local coordinate sytem
	double _X, _Y;
	//height and width of the rectangle
	double _width, _height;
};

#endif //__RECTANGLE_H
