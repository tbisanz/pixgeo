#ifndef __MYOTHERPIXELGEODESCR_H
#define __MYOTHERPIXELGEODESCR_H

#include "PixelGeoDescr.h"

class MyOtherPixelGeoDescr : public PixelGeoDescr
{
	public:

	MyOtherPixelGeoDescr();

	//returns the shapes a pixel is composed of
	std::vector<std::shared_ptr<Shape> > getShape(int, int);

	//returns the neighbouring pixels
	std::set<std::pair<int, int> > getNeighbours(int, int);

	//returns true if the pixel is an edge pixel
	bool isEdgePixel(int, int);

	//returns the lower-left-corner of a pixel,
	//this is NOT necessarily a point on the edge of the sensitive area
	//but the origin of the local pixel coordinate sytem for the shape objects!
	std::pair<double, double> getLowerLeftCorner(int, int);
};

extern "C"
{
        PixelGeoDescr* maker();
};

#endif //__MYOTHERPIXELGEODESCR_H
