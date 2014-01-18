#ifndef __egal
#define __egal

#include "TObject.h"
#include "TROOT.h"

class MyClass: public TObject {
public:  
	MyClass();
	virtual ~MyClass();
	ClassDef(MyClass, 1) //TheClassTitle
};

#endif
