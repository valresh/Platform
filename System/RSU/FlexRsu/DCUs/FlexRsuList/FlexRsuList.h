#pragma once
#ifdef FLEXRSULIST_EXPORTS
#define FLEXRSULIST_API _EXP
#else
#define FLEXRSULIST_API _IMP
#endif

// This class is exported from the FlexRsuList.dll
class FLEXRSULIST_API CFlexRsuList {
public:
	CFlexRsuList(void);
	// TODO: add your methods here.
};

extern FLEXRSULIST_API int nFlexRsuList;

FLEXRSULIST_API int fnFlexRsuList(void);
