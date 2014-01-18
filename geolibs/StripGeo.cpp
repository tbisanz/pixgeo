#include "StripGeo.h"
#include <iostream>
#include <stdlib.h>
#include "Rectangle.h"

StripGeo::StripGeo()
{	
	//Setting the pixel count, inherited from PixelGeoDescr
	noPixelsX = 40;
	noPixelsY = 1;
	//And the size in microns
	sizeX = 40*50;
	sizeY = 1000;
}

std::vector<std::shared_ptr<Shape> > StripGeo::getShape(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::vector<std::shared_ptr<Shape> > result;
	std::shared_ptr<Rectangle> Shape1(new Rectangle(0, 0, 50, 1000));
	result.push_back(Shape1);
	return result;

}


std::set<std::pair<int, int> > StripGeo::getNeighbours(int XCo, int YCo)
{
	std::set<std::pair<int, int>> neighbours;

	if(XCo == 0)
	{

		neighbours.insert(std::make_pair(1,0));
	}
	
	else if(XCo == 39)
	{
		neighbours.insert(std::make_pair(38,0));
	}
	else
	{
			neighbours.insert(std::make_pair(XCo-1,0));
			neighbours.insert(std::make_pair(XCo+1,0));
	}

	return neighbours;
}


bool StripGeo::isEdgePixel(int XCo, int YCo)
{
	return true;
}

std::pair<double, double> StripGeo::getLowerLeftCorner(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}

	double YPos = 0;
	double XPos = XCo*50;
	std::pair<double, double> result;
	result.first = XPos;
	result.second = YPos;
	return result;
}


PixelGeoDescr* maker()
{
	StripGeo* PixGeoDescr = new StripGeo();
	return dynamic_cast<PixelGeoDescr*>(PixGeoDescr);
}

