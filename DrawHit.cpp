//Pixel geometry
#include "PixelGeoDescr.h"
//Defines
#include "SvgDefines.h"
#include "EventDefines.h"
//Read event
#include "ReadEvent.h"
#include <iostream>

#include <sstream>
#include <list>
#include <cmath>
#include <set>

#include <cstdlib>
//linker
#include <dlfcn.h>




	

int main(int argc, char *argv[])
{

	//in- & out-put files as well as pixelgeolibrary, latter gets read in from file
	std::string lib;
	std::string inp = "SimOutput.txt";
	std::string out = "hitMap.svg";
	
	std::vector<event> eventVec;

	int drawEvent = 1;

	for(int i=1; i<argc; i+=2)
	{
		//std::cout << argv[i] << std::endl;
		std::string s (argv[i]);
		if(s=="-inp")		inp=argv[i+1];
		else if(s=="-o")	out=argv[i+1];
		else if(s=="-n")	drawEvent=atoi(argv[i+1]);

	}
	
	if(!ReadEvent("output/"+inp, eventVec, lib))
	{
		std::cout << "Could not process input file, terminating!" << std::endl;
		return -1;
	}
	
	//Load shared library, be sure to export the path of the lib to LD_LIBRARY_PATH!
	void *hndl = dlopen(lib.c_str(), RTLD_NOW);
	if(hndl == NULL)
	{
		std::cout << "loading failed: " << dlerror() << std::endl;
   		return -1;
	}

	//Use maker() to create the instance of the description
	void *mkr = dlsym(hndl, "maker");
	PixelGeoDescr* my_PixelGeoDescr = reinterpret_cast<PixelGeoDescr*(*)()>(mkr)();

	//The rest is drawing the pixel geometry by obtaining the vector containing all the shapes foe each pixel
	//The scale sets how many microns should be one pixel, if your description uses one place after the comma then set
	//this amount as the scale, e.g. 0.1 and so forth, if you don't this you have to live with (nasty) rounding
	double scale = 1;

	//Set the output file and compute the svg size

	std::ofstream outputFile("output/"+out);
	int svgPixX = (int)ceil(my_PixelGeoDescr->getSizeX()/scale);
	int svgPixY = (int)ceil(my_PixelGeoDescr->getSizeY()/scale);
	outputFile << "<svg width=\""<< svgPixX << "\" height=\"" << svgPixY << "\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink= \"http://www.w3.org/1999/xlink\">\n";

	std::list<std::string> coloursBlue = svg::BLUELIST;
	std::string colour;


	std::set<std::pair<int, int>> hits;
	for (auto it = begin((eventVec.at(drawEvent)).hitVec); it != end((eventVec.at(drawEvent)).hitVec); ++it)
	{
		hits.insert(std::make_pair(it->x, it->y));
	}


	//Iterate over all the pixels
	for(int i = 0; i<my_PixelGeoDescr->getNoPixelsX() ; i++)
	{
		for(int j = 0; j<my_PixelGeoDescr->getNoPixelsY() ; j++)
		{
			//std::pair rep of current pixel
			std::pair<int, int> currentPixel = std::make_pair(i,j);

			//if the pixel is in the hit collection, draw it in corresponding colour
			if(hits.find(currentPixel)!=hits.end())
			{
				double charge;

				//find the hit pixel to determine its charge
				for (auto it = begin((eventVec.at(drawEvent)).hitVec); it != end((eventVec.at(drawEvent)).hitVec); ++it) 
				{
					if((it->x == i) && (it->y == j))
					{
						charge = it->charge;
						break;
					}
				}				
				
				//compute colour from value of charge
				int col = (int)round(60.0-60.0*charge);
				std::stringstream ss;
				ss << col;
				std::string colr = ss.str();
				colour = "hsl("+colr+", 100%, 50%)";

			}
			else
			{
				colour = coloursBlue.front();
				coloursBlue.pop_front();
				coloursBlue.push_back(colour);
			}
			

			//Iterate over all the shapes and draw them
			std::vector<std::shared_ptr<Shape> > obtainedShapes = my_PixelGeoDescr->getShape(i,j);
			for (auto it = begin(obtainedShapes); it != end(obtainedShapes); ++it) 
			{
				(*it)->svgDraw(outputFile, my_PixelGeoDescr->getLowerLeftCorner(i,j).first, my_PixelGeoDescr->getLowerLeftCorner(i,j).second, scale, svgPixX, svgPixY, colour);
			}

		}//y-pixel
	}//x-pixel

	//draw the hit and 3 circles corresponding to 1-3 sigma around the hit
	int svgHitX = (int)ceil((eventVec.at(drawEvent)).realHitX/scale);
	int svgHitY = (int)ceil((eventVec.at(drawEvent)).realHitY/scale);
	int sigma = (int)round((eventVec.at(drawEvent)).sigma);
	outputFile <<  "<circle fill=\"black\" cx=\""<< svgHitX <<"\" cy=\""<< svgPixY-svgHitY <<"\" r=\"2\"/>\n";
	outputFile <<  "<circle fill=\"black\" cx=\""<< svgHitX <<"\" cy=\""<< svgPixY-svgHitY <<"\" r=\""<< sigma/scale <<"\" style=\"stroke: black; fill: none;\"/>\n";
	outputFile <<  "<circle fill=\"black\" cx=\""<< svgHitX <<"\" cy=\""<< svgPixY-svgHitY <<"\" r=\""<< 2*sigma/scale <<"\" style=\"stroke: black; fill: none;\"/>\n";
	outputFile <<  "<circle fill=\"black\" cx=\""<< svgHitX <<"\" cy=\""<< svgPixY-svgHitY <<"\" r=\""<< 3*sigma/scale <<"\" style=\"stroke: black; fill: none;\"/>\n";

	//finalize svg part in xml-style file
	outputFile << "</svg>\n"; 
	
	//clean up and return 1 to OS
	outputFile.close();
	delete my_PixelGeoDescr;
	return 1;
};
