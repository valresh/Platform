#include "stdafx.h"
#include "CV.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

CCV::CCV( char * _ObjName, char* _Type ) :	CValve_A_b ( _ObjName, _Type ) 
#include "ACS_Constr.h"
#include "CV_ACS.h"
{
#include "IO_Clear.h"
#include "CV_Pnt.h"
#include "CV_Defects.h"
	nDefect = 0;
	bIgnoreBlk = false;
	Strapping = 0;
	Zadv_In = Zadv_Out = 100.0;
	Zadv_Bypass = 0.0;
	lstrcpy(Type, "CV");
}

CCV::~CCV()
{
}

int CCV::SetData( int TypeData, void * pData )
{
	switch ( TypeData )
	{
	case sd_SetEquipParam:
		{
			CEquipParam* pEP = (CEquipParam*)pData;
            if(!lstrcmpi(pEP->Name, "Вход"))
			{
				if(atoi(pEP->Value) != 0)
				{
					Strapping |= STRAPPING_IN;
					lstrcpy(Type, "CVFA");
				}
				return 0;
			}
			else if(!lstrcmpi(pEP->Name, "Выход"))
			{
				if(atoi(pEP->Value) != 0)
				{
					Strapping |= STRAPPING_OUT;
					lstrcpy(Type, "CVFA");
				}
				return 0;
			}
			else if(!lstrcmpi(pEP->Name, "Байпас"))
			{
				if(atoi(pEP->Value) != 0)
				{
					Strapping |= STRAPPING_BAYPASS;
					lstrcpy(Type, "CVFA");
				}
				return 0;
			}
			return 2;
		}
	case sd_SetInitState:
		{
			Zadv_In = Zadv_Out = 100.0;
			Zadv_Bypass = 0.0;
			break;
		}
	}
	return CValve_A_b::SetData(TypeData, pData);
}

int CCV::SaveState( )
{
	S_CLASS("_W_CV", _W_CV);
	return CValve_A_b::SaveState();
}

int CCV::RestoreState( char * StrName )
{
	COPY_FIRST;
	R_CLASS("_W_CV", _W_CV);
	return CValve_A_b::RestoreState(StrName);	
}
