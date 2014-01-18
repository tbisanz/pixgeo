#include "PixelGeoDescr.h"
#include "SvgDefines.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <dlfcn.h>
#include "stdlib.h"
#include <cmath>
#include <random>
#include "Rectangle.h"
#include "EventDefines.h"
#include <iomanip>

//Root includes

#include "TFile.h"
#include "TTree.h"

typedef std::pair<double, double> coord;


int main(int argc, char *argv[])
{
	//prepare rngen
	std::random_device rd;
	std::mt19937 gen(rd());

	//process cli arguments
	std::string lib = "MyPixelGeoDescr.so";
	std::string out = "SimOutput.txt";
	std::string outRoot = "SimOutput.root";

	int events = 1000;
	double sigma = 9;

	for(int i=1; i<argc; i+=2)
	{
		std::string s (argv[i]);
		if(s=="-lib")		lib=argv[i+1];
		else if(s=="-o")	out=argv[i+1];
		else if(s=="-s")	sigma=atof(argv[i+1]);
		else if(s=="-n")	events=atoi(argv[i+1]);
	}

	TFile* myTFile = new TFile(("output/"+outRoot).c_str(), "RECREATE");
	myTFile->cd();
	TTree* tree = new TTree("tree", "my tree");

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

	double offset = 0;

	//Get the size of the pixels and prepare the distributions!
	std::uniform_real_distribution<> disX(offset, my_PixelGeoDescr->getSizeX()-offset);
	std::uniform_real_distribution<> disY(offset, my_PixelGeoDescr->getSizeY()-offset);
	
	//Set the output file
	std::ofstream outputFile("output/"+out);
	outputFile << lib << "\n";
	
	double hitX;
	double hitY;
	std::vector<hit> hitVec;
	double totalCharge;

	tree->Branch("HitXPos",&hitX);
	tree->Branch("HitYPos",&hitY);
	tree->Branch("Sigma",&sigma);
	tree->Branch("TotalCharge", &totalCharge); 
	tree->Branch("Hits", &hitVec);

	for(int n = 0; n < events; n++)
	{
		hitVec.clear();
		hitX = disX(gen);
		hitY = disY(gen);

		double sigmasqrd = sigma*sigma;
		totalCharge = 0.;

		//Iterate over all the pixels
		for(int i = 0; i<my_PixelGeoDescr->getNoPixelsX() ; i++)
		{
			for(int j = 0; j<my_PixelGeoDescr->getNoPixelsY() ; j++)
			{
				hit myHit;
				myHit.x = i;
				myHit.y = j;
				myHit.charge=0;
				
				//Iterate over all the shapes
				std::vector<std::shared_ptr<Shape> > obtainedShapes = my_PixelGeoDescr->getShape(i,j);
				for (auto it = begin(obtainedShapes); it != end(obtainedShapes); ++it) 
				{

					//Get the COG of the obatained shape
					coord COG_local = (*it)->getCOG();
					//And the local pos of the pixel
					coord locPos = my_PixelGeoDescr->getLowerLeftCorner(i, j);

					//Compute the distance of each COG and the hit-position
					double xCOG_global = COG_local.first + locPos.first;
					double yCOG_global = COG_local.second + locPos.second;
					double xDiff = xCOG_global - hitX;
					double yDiff = yCOG_global - hitY;

					double distsqrd = xDiff*xDiff+yDiff*yDiff;

					//If the hit is within x*sigma, we process it
					if( sqrt(distsqrd) < 500+12.5+4*sigma )
					{
						//case that the current shape is a rectangle
						if((*it)->getType()==0)
						{
							//cast shape into rectangle
							Rectangle* rec = static_cast<Rectangle*>((*it).get());

							//and get geometry
							coord recPos = rec->getPos();
							std::pair<double, double> recDim = rec->getDim();

							//the charge is computed via the 2D integral, which is 1/4(ERFX)(ERFY)
							//where ERF are the erf() evaluated at the bondaries in X and Y direction
							double charge = 1.0/(4.0);

							//get the starting position
							double startDistX = recPos.first+locPos.first-hitX;
							double startDistY = recPos.second+locPos.second-hitY;

							//compute erf((x-mu)/sqrt(2*sigma^2))
							charge *= erf((startDistX+recDim.first)/sqrt(2*sigmasqrd))-erf((startDistX)/sqrt(2*sigmasqrd));
							charge *= erf((startDistY+recDim.second)/sqrt(2*sigmasqrd))-erf((startDistY)/sqrt(2*sigmasqrd));
						
							//sum up charge for pixel and total
							myHit.charge += charge;
							totalCharge += charge;
						}

						//other shapes are not supported and should cause the program to terminate!
						else
						{
							std::cout << "The geometry consists of other pixels than only rectengular ones, this is not supported!" << std::endl;
							return -1;
						}
					}//If distance is small enough
				}//Iterator over Shapes

			//Only write hits with at least some reasonable charge (1%) into file
			if(myHit.charge > 0.001)
			{
				//std::cout <<  std::setprecision(100) << std::fixed << myHit.charge << std::endl;
				hitVec.push_back(myHit);
			}
			}//Pixel y loop
		}//Pixel x loop

		//Write event information
		outputFile << "***\n";
		outputFile << "Event: " << n << " charge: " << totalCharge << "\n";
		outputFile << "HitInfo: " << hitX << " " << hitY << " " << sigma <<"\n";

		for (auto it = begin(hitVec); it != end(hitVec); ++it)
		{
			outputFile << it->x << " " << it->y << " " << it->charge << "\n";
		} 

	tree->Fill();

	} //for loop

	tree->Write();
	myTFile->Close();

	//cleanup and return 1 to OS
	outputFile.close();
	//delete tree;
	delete myTFile;
	delete my_PixelGeoDescr;
	return 1;
};
