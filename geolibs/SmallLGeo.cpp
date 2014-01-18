#include "SmallLGeo.h"
#include <iostream>
#include <stdlib.h>
#include "Rectangle.h"

SmallLGeo::SmallLGeo()
{	
	//Setting the pixel count, inherited from PixelGeoDescr
	noPixelsX = 20;
	noPixelsY = 80;
	//And the size in microns
	sizeX = 10*500;
	sizeY = 80*25;
}

std::vector<std::shared_ptr<Shape> > SmallLGeo::getShape(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (XCo%2 == 0)
	{
		std::vector<std::shared_ptr<Shape> > result;
		std::shared_ptr<Rectangle> Shape1(new Rectangle(0, 0, 50, 50));
		std::shared_ptr<Rectangle> Shape2(new Rectangle(50, 0, 400, 25));
		result.push_back(Shape1);
		result.push_back(Shape2);
		return result;
	}
	else
	{
		std::vector<std::shared_ptr<Shape> > result;
		std::shared_ptr<Rectangle> Shape1(new Rectangle(450, 0, 50, 50));
		std::shared_ptr<Rectangle> Shape2(new Rectangle(50, 25, 400, 25));
		result.push_back(Shape1);
		result.push_back(Shape2);
		return result;
	}
}


std::set<std::pair<int, int> > SmallLGeo::getNeighbours(int XCo, int YCo)
{
	std::set<std::pair<int, int> > neighbours;

	if(XCo == 0)
	{
		if(YCo == 0)
		{
			neighbours.insert(std::make_pair(0,1));
			neighbours.insert(std::make_pair(1,0));
			//neighbours.insert(std::make_pair(1,1));
		}
		else if(YCo == 79)
		{
			neighbours.insert(std::make_pair(1,79));
			neighbours.insert(std::make_pair(0,78));
			neighbours.insert(std::make_pair(1,78));
		}
		else
		{
			neighbours.insert(std::make_pair(0,YCo+1));
			neighbours.insert(std::make_pair(0,YCo-1));
			//neighbours.insert(std::make_pair(1,YCo+1));
			neighbours.insert(std::make_pair(1,YCo));
			neighbours.insert(std::make_pair(1,YCo-1));
		}
	}
	else if(XCo == 19)
	{
		if(YCo == 0)
		{
			neighbours.insert(std::make_pair(19,1));
			neighbours.insert(std::make_pair(18,0));
			neighbours.insert(std::make_pair(18,1));
		}
		else if(YCo == 79)
		{
			neighbours.insert(std::make_pair(19,78));
			neighbours.insert(std::make_pair(18,79));
			//neighbours.insert(std::make_pair(18,13));
		}
		else
		{
			neighbours.insert(std::make_pair(19,YCo+1));
			neighbours.insert(std::make_pair(19,YCo-1));
			neighbours.insert(std::make_pair(18,YCo+1));
			neighbours.insert(std::make_pair(18,YCo));
			neighbours.insert(std::make_pair(18,YCo-1));
		}
	}
	//The edge pixels have already been treated, thus this is rather straightforward :-)
	else if(YCo == 0)
	{
		if(XCo%2 == 0)
		{
			neighbours.insert(std::make_pair(XCo-1,0));
			neighbours.insert(std::make_pair(XCo+1,0));
			neighbours.insert(std::make_pair(XCo-1,1));
			//neighbours.insert(std::make_pair(XCo+1,1));
			neighbours.insert(std::make_pair(XCo,1));
		}
		else
		{
			neighbours.insert(std::make_pair(XCo-1,0));
			neighbours.insert(std::make_pair(XCo+1,0));
			neighbours.insert(std::make_pair(XCo-1,1));
			neighbours.insert(std::make_pair(XCo+1,1));
			neighbours.insert(std::make_pair(XCo,1));
		}
	}
	else if(YCo == 79)
	{
		if(XCo%2==1)
		{
			neighbours.insert(std::make_pair(XCo-1,79));
			neighbours.insert(std::make_pair(XCo+1,79));
			//neighbours.insert(std::make_pair(XCo-1,13));
			neighbours.insert(std::make_pair(XCo+1,78));
			neighbours.insert(std::make_pair(XCo,78));
		}
		else
		{
			neighbours.insert(std::make_pair(XCo-1,79));
			neighbours.insert(std::make_pair(XCo+1,79));
			neighbours.insert(std::make_pair(XCo-1,78));
			neighbours.insert(std::make_pair(XCo+1,78));
			neighbours.insert(std::make_pair(XCo,78));
		}
	}
	//The case that the pixel has all 7 neighbours:
	else if(XCo%2 == 0)
	{
			neighbours.insert(std::make_pair(XCo-1,YCo-1));
			neighbours.insert(std::make_pair(XCo+1,YCo-1));
			neighbours.insert(std::make_pair(XCo,YCo-1));

			neighbours.insert(std::make_pair(XCo-1,YCo+1));
			//neighbours.insert(std::make_pair(XCo+1,YCo+1));
			neighbours.insert(std::make_pair(XCo,YCo+1));

			neighbours.insert(std::make_pair(XCo+1,YCo));
			neighbours.insert(std::make_pair(XCo-1,YCo));
	}
	else
	{
			//neighbours.insert(std::make_pair(XCo-1,YCo-1));
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


bool SmallLGeo::isEdgePixel(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}

	if(XCo == 0 || XCo == 19 || YCo == 0 || YCo == 79)
	{
		return true;
	}
	
	return false;
}

std::pair<double, double> SmallLGeo::getLowerLeftCorner(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}


	double YPos = 80.0*50.0-YCo*50.0;
	int temp = XCo/2;
	double XPos = temp*500.0;

	std::pair<double, double> result;
	result.first = XPos;
	result.second = YPos;
	return result;
}

PixelGeoDescr* maker()
{
	SmallLGeo* PixGeoDescr = new SmallLGeo();
	return dynamic_cast<PixelGeoDescr*>(PixGeoDescr);
}

