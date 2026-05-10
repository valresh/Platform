#include "stdafx.h"
#include "CC_1.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

LPCSTR g_szHelpFileName = "Базовый ЦК.pdf";

CCC_1::CCC_1( char * _ObjName, char * Param ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
, ST_1 ( "Ступень", this )
{
#include "IO_Clear.h"
#include "Points.h"
#include "CC_1_Defects.h"
	UNIMODEL;
	ObjName = _ObjName;
    Model = "CC";
    Defect = Исправен;
	CanPuskStop = true;
	ErrInBlk = false;
	FlowQ = 0.;
	DefectStop = false;
	RazGerm = false;
	OmegaRazGerm = 0.;
	kExternal = 0;
	k_Pnt_Q = 20;
	aF = 0, cF = 0;
	char * P = strstr ( Param, "РХ:" );
	if ( P )
	{
		k_Pnt_Q = atoi ( P + 3 );
	}
	pMotor = NULL;
	MotorData.pOn = &On;
	MotorData.pRelOborot = &Oborot;
}

CCC_1::~CCC_1()
	{
	}

int CCC_1::Init( int Regim )
	{
	int Res = 0;
//  ASS(GiveCommData( ObjName,  CPumpData::TypeID,	(void**)&pPump)==0);
  CanPuskStop = true;
  ErrInBlk = false;
  if ( k_Pnt_Q > 0 )
    k_Points = k_Pnt_Q;
  if ( k_Points > 0 )
    {
    F_Характеристики = NewArr (double,(k_Points+1));
    P_Характеристики = NewArr (double,(k_Points+1));
    S_Характеристики = NewArr (char16,(k_Points+1));
    double X_Nom = Flow_Nom / Flow_Surge - 1.;
    if ( X_Nom < 0.01 )
      X_Nom = 0.01;
    // 1 - A * X * X = Спад
    double A = ( 1. - Спад ) / ( X_Nom * X_Nom );
    double Xmax = sqrt ( 1. / A );
    double dX = Xmax / k_Points;
    for ( int n = 0; n <= k_Points; n++ )
      {
      double X = n * dX;
      double P = 1. - A * X * X;
      F_Характеристики[n] = X; 
      P_Характеристики[n] = P; 
      }
    // Компенсируем лианеризацию
    int n = 0;
    while ( X_Nom > F_Характеристики[n+1] && n < k_Points )
      n++;
    double F0 = F_Характеристики[n];
    double F1 = F_Характеристики[n+1];
    double P0 = P_Характеристики[n];
    double P1 = P_Характеристики[n+1];
    double Q = P0 + ( X_Nom - F0 ) * ( P1 - P0 ) / ( F1 - F0 );
    double K = Спад / Q;
    //  Спад = P0 + ( X_Nom - F0 * kF ) * ( P1 - P0 ) / (( F1 - F0 ) * kF);
    double kF = X_Nom * ( P0 - P1 )/( Спад *( F0 - F1 ) + P0 * F1 - F0 * P1 );
    for ( int n = 0; n <= k_Points; n++ )
      {
      F_Характеристики[n] *= kF; 
      }
    F0 = F_Характеристики[n];
    F1 = F_Характеристики[n+1];
    Q = P0 + ( X_Nom - F0 ) * ( P1 - P0 ) / ( F1 - F0 );
    K = Спад / Q;
    KKK();
    }
	return 0;
	}

int CCC_1::Step0( )
	{
// = R * T / P
  To_m3_in = RP * 293.15;
	return 0;
	}

int CCC_1::Step1( )
	{
	if( bDefect )
	  {
		ObjRoot = GetDefect( RootNode );
		ObjRoot->Show = true;
		ObjRoot->Flags |= CDef::Active;
	  }
  if ( k_Pnt_Q > 0 )
    k_Points = k_Pnt_Q;
  ST_1.Calc_K_P2_P1( );
	return 0;
	}

int CCC_1::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CCC_1::SetData( int TypeData, void * pData )
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
        ST_1.Flow_0 = atof( pP->Value );
        if ( ST_1.Flow_0 < 10. || ST_1.Flow_0 > 500000.  )
          {
          pP->Msg = "Неправильное значение подачи, нм3";
          return -1;
          }
      EIF
        IF("Напор")
        if ( pP->Value[0] == 0 )
          return 0;
        ST_1.k_dP_0 = atof( pP->Value );
        if ( ST_1.k_dP_0 < 0.01 || ST_1.k_dP_0 > 300.  )
          {
          pP->Msg = "Неправильное значение напора, атм";
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

int CCC_1::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CCC_1::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W",_W)
  return 1;
  }


int CCC_1::DstMM( char * DataName, struct IMM * pMM )
{
	if (!lstrcmp( DataName, "Motor"))
	{
		ASS( pMM->ID == CMotorData::TypeID );
		pMotor = (CMotorData*)pMM->pData;
		if(pMotor->ppPower)
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

int CCC_1::SrcMM( char * DataName, struct IMM * pMM )
{

	if ( lstrcmp( DataName, "Motor" ) == 0 )
	{
		pMM->ID = CMotorData::TypeID;
		pMM->pData = &MotorData;
		pMM->ObjName = ObjName;
		return 0;
	}		
	if ( Lstrcmp( DataName, "Разгерметизация" ) == 0 )
	{
		pMM->ID = 0x2B8C4C64;
		pMM->pData = Разгерметизация;
		pMM->ObjName = ObjName;
		return 0;
	}
	if ( Lstrcmp( DataName, "Утечка" ) == 0 )
	{
		pMM->ID = 0x2B8C4C64;
		pMM->pData = Утечка;
		pMM->ObjName = ObjName;
		return 0;
	}
	return 2;
}
