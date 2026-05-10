#include "stdafx.h"
#include "CtrlReg.h"
#include "Err.h"
#include "SetData.h"
#include "ACS_Def.h"

#ifdef LINUX
#define CParams QParams
#endif

double* Connect(const char* Pnt )
{
	if ( Pnt == NULL || Pnt[0] == 0 )
		return NULL;
	char Point[256];
	lstrcpy ( Point, Pnt );
	char * P = strchr ( Point, '.' );
	if ( P == NULL )
		return NULL;
	*P = 0;
	char * Obj = Point;
	IBaseModel * pModel = IBaseModel::Find ( Obj );
	if ( pModel == NULL )
		return NULL;
	char * Param = P + 1;
	if ( Param[0] == '@' )
	{
		// ACS
		Param++;
		CExtern_Pnt * pPnt = (CExtern_Pnt*)::Find( &pModel->Points, Param, false );
		if ( pPnt == NULL )
			return NULL;
		if ( pPnt->Type[0] != 'A' )
			return NULL;
		CAnalog * pA = (CAnalog*)pPnt;
		double * pAddr = (double*)&(pA->Value);
		return pAddr;
	}
	CLockParams Lock;
	pModel->GetParams( NULL );
	for ( int n = 0; n < CParams::kParams; n++ )
	{
		CParams & Parm = CParams::Params[n];
		if ( lstrcmp ( Parm.ParamName, Param ) == 0 && Parm.Type == 'D' )
		{
			double * pAddr = (double*)Parm.Addr;
			return pAddr;
		}
	}
	return NULL;
}


CExtRegOut* CExtRegOutEx::GetNextOut()
{
	return pNextOut;
}


CExtRegOut* CExtRegEx::GetFirstOut()
{
	return pFirstOut;
}


CExtReg* CExtRegEx::GetNextReg()
{
	return pNextReg;
}

CExtRegEx::CExtRegEx()
{
	pFirstOut = NULL;
	pNextReg = NULL;
	nTrend = -1;
	TrendScale = 10.;
	TrendStep = 2;
}



CExtRegOut* CExtRegEx::AddOut(const char* szRef, double MV_0, double MV_1)
{
	CExtRegOutEx* pOut = new CExtRegOutEx;	
	pOut->pNextOut = pFirstOut;
	pFirstOut = pOut;
	pOut->MV_0 = MV_0;
	pOut->MV_1 = MV_1;			
	pOut->pMV = Connect(szRef);
	//if(pOut->pMV)
		//*pOut->pMV = (MV_1 - MV_0) / 2.;
	strcpy_s(pOut->Ref, CExtRegOut::MAX_REG_NAME, szRef);
	return pOut;
}



void CExtRegEx::DeleteOut(CExtRegOut* pDelete)
{
	CExtRegOut* pOut = pFirstOut;
	if(pOut == pDelete)
	{
		pFirstOut = pDelete->pNextOut;
	}
	else
	{
		while ( pOut )
		{
			if ( pOut->pNextOut == pDelete )
			{
				pOut->pNextOut = pDelete->pNextOut;
				break;
			}
			pOut = pOut->pNextOut;
		}
	}
}

int CExtRegEx::StepT( double dt )
{
	if ( pPV == NULL )
		return 0;
	PV = *pPV;
	if ( fabs ( Max - Min ) > 1e-5 )
		Delta =  ( PV - SP ) / ( Max - Min ) * 100.;
	else
		Delta = 0.;
	if ( pFirstOut == NULL )
		return 0;
	if ( !On )
		return 0;
	dVm	= dV;
	if(Max - Min == 0.0)
		return 0;
	dV = ( PV - SP ) / ( Max - Min );
	Delta = dV * 100.;
	double dU;
	double dt_sec = dt * 3600.;
	if ( T > 0. )
		dU = K * (( dV - dVm ) + 0.5 * dt_sec * ( dV + dVm ) / T );
	else
		dU = K * dV;
	//
	OP += dU;
	if ( OP < 0. )
		OP = 0.;
	if ( OP > 1. )
		OP = 1.;
	CExtRegOut * pOut = pFirstOut;
	while ( pOut )
	{
		double V = pOut->MV_0 + OP * ( pOut->MV_1 - pOut->MV_0 );
		if ( pOut->pMV )
			*pOut->pMV = V;
		pOut = pOut->pNextOut;
	}
	return 0;
}

