#pragma once

#include "CommProc.h"

#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

struct IN_DLL CPropInfo
{
  CStr Name;
  CStr Val;
};

struct IN_DLL CPropertiesM
{
	CMem<CPropInfo,16,16>Props;
	bool FromProject;
    bool m_bWasLoad;

	CPropertiesM();
	~CPropertiesM();
	bool LoadProp();
	bool ReadProp();
	bool ReadProjectProps();
//	bool SaveProp();
//	bool SaveProp( int Start, int Finish, char * Path );
//	bool ClickGroup( int N );
    bool Get ( const char * Name, double & Val );
    bool Get ( const char * Name, int & Val );
    bool Get ( const char * Name, char Val[256] );
    bool Get ( const char * Name, bool & Val );
	char * GetVal ( char * Name );
    char * GetErrorMsg ();
//	void SetVisible( int N_Parent, bool Visible );
};


extern IN_DLL CPropertiesM PROPS;


