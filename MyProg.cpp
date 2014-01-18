#include "PixelGeoDescr.h"
#include "SvgDefines.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <dlfcn.h>
#include <cmath>

int main(int argc, char *argv[])
{
	std::string xCoord = "0";
	std::string yCoord = "0";
	std::string lib = "MyPixelGeoDescr.so";

	for(int i=1; i<argc; i+=2)
	{
		//std::cout << argv[i] << std::endl;
		std::string s (argv[i]);
		if(s=="-x")	xCoord=argv[i+1];
		else if(s=="-y")	yCoord=argv[i+1];
		else if(s=="-lib")	lib=argv[i+1];
	}

	int xCo;
	std::istringstream( xCoord ) >> xCo;
	int yCo;
	std::istringstream ( yCoord ) >> yCo;
	

	
	//Load shared library, be sure to export the path of the lib to LD_LIBRARY_PATH!
	void *hndl = dlopen(lib.c_str(), RTLD_NOW);
	if(hndl == NULL)
	{
		std::cout << "loading failed: " << dlerror() << std::endl;
   		return -1;
	}

	//Use maker() to create the instance of the description
	void *mkr = dlsym(hndl, "maker");
	//PixelGeoDescr* my_PixelGeoDescr = ((PixelGeoDescr*(*)())(mkr))();
	PixelGeoDescr* my_PixelGeoDescr = reinterpret_cast<PixelGeoDescr*(*)()>(mkr)();

	//std::cout << sizeof(*my_PixelGeoDescr) << std::endl;
	//The rest is drawing the pixel geometry by obtaining the vector containing all the shapes foe each pixel
	//The scale sets how many microns should be one pixel, if your description uses one place after the comma then set
	//this amount as the scale, e.g. 0.1 and so forth, if you don't this you have to live with (nasty) rounding
	double scale = 1;

	//Set the output file and compute the svg size
	std::ofstream outputFile("output/pixelGeo.svg");
	int svgPixX = (int)ceil(my_PixelGeoDescr->getSizeX()/scale);
	int svgPixY = (int)ceil(my_PixelGeoDescr->getSizeY()/scale);
	outputFile << "<svg width=\""<< svgPixX << "\" height=\"" << svgPixY << "\">\n"; 

	std::list<std::string> coloursGreen = svg::GREENLIST;
	std::list<std::string> coloursRed = svg::REDLIST;
	std::list<std::string> coloursBlue = svg::BLUELIST;

	std::pair<int, int> selectedPixel = std::make_pair(xCo, yCo);

	std::set<std::pair<int, int> > neighbours = my_PixelGeoDescr->getNeighbours(selectedPixel);

	//Iterate over all the pixels
	for(int i = 0; i<my_PixelGeoDescr->getNoPixelsX() ; i++)
	{
		for(int j = 0; j<my_PixelGeoDescr->getNoPixelsY() ; j++)
		{
			std::string colour;
			std::pair<int, int> currentPixel = std::make_pair(i, j);
			//Change colours by rearranging the list! Could be done more easily with a vector and % division!
			if((i == selectedPixel.first) & (j == selectedPixel.second))
			{
			colour = "orange";
			}
			else if(neighbours.find(currentPixel)!=neighbours.end())
			{
				colour = coloursBlue.front();
				coloursBlue.pop_front();
				coloursBlue.push_back(colour);
			}
		
			else if(my_PixelGeoDescr->isEdgePixel(i,j))
			{
				colour = coloursRed.front();
				coloursRed.pop_front();
				coloursRed.push_back(colour);
			}
			else
			{
				colour = coloursGreen.front();
				coloursGreen.pop_front();
				coloursGreen.push_back(colour);
			}

			//Iterate over all the shapes
			std::vector<std::shared_ptr<Shape> > obtainedShapes = my_PixelGeoDescr->getShape(i,j);
			for (auto it = begin(obtainedShapes); it != end(obtainedShapes); ++it) 
			{
				(*it)->svgDraw(outputFile, my_PixelGeoDescr->getLowerLeftCorner(i,j).first, my_PixelGeoDescr->getLowerLeftCorner(i,j).second, scale, svgPixX, svgPixY, colour);
			}
		}
	}
	outputFile << "</svg>\n"; 
	outputFile.close();

	delete my_PixelGeoDescr;
	return 1;
};
