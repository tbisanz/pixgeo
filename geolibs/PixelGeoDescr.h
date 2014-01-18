#ifndef __PIXELGEODESCR_H
#define __PIXELGEODESCR_H

#include<utility>
#include<vector>
#include<set>
#include<memory>
#include "Shape.h"

class PixelGeoDescr
{
	public:

	virtual ~PixelGeoDescr() = default;
	//returns the shapes a pixel is composed of
	virtual std::vector<std::shared_ptr<Shape> > getShape(int , int) = 0;
	std::vector<std::shared_ptr<Shape> > getShape(std::pair<int, int> const& pair)
	{
		return this->getShape(pair.first, pair.second); //this!
	}

	//returns the neighbouring pixels
	virtual std::set<std::pair<int, int> > getNeighbours(int, int) = 0;
	std::set<std::pair<int, int> > getNeighbours(std::pair<int, int> const& pair)
	{
		return this->getNeighbours(pair.first, pair.second);
	}

	//returns true if the pixel is an edge pixel
	virtual bool isEdgePixel(int, int) = 0;
	bool isEdgePixel(std::pair<int, int>const& pair) 
	{
		return this->isEdgePixel(pair.first, pair.second);
	}

	//returns the lower-left-corner of a pixel,
	//this is NOT necessarily a point on the edge of the sensitive area
	//but the origin of the local pixel coordinate sytem for the shape objects!
	virtual std::pair<double, double> getLowerLeftCorner(int, int) = 0;
	std::pair<double, double> getLowerLeftCorner(std::pair<int,int> const& pair)
	{
		return this->getLowerLeftCorner(pair.first, pair.second); 
	}
	
	const double& getSizeX()
	{
		return sizeX;
	}
	
	const double& getSizeY()
	{
		return sizeY;
	}

	const int& getNoPixelsX()
	{
		return noPixelsX;
	}
	
	const int& getNoPixelsY()
	{
		return noPixelsY;
	}


	protected:
	
	int noPixelsX;
	int noPixelsY;
	double sizeX;
	double sizeY;

	bool isValidAdress(int const& X, int const& Y)
	{
		if( X < noPixelsX && Y < noPixelsY && X >=0 && Y>=0)
		{	
			return true;
		}
	return false;
	}
};

#endif //__PIXELGEODESCR_H
