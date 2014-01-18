#include "Rectangle.h"
#include<cmath>
//#include<sstream>


Rectangle::Rectangle(double X, double Y, double width, double height): _X(X), _Y(Y), _width(width), _height(height)
{
	_type = 0;
}

std::pair<double, double> Rectangle::getCOG()
{
	std::pair<double, double> cog;
	cog.first = _X+(_width)/2.0;
	cog.second = _Y+(_height)/2.0;
	return cog;
}

void Rectangle::svgDraw(std::ofstream& stream, double X, double Y, double scale, int svgPixX, int svgPixY, std::string colour)
{
	int svgIntX = (int)round((X+_X)/scale);
	int svgIntY = svgPixY-(int)round((Y+_Y)/scale)-(int)round(_height/scale);	
	int svgIntWidth = (int)round(_width/scale);
	int svgIntHeight = (int)round(_height/scale);

	stream << "<rect x=\"" << svgIntX << "\" y=\"" << svgIntY << "\" width=\"" << svgIntWidth << "\" height=\"" << svgIntHeight << "\" style=\"fill:" << colour << ";opacity:1\"/>\n";
}

std::pair<double, double> Rectangle::getPos()
{
	std::pair<double, double> result;
	result.first = _X;
	result.second = _Y; 
	return result;
}

std::pair<double, double> Rectangle::getDim()
{
	std::pair<double, double> result;
	result.first = _width;
	result.second = _height; 
	return result;
}
