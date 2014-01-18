#ifndef __EVENTDEFINES_H
#define __EVENTDEFINES_H

//std includes
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

#endif //__EVENTDEFINES_H
