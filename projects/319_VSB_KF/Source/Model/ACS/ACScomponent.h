#pragma once
#include "Interface/ACSInterface.h"

class ACScomponent : public ACSInterface
{
public:
	virtual SAdressValue GetACS(const char * Name, const char * Parameter);
};
