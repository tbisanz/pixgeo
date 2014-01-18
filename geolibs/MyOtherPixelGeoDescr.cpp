#include "MyOtherPixelGeoDescr.h"
#include <iostream>
#include <stdlib.h>
#include "Rectangle.h"

MyOtherPixelGeoDescr::MyOtherPixelGeoDescr()
{	
	//Setting the pixel count, inherited from PixelGeoDescr
	noPixelsX = 20;
	noPixelsY = 15;
	//And the size in microns
	sizeX = 10*200;
	sizeY = 15*50;
}

std::vector<std::shared_ptr<Shape> > MyOtherPixelGeoDescr::getShape(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (XCo%2 == 0)
	{
		std::vector<std::shared_ptr<Shape> > result;
		std::shared_ptr<Rectangle> Shape1(new Rectangle(0, 0, 30, 50));
		std::shared_ptr<Rectangle> Shape2(new Rectangle(30, 0, 140, 25));
		result.push_back(Shape1);
		result.push_back(Shape2);
		return result;
	}
	else
	{
		std::vector<std::shared_ptr<Shape> > result;
		std::shared_ptr<Rectangle> Shape1(new Rectangle(170, 0, 30, 50));
		std::shared_ptr<Rectangle> Shape2(new Rectangle(30, 25, 140, 25));
		result.push_back(Shape1);
		result.push_back(Shape2);
		return result;
	}
}


std::set<std::pair<int, int> > MyOtherPixelGeoDescr::getNeighbours(int XCo, int YCo)
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
		else if(YCo == 14)
		{
			neighbours.insert(std::make_pair(1,14));
			neighbours.insert(std::make_pair(0,13));
			neighbours.insert(std::make_pair(1,13));
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
		else if(YCo == 14)
		{
			neighbours.insert(std::make_pair(19,13));
			neighbours.insert(std::make_pair(18,14));
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
	else if(YCo == 14)
	{
		if(XCo%2==1)
		{
			neighbours.insert(std::make_pair(XCo-1,14));
			neighbours.insert(std::make_pair(XCo+1,14));
			//neighbours.insert(std::make_pair(XCo-1,13));
			neighbours.insert(std::make_pair(XCo+1,13));
			neighbours.insert(std::make_pair(XCo,13));
		}
		else
		{
			neighbours.insert(std::make_pair(XCo-1,14));
			neighbours.insert(std::make_pair(XCo+1,14));
			neighbours.insert(std::make_pair(XCo-1,13));
			neighbours.insert(std::make_pair(XCo+1,13));
			neighbours.insert(std::make_pair(XCo,13));
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


bool MyOtherPixelGeoDescr::isEdgePixel(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}

	if(XCo == 0 || XCo == 19 || YCo == 0 || YCo == 14)
	{
		return true;
	}
	
	return false;
}

std::pair<double, double> MyOtherPixelGeoDescr::getLowerLeftCorner(int XCo, int YCo)
{
	if(!isValidAdress(XCo, YCo))
	{
		std::cout << "(" << XCo << "," << YCo << ") is not a valid pixel adress, terminating!" << std::endl;
		exit(EXIT_FAILURE);
	}

	double YPos = YCo*50.0;
	int temp = XCo/2;
	double XPos = temp*200.0;

	std::pair<double, double> result;
	result.first = XPos;
	result.second = YPos;
	return result;
}

PixelGeoDescr* maker()
{
	MyOtherPixelGeoDescr* PixGeoDescr = new MyOtherPixelGeoDescr();
	return dynamic_cast<PixelGeoDescr*>(PixGeoDescr);
}