CExtReg* CCtrlReg::GetFirstReg()
{
	return pFirstReg;

}

CCtrlReg::CCtrlReg( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
{
	ObjName = _ObjName;
  Model = "CtrlReg";
    #include "IO_Clear.h"
	#include "Points.h"
	kStep = 0;
    pFirstReg = NULL;
    pLastReg = NULL;
}

CCtrlReg::~CCtrlReg()
{

}

int CCtrlReg::Init( int Regim )
{
	return 0;
}

void CCtrlReg::Save()
{
	Char<256>Path;
	Path.Prt ( "%sDATA\\Reg\\Reg.csv", PROJECT_ROOT );
	FILE * F;
	fopen_s(&F, Path, "wt");
	if ( F == NULL )
	{
		ModMsg( "Ошибка открытия файла '%s' на запись",(char*)Path );
		return;
	}
	fprintf( F, "Тип;Название;K;T;Td;Цель;Задание;Мин;Макс;\n" );
	//  Тип;Название;K;T;Td;Цель;Задание;Мин;Макс
	//    Вых;ХВ-1/1.Теплоотдача;10;100;;;;;
	CExtRegEx* pReg = pFirstReg;
	int N = 0;
	while ( pReg )
	{
		char * Type;
		if ( pReg->On )
			Type = "Рег";
		else
			Type = "Откл";
		fprintf( F, "%s;%s;%6.4lf;%5.3lf;%5.3lf;%s;%6.4lf;%5.3lf;%5.3lf;\n", 
			Type, pReg->Name, pReg->K, pReg->T, pReg->TD,
			pReg->szGoal, pReg->SP, pReg->Min, pReg->Max );
		CExtRegOut * pOut = pReg->pFirstOut;
		while ( pOut )
		{
			fprintf( F, "Вых;%s;%5.3lf;%5.3lf;\n", 
				pOut->Ref, pOut->MV_0, pOut->MV_1 );
			pOut = pOut->pNextOut;
		}
		pReg = pReg->pNextReg;
	}
	fclose ( F );
}

int CCtrlReg::Step0( )
{
	Char<256>Path;
	Path.Prt ( "%sDATA\\Reg\\Reg.csv", PROJECT_ROOT );
	FILE *F;
	fopen_s(&F, Path, "rt" );
	if ( F == NULL )
	{
		ModMsg( "#Ошибка открытия файла Reg.csv");
		return 0;
	}
	// Анализ
	char Str[256];
	fgets ( Str, 255, F );
	int Line = 1;
	pFirstReg = NULL;
	CExtReg * pReg = NULL;
	double MVs = 0.;
	double kMV = 0.;
	CExtReg* pLastReg = NULL;
	while ( fgets ( Str, 255, F ))
	{          
		int L = lstrlen ( Str ) - 1;
		Str[L] = 0;
		LPCSTR aColumn[COLUMN_COUNT];
		char* pChar = Str;
		for(unsigned int i = 0; i < COLUMN_COUNT; i++)
		{
			aColumn[i] = pChar;
			pChar = strchr(pChar, ';');
			if(!pChar)
				break;
			*pChar++ = 0;
		}
		if(!lstrcmp(aColumn[COLUMN_TYPE], "Рег"))
		{
			pLastReg = AddReg(aColumn[COLUMN_NAME], atof(aColumn[COLUMN_K] ), atof(aColumn[COLUMN_T]), atof(aColumn[COLUMN_TD]), aColumn[COLUMN_GOAL],
				atof ( aColumn[COLUMN_TASK] ), atof(aColumn[COLUMN_MIN]), atof(aColumn[COLUMN_MAX]), true);
		}
		else if(!lstrcmp(aColumn[COLUMN_TYPE],"Откл"))
		{
			pLastReg = AddReg(aColumn[COLUMN_NAME], atof(aColumn[COLUMN_K] ), atof(aColumn[COLUMN_T]), atof(aColumn[COLUMN_TD]), aColumn[COLUMN_GOAL],
				atof ( aColumn[COLUMN_TASK] ), atof(aColumn[COLUMN_MIN]), atof(aColumn[COLUMN_MAX]), false);
		}
		else if(!lstrcmp(aColumn[COLUMN_TYPE], "Вых" ))
		{
			if(pLastReg)
				pLastReg->AddOut(aColumn[COLUMN_NAME], atof(aColumn[COLUMN_K]), atof(aColumn[COLUMN_T]));			
		}
		else
		{
			ASS(0)
		}
	}
	fclose( F );
	return 0;
}

int CCtrlReg::Step1( )
{
	return 0;
}

int CCtrlReg::StepT ( double dt )
{
	SET_BP BreakPoint;
	if(kStep < 3)
	{
		kStep++;
		return 0;
	}
	CExtRegEx* pReg = pFirstReg;
	while(pReg)		
	{
		pReg->StepT(dt);
		pReg = pReg->pNextReg;
	}
	return 0;
}


CExtReg* CCtrlReg::AddReg( const char* szRegName, double K, double T, double TD, const char* szGoal, double SP, double Min, double Max, bool On )
{
	CExtRegEx * pReg = new CExtRegEx;	
	if ( pFirstReg == NULL )
		pFirstReg = pReg;
	if ( pLastReg )
		pLastReg->pNextReg = pReg;
	pLastReg = pReg;
	//
	pReg->nTrend = -1;
	pReg->TrendScale = 10.;
	pReg->TrendStep = 2;
	strcpy_s(pReg->Name, CExtRegEx::MAX_REG_NAME, szRegName);
	pReg->K = K;
	pReg->T = T;
	pReg->TD = TD;
	lstrcpy(pReg->szGoal, szGoal);
	pReg->SP = SP;
	pReg->PV = SP;
	pReg->Min = Min;
	pReg->Max = Max;
	pReg->pPV = Connect(szGoal);
	pReg->On = On;
	return pReg;
}



void CCtrlReg::ChangeReg( CExtReg* pReg )
{
	pReg->pPV = Connect(pReg->szGoal);
}



void CCtrlReg::DeleteReg( CExtReg* pDelete )
{
	CExtRegEx* pReg = pFirstReg;
	if(pReg == pDelete)
	{
		pFirstReg = pReg->pNextReg;
	}
	else
	{
		while ( pReg )
		{
			if ( pReg->pNextReg == pDelete )
			{
				pReg->pNextReg = pReg->pNextReg->pNextReg;
				break;
			}
			pReg = pReg->pNextReg;
		}
	}
}


int CCtrlReg::SetData( int TypeData, void * pData )
{
	return 1;
}

int CCtrlReg::SaveState( )
{	
	CExtRegEx* pReg = pFirstReg;
	while(pReg)		
	{
		pSRFile->WHeadStruct(pReg->Name, lstrlen(pReg->Name));
		pSRFile->WD(static_cast<CExtReg_W*>(pReg), sizeof(CExtReg_W));
		pSRFile->WD(static_cast<CExtRegEx_W*>(pReg), sizeof(CExtRegEx_W));
		pReg = pReg->pNextReg;
	}
	return 0;
}

int CCtrlReg::RestoreState( char * StrName )
{
	//R_CLASS("W",_W_CtrlReg)
	CExtRegEx* pReg = pFirstReg;
	CExtReg_W ExtReg;
	CExtRegEx_W ExtRegEx;
	ASS(pSRFile->RD(&ExtReg, sizeof ( CExtReg_W )));
	ASS(pSRFile->RD(&ExtRegEx, sizeof ( CExtRegEx_W )));
	while(pReg)
	{
		if(!lstrcmp(pReg->Name, StrName))
		{
			memcpy(static_cast<CExtReg_W*>(pReg), &ExtReg, sizeof ( CExtReg_W ));
			memcpy(static_cast<CExtRegEx_W*>(pReg), &ExtRegEx, sizeof ( CExtRegEx_W ));			
			break;
		}
		pReg = pReg->pNextReg;
	}
	return 0;
}
