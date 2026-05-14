#pragma once
#define ACS_LIB_NAME				"ACS.dll"
#define ACS_GET_NAME				"GetACSInterface"
struct SAdressValue
{
	void * Point;
	char Letter;
};

struct ACSInterface
{
	virtual SAdressValue GetACS(const char * Name, const char * Parameter) = 0;
};

typedef ACSInterface * (*AddressACSInterface)();
