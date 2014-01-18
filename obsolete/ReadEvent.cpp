#include <iterator>
#include <sstream>
#include <fstream>
#include "ReadEvent.h"

//takes the input file and writes the used lib, sigma and 
bool ReadEvent(std::string file, std::vector<event>& resultVec, std::string& lib)
{
	std::ifstream myfile (file);
	std::string line;

	if (myfile.is_open())
	{
		getline(myfile,lib); //Read in which lib to use! (first line in sim-output)

		while ( getline(myfile,line) )
		{
			//stream for reading data and iterator
			std::istringstream buf(line);
			std::istream_iterator<std::string> beg(buf), end;
			std::vector<std::string> splitStrings(beg, end); 


			//read event info
			if(splitStrings[0]=="Event:")
			{
				//create event to store data into 				
				event myEvent;
				myEvent.eventNo = atoi(splitStrings[1].c_str());

				//scan over whole event
				bool eoe = false;
				eoe = !getline(myfile,line);
				while(line != "***" && !eoe)
				{
					std::istringstream buf2(line);
					std::istream_iterator<std::string> beg(buf2), end;
					std::vector<std::string> splitStrings2(beg, end); 

					if(splitStrings2[0]=="HitInfo:")
					{
						//in line "HitInfo:  " following format: xPos yPos sigma
						myEvent.realHitX = atof(splitStrings2[1].c_str()); 
						myEvent.realHitY = atof(splitStrings2[2].c_str()); 
						myEvent.sigma = atof(splitStrings2[3].c_str());
					}
					else
					{
						//Other info is collection of hit pixels in following format: X Y charge
						int x = atoi(splitStrings2[0].c_str());
						int y = atoi(splitStrings2[1].c_str());
						double charge = atof(splitStrings2[2].c_str());

						//store info in hit struct
						hit myHit;
						myHit.x = x;
						myHit.y= y;
						myHit.charge = charge;

						//push hit onto hit vector
						(myEvent.hitVec).push_back(myHit);
					}
					//read next hit of same event
					eoe = !getline(myfile,line);
				}
				//store read in event
				resultVec.push_back(myEvent);
			}
		}
		myfile.close();
	}
	else
	{
		return false;
	}
	return true;
}
