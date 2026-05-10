#include "stdafx.h"
#define DLL_IntReg
#include "IntReg.h"
#include "math.h"
#include "Err.h"
#include "Connect.h"

#ifdef LINUX
#define CParams QParams
#endif

bool CIntReg::PrintUsed = true;

CIntReg::CIntReg( IBaseModel * pMain, char * Name ) : IBaseModel ( Name, pMain ), ISet ( this )
  {
	#include "IO_Clear.h"
//
  char * P = strchr ( Name, '|' );
  if ( P )
    {
    char TrueName[256];
    int L = (int)(P - Name);
    memcpy ( TrueName, Name, L );
    TrueName[L] = 0;
    ObjName = TrueName;
    Descr = P + 1;
    }
  else
    Descr = NULL;
//
  Error = false;
  Model = "IntReg";
  pMain->AddSet( this );
  PID = false;
  PID_P = 2000.;
  PID_I = 10.;
  En1 = En = 0.;
  Work = true;
  NotRelease = true;
  }

CIntReg_W::CIntReg_W()
	{
	memset ( this, 0, sizeof ( CIntReg_W ));
	}

int CIntReg::Step0()
  {
  KKK();
  return 0;
  }

int CIntReg::Step1()
  {
  if ( PrintUsed && Use )
    Fsend ( "Регуляторы.csv", "%s;\n", (char*)ObjName );
  return 0;
  }

#undef DESC
#define DESC(Desc) \
  {\
  if ( !CParams::SaveAllParams )\
    {\
    CParams & __P = CParams::GetLast();\
    __P.pModelDescr = (char*)Desc;\
    __P.Class = 'Z';\
    }\
  }

#undef PARM
#define PARM(Val,Name) \
  {\
  if ( CParams::AddParmToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
	  __P.Set( this, Name, Val, sizeof ( Val ) );\
    __P.Property = false;\
    __P.Class = 'Z';\
    }\
  }

int CIntReg::GetParams( char * NameTab )
	{
  if ( !Work )
    {
    Use = false;
    return 0;
    }
  char Name[128];
  char * pName = ObjName;
  while ( 1 )
    {
    char * pNext = strchr ( pName, '/' );
    if ( pNext == NULL )
      break;
    pName = pNext + 1;
    }
  sprintf_s ( Name, 128, "%s", pName );
  PARM( Value, $ Name );
  if ( Descr )
    {
    DESC( Descr );
    }
  sprintf_s ( Name, 128, "Задать '%s'", pName );
  PARM( Use, $ Name ); DESC( "Вкл/Выкл задания" );
  if ( PID )
    {
    PARM( PID_P, "P" );
    PARM( PID_I, "I" );
    }
	return 0;
	}


int CIntReg::UpdateParam( struct CParams & Param )
{
	if(pMain)
		return pMain->UpdateParam(Param);
	return 1;
}


void CIntReg::OutValue ( char Txt[64] )
  {
	if ( !_finite( Value ) || fabs( Value ) > 1e10 )
		{
    lstrcpy ( Txt, "NaN" );
		return;
		}
	if ( Value == 0. )
		lstrcpy ( Txt, "0" );
  else
	if ( fabs ( Value ) < 1e-5 )
		sprintf_s ( Txt, 64, "%13.5le", Value );
  else
	if ( fabs ( Value ) < 1e-4 )
		sprintf_s ( Txt, 64, "%11.9lf", Value );
  else
	if ( fabs ( Value ) < 1e-3 )
		sprintf_s ( Txt, 64, "%10.8lf", Value );
  else
	if ( fabs ( Value ) < 1e-2 )
		sprintf_s ( Txt, 64, "%9.7lf", Value );
  else
	if ( fabs ( Value ) < 1e-1 )
		sprintf_s ( Txt, 64, "%8.6lf", Value );
  else
	if ( fabs ( Value ) < 1. )
		sprintf_s ( Txt, 64, "%7.5lf", Value );
  else
	if ( fabs ( Value ) < 10. )
		sprintf_s ( Txt, 64, "%6.4lf", Value );
  else
	if ( fabs ( Value ) < 100. )
		sprintf_s ( Txt, 64, "%5.3lf", Value );
  else
	if ( fabs ( Value ) < 1000. )
		sprintf_s ( Txt, 64, "%5.2lf", Value );
  else
	if ( fabs ( Value ) < 10000. )
		sprintf_s ( Txt, 64, "%5.1lf", Value );
  else
	if ( fabs ( Value ) > 10000000. )
		sprintf_s ( Txt, 64, "%e", Value );
  else
		sprintf_s ( Txt, 64, "%5.0lf", Value );
  }

int CIntReg::SaveState( )
  {
	S_CLASS("W",CIntReg_W)
  return 0;
  }

int CIntReg::RestoreState( char * StrName )
  {
//  COPY_FIRST
	R_CLASS("W",CIntReg_W)
  return 1;
  }

void CTRL_Params( char * TabName, IBaseModel * pModel, const char * Descr )
  {
  static int Show = 0;
  if ( TabName && TabName[0] )
    {
    CParams & __P = CParams::Get();
    __P.SetDefaultValue(NULL);
    __P.Set( pModel, $ TabName, Show, 4 );
    __P.Type = 'T';
    __P.Class = 'Z';
    __P.pModelDescr = (char*)Descr;
    }
//.................................
  ISet * pSet = pModel->pFirstISet;
  while ( pSet )
    {
    pSet->GetParam( );
    pSet = pSet->pModelNextISet;
    }
  if ( TabName && TabName[0] )
    {
    CParams & __PE = CParams::Get();
    __PE.SetDefaultValue(NULL);
    __PE.Set( pModel, $ TabName, Show, 4 );
    __PE.Type = 'E';
    __PE.Class = 'Z';
    __PE.pModelDescr = (char*)NULL;
    }
  }

// MV = [0,1]
// PV = [0,Value]
// SP = Value
bool CIntReg::Reg( double dt, double PV, double * MV )
  {
  if ( !Work )
    {
    Use = false;
    return false;
    }
  if ( En == 0. )
    En1 = En = PV - Value;
  else
    {
    En1 = En;
    En = PV - Value;
    }
  SET_BP BreakPoint;
  dt *= 3600.;
  double Ks = 1. / Value;
  double Kp = 100. / PID_P;
  double dEn = En - En1;
  double dMV = Kp * Ks * ( dEn + dt / PID_I * En );
    //
  *MV += dMV;
  if ( *MV < 0. )
    *MV = 0.;
  if ( *MV > 1. )
    *MV = 1.;
  return true;
  }



void CExtReg::StepT ( double dt )
{
	if ( pPV == NULL )
		return;
	PV = *pPV;
	if ( fabs ( Max - Min ) > 1e-5 )
		Delta =  ( PV - SP ) / ( Max - Min ) * 100.;
	else
		Delta = 0.;
	if ( pFirstOut == NULL )
		return;
	if ( !On )
		return;
	dVm	= dV;
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
}
