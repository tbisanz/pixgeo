//Pixel geometry
#include "PixelGeoDescr.h"
//Defines
#include "EventDefines.h"
//Read event
#include "ReadEvent.h"

#include <iostream>
#include <list>
#include <cmath>
#include <set>
#include <cstdlib>
//dynamic linker
#include <dlfcn.h>

//ROOT includes
#include "TH1F.h"
#include "TFile.h"



int main(int argc, char *argv[])
{

	//in- & out-put files as well as pixelgeolibrary, latter gets read in from file
	std::string lib;
	std::string inp = "SimOutput.txt";

	
	std::vector<event> eventVec;

	for(int i=1; i<argc; i+=2)
	{
		//std::cout << argv[i] << std::endl;
		std::string s (argv[i]);
		if(s=="-inp")		inp=argv[i+1];
	}
	
	if(!ReadEvent(inp, eventVec, lib))
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

	TFile* myTFile = new TFile("MyTFile.root", "RECREATE");
	myTFile->cd();
	TH1F* hitX = new TH1F("hitX", "real x hit distribution", 1000, 0, (int)ceil(my_PixelGeoDescr->getSizeX()));
	TH1F* hitY = new TH1F("hitY", "real y hit distribution", 1000, 0, (int)ceil(my_PixelGeoDescr->getSizeY()));
	TH1F* chargeMost = new TH1F("chargeMost", "charge in most charge pixel", 100, 0, 1);
	TH1F* chargeSecond = new TH1F("chargeSecond", "charge in second most charge pixel", 100, 0, 1);
	TH1F* resX = new TH1F("resX", "residual x  distribution", 1000, -50, 50);
	TH1F* resXnaive = new TH1F("resXnaive", "naive residual x  distribution", 1000, -50, 50);
	TH1F* eta = new TH1F("eta", "eta distribution", 200, 0, 1);
	TH1F* etacorr = new TH1F("etacorr", "eta correction", 200, 0, 1);
	int singleHits = 0;

	for (auto it = begin(eventVec); it != end(eventVec); ++it)
	{
		hitX->Fill(it->realHitX);
		hitY->Fill(it->realHitY);
		std::vector<hit> eventHits = it->hitVec;
		hit mostChargeHit, secondMostChargeHit;
		mostChargeHit.charge=0;
		secondMostChargeHit.charge=0;

		for (auto itHit = begin(eventHits); itHit != end(eventHits); ++itHit)
		{
			if( (itHit->charge) > mostChargeHit.charge )
			{
				secondMostChargeHit = mostChargeHit;
				mostChargeHit = *itHit;
			}
			else if( (itHit->charge) > secondMostChargeHit.charge )
			{
				secondMostChargeHit = *itHit;
			}
		}

		
		double posMostChargeHit = mostChargeHit.x*50.0 + 25;
		double posSecondMostChargeHit = secondMostChargeHit.x*50.0 + 25; 
		double difHit = posSecondMostChargeHit - posMostChargeHit;
		
		double mostCharge = mostChargeHit.charge;
		double secondMostCharge = secondMostChargeHit.charge;

		double recoPos = posMostChargeHit + secondMostCharge/(mostCharge+secondMostCharge)*difHit;
		double diff = recoPos - (it->realHitX);
		
		if ( secondMostChargeHit.charge > mostChargeHit.charge)
		{
			std::cout << "panic" << std::endl;
		}

		double eta1;
		if (secondMostChargeHit.x > mostChargeHit.x)
		{
			eta1 = secondMostChargeHit.charge;
		}
		else
		{
			eta1 = mostChargeHit.charge;
		}
	
		if (!secondMostChargeHit.charge == 0)
		{
	
			double etaRes = eta1/(mostChargeHit.charge+secondMostChargeHit.charge);
			eta->Fill(etaRes);

			resX->Fill(diff);
			resXnaive->Fill(posMostChargeHit - (it->realHitX));
		}	
		else
		{
			singleHits++;
		}

		chargeMost->Fill(mostCharge);
		chargeSecond->Fill(secondMostCharge);
		
	}
	std::cout << singleHits << std::endl;
	eta->Write();

	double norm = eta->Integral(1,200);

	for(int bin = 1	; bin <= 200; bin++)
	{
		etacorr->SetBinContent(bin, eta->Integral(1,bin)/norm);
	}
	chargeMost->Write();
	chargeSecond->Write();
	etacorr->Write();
	resX->Write();
	resXnaive->Write();
	hitX->Write();
	hitY->Write();
	myTFile->Close();


	std::set<std::pair<int, int>> hits;

	for (auto it = begin((eventVec.at(0)).hitVec); it != end((eventVec.at(0)).hitVec); ++it)
	{
		hits.insert(std::make_pair(it->x, it->y));
	}

	
	//clean up and return 1 to OS
	delete my_PixelGeoDescr;
	return 1;
};
