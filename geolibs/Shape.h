#ifndef __SHAPE_H
#define __SHAPE_H

#include <utility>
#include <string>
#include <fstream>
#include <vector>



class Shape
{


public:
	//Returns the geometric centre of gravity as X,Y pair
	virtual std::pair<double, double> getCOG() = 0;

	//Obtain the svg representation of the shape
	virtual void svgDraw(std::ofstream&, double, double, double, int, int, std::string){};

	int getType(){ return _type; };

protected:
	//types: 0 = Rectangle
	int _type;
};

#endif //__SHAPE_H
