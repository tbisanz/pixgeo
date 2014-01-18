#ifndef __READEVENT_H
#define __READEVENT_H

#include "EventDefines.h"
#include <string>
#include <vector>


bool ReadEvent(std::string file, std::vector<event>& resultVec, std::string& lib);
	

#endif //__READEVENT_H
