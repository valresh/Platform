#include "stdafx.h"
#include "PC_1.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"
#include "MM.h"

LPCSTR g_szHelpFileName = "Базовый ПК.pdf";

CPC_1::CPC_1( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
, ST_1 ( "Ступень 1", this )
{
	UNIMODEL
		ObjName = _ObjName;
#include "IO_Clear.h"
#include "Points.h"
#include "PC_1_Defects.h"
	RazGerm = false;
	OmegaRazGerm = 0.;
	FlowQ = 0.;
	pMotor = NULL;
	CanPuskStop = true;
	ErrInBlk = false;
	DefectStop = false;
	//  UseCtrl = false;
	DefectPow = false;
	RelDefectPow = 1.;
	MotorData.pOn = &On;
	MotorData.pRelOborot = &Oborot;
  ST_1.pCompr = this;
}

CPC_1::~CPC_1()
{
}

int CPC_1::Init( int Regim )
{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
	//ASS(GiveCommData( ObjName,  CPumpData::TypeID,	(void**)&pPump)==0);
	return 0;
}

int CPC_1::Step0( )
{
	// Fsend ( "Опасные объекты.csv", "%s;Да;\n", ObjName );
	return 0;
}

int CPC_1::Step1( )
{
	//  UseCtrl = false;
	//  if ( Задание_25.Use() || Задание_50.Use() || Задание_75.Use() || Задание_100.Use() )
	//    UseCtrl = true;
	if(bDefect)
	{
		ObjRoot = GetDefect( RootNode );
		ObjRoot->Show = true;
		ObjRoot->Flags|=CDef::Active;
	}
	//	ST_1.Thermo.bGamma = bGamma;
	return 0;
}

int CPC_1::StepT ( double dt )
{
	//  if ( Задание_25.Use() || Задание_50.Use() || Задание_75.Use() || Задание_100.Use() )
	//    UseCtrl = true;
	Calc ( dt );
	return 0;
}

int CPC_1::SetData( int TypeData, void * pData )
{
#undef IF
#define IF(Parm) if ( strcmp ( pP->Name, Parm ) == 0 ) {
#undef EIF
#define EIF return 0; }
	switch ( TypeData )
	{
	case sd_GetPow:
		{
			*(double**)pData = &ST_1.Pow; 
			return sd_GetPow;
		}
	case sd_GetOverGasSource:
		{
			*(double**)pData = &FlowQ; 
			return sd_GetOverGasSource;
		}
	case sd_SetEquipParam:
		{
			CEquipParam * pP = (CEquipParam*)pData;
			IF("Подача")
				if ( pP->Value[0] == 0 )
					return 0;
			ST_1.Flow_Nom_m3 = atof( pP->Value );
			if ( ST_1.Flow_Nom_m3 < 10. || ST_1.Flow_Nom_m3 > 500000.  )
			{
				pP->Msg = "Неправильное значение подачи, нм3";
				return -1;
			}
			EIF
		}
	case sd_GetHelpPath:
		{
			lstrcpy((char*)pData, HELP_PATH);
			lstrcat((char*)pData, g_szHelpFileName);
			return 1;
		}
	}
	return 1;
}

int CPC_1::SaveState( )
{
	S_CLASS("W",_W)
		return 0;
}

int CPC_1::RestoreState( char * StrName )
{
	COPY_FIRST
		R_CLASS("W",_W)
		return 1;
}

int CPC_1::DstMM( char * DataName, struct IMM * pMM )
{
	if (!lstrcmp( DataName, "Motor"))
	{
		ASS( pMM->ID == CMotorData::TypeID );
		pMotor = (CMotorData*)pMM->pData;
		*pMotor->ppPower = &ST_1.Pow;
		strcpy_s ( Мотор, 31, (char*)pMM->ObjName );
		return 0;
	}
	if ( strncmp( DataName, "Отказы", 6 ) == 0 )
	{
		ASS( pMM->ID == 0x2B8C4C64 )
			if(!pMM->pData)
				return 1;
		CDef* pDef = NULL;
		LastDefect = NewDefect( );
		pDef = GetDefect( LastDefect );
		pDef->Parent = RootNode;
		lstrcpy ( pDef->Name, pMM->Description );
		lstrcpy ( pDef->ObjName, pMM->ObjName );
		pDef->Flags = CDef::Defects|CDef::Defect|CDef::Active;
		pDef->ScriptID = 0;
		//pDef->NumbScript = -1;
		pDef->pModel = this;
		pDef->pNextDefect = (CDef*)pMM->pData;
		pDef->Param = ((CDef*)pMM->pData)->Param;
		defVector.push_back(pDef);
		return 0;
	}
	return 1;
}

int CPC_1::SrcMM( char * DataName, struct IMM * pMM )
{
	if (!lstrcmp( DataName, "Motor"))
	{
		pMM->ID = CMotorData::TypeID;
		pMM->pData = &MotorData;
		pMM->ObjName = ObjName;
		return 0;
	}
	//if ( Lstrcmp( DataName, "Разгерметизация" ) == 0 )
	//  {
	//  pMM->ID = 0x2B8C4C64;
	//  pMM->pData = Разгерметизация;
	//  pMM->ObjName = ObjName;
	//  return 0;
	//  }
	if ( Lstrcmp( DataName, "Утечка" ) == 0 )
	{
		pMM->ID = 0x2B8C4C64;
		pMM->pData = Утечка;
		pMM->ObjName = ObjName;
		return 0;
	}
	if ( Lstrcmp( DataName, "Снижение производительности" ) == 0 )
	{
		pMM->ID = 0x2B8C4C64;
		pMM->pData = Снижение_производительности;
		pMM->ObjName = ObjName;
		return 0;
	}
	return 2;
}
