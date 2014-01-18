#include "LongEdgePixelGeo.h"
#include <iostream>
#include <stdlib.h>
#include "Rectangle.h"

LongEdgePixelGeo::LongEdgePixelGeo()
{	
	//Setting the pixel count, inherited from PixelGeoDescr
	noPixelsX = 80;
	noPixelsY = 336;
	//And the size in microns
	sizeX = 2*400+78*250;
	sizeY = 336*50;
}

std::vector<std::shared_ptr<Shape> > LongEdgePixelGeo::getShape(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (XCo == 0 || XCo == 79)
	{
		//std::cout << "getShape() called" << std::endl;
		std::vector<std::shared_ptr<Shape> > result;
		std::shared_ptr<Rectangle> Shape1(new Rectangle(0, 0, 400, 50));
		result.push_back(Shape1);
		return result;
	}
	else
	{
		std::vector<std::shared_ptr<Shape> > result;
		std::shared_ptr<Rectangle> Shape1(new Rectangle(0, 0, 250, 50));
		result.push_back(Shape1);
		return result;
	}
}


std::set<std::pair<int, int> > LongEdgePixelGeo::getNeighbours(int XCo, int YCo)
{
	std::set<std::pair<int, int>> neighbours;

	if(XCo == 0)
	{
		if(YCo == 0)
		{
			neighbours.insert(std::make_pair(0,1));
			neighbours.insert(std::make_pair(1,0));
			neighbours.insert(std::make_pair(1,1));
		}
		else if(YCo == 335)
		{
			neighbours.insert(std::make_pair(1,335));
			neighbours.insert(std::make_pair(0,334));
			neighbours.insert(std::make_pair(1,334));
		}
		else
		{
			neighbours.insert(std::make_pair(0,YCo+1));
			neighbours.insert(std::make_pair(0,YCo-1));
			neighbours.insert(std::make_pair(1,YCo+1));
			neighbours.insert(std::make_pair(1,YCo));
			neighbours.insert(std::make_pair(1,YCo-1));
		}
	}
	else if(XCo == 79)
	{
		if(YCo == 0)
		{
			neighbours.insert(std::make_pair(79,1));
			neighbours.insert(std::make_pair(78,0));
			neighbours.insert(std::make_pair(78,1));
		}
		else if(YCo == 335)
		{
			neighbours.insert(std::make_pair(79,334));
			neighbours.insert(std::make_pair(78,335));
			neighbours.insert(std::make_pair(78,334));
		}
		else
		{
			neighbours.insert(std::make_pair(79,YCo+1));
			neighbours.insert(std::make_pair(79,YCo-1));
			neighbours.insert(std::make_pair(78,YCo+1));
			neighbours.insert(std::make_pair(78,YCo));
			neighbours.insert(std::make_pair(78,YCo-1));
		}
	}
	//The edge pixels have already been treated, thus this is rather straightforward :-)
	else if(YCo == 0)
	{
			neighbours.insert(std::make_pair(XCo-1,0));
			neighbours.insert(std::make_pair(XCo+1,0));
			neighbours.insert(std::make_pair(XCo-1,1));
			neighbours.insert(std::make_pair(XCo+1,1));
			neighbours.insert(std::make_pair(XCo,1));
	}
	else if(YCo == 335)
	{
			neighbours.insert(std::make_pair(XCo-1,335));
			neighbours.insert(std::make_pair(XCo+1,335));
			neighbours.insert(std::make_pair(XCo-1,334));
			neighbours.insert(std::make_pair(XCo+1,334));
			neighbours.insert(std::make_pair(XCo,334));
	}
	//The case that the pixel has all 8 neighbours:
	else
	{
			neighbours.insert(std::make_pair(XCo-1,YCo-1));
			neighbours.insert(std::make_pair(XCo+1,YCo-1));
			neighbours.insert(std::make_pair(XCo,YCo-1));

			neighbours.insert(std::make_pair(XCo-1,YCo+1));
			neighbours.insert(std::make_pair(XCo+1,YCo+1));
			neighbours.insert(std::make_pair(XCo,YCo+1));

			neighbours.insert(std::make_pair(XCo+1,YCo));
			neighbours.insert(std::make_pair(XCo-1,YCo));
	}

	return neighbours;
}


bool LongEdgePixelGeo::isEdgePixel(int XCo, int YCo)
{
	if(XCo == 0 || XCo == 79 || YCo == 0 || YCo == 335)
	{
		return true;
	}
	
	return false;
}

std::pair<double, double> LongEdgePixelGeo::getLowerLeftCorner(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}

	double YPos = 335*50-YCo*50.0;
	double XPos;

	if(XCo == 0)
	{			
		XPos = 0;
	}
	else
	{
		XPos = 400+(XCo-1)*250;
	}

	std::pair<double, double> result;
	result.first = XPos;
	result.second = YPos;
	return result;
}


PixelGeoDescr* maker()
{
	LongEdgePixelGeo* PixGeoDescr = new LongEdgePixelGeo();
	return dynamic_cast<PixelGeoDescr*>(PixGeoDescr);
}

