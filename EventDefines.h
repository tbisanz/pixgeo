#ifndef __EVENTDEFINES_H
#define __EVENTDEFINES_H

#include <vector>

//ROOT includes
#include "TObject.h"


class hit: public TObject
{
public:
	ClassDef(hit, 1);
	hit(){};
	virtual ~hit(){};
	int x;
	int y;
	double charge;
	
};

class event
{

public:
	int eventNo;
	std::vector<hit> hitVec;
	double realHitX;
	double realHitY;
	double sigma;
};

	

#endif //__EVENTDEFINES_H
