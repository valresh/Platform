#include "stdafx.h"
#include "IV.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

CIV::CIV( char * _ObjName, char* _Type ) :	CValve_A_b ( _ObjName, _Type ) 
#include "ACS_Constr.h"
#include "IV_ACS.h"
{
#include "IO_Clear.h"
#include "IV_Pnt.h"
#include "IV_Defects.h"
  pRef = NULL;
}

CIV::~CIV()
{

}

int CIV::SaveState( )
{
	S_CLASS("_W_IV", _W_IV);
	return CValve_A_b::SaveState();
}

int CIV::RestoreState( char * StrName )
{
	COPY_FIRST;
	R_CLASS("_W_IV", _W_IV);
	return CValve_A_b::RestoreState(StrName);	
}
