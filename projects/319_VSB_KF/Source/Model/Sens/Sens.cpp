#include "stdafx.h"
#include "Sens.h"
#include "Err.h"
#include "CommProc.h"
#include "Chem.h"
#include "SetData.h"

LPCSTR g_szHelpFileName = "Базовый датчик.pdf";

char* CSens::pCompName = NULL;

char * AllMes = NULL;

CSens::CSens( char * _ObjName ) :	IBaseModel ( _ObjName ) 
, SensT("T",this)
#include "ACS_Constr.h"
#include "ACS.h"
//, Script ( this )
	{
	UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
//
//  pSens = NULL;
  ModelFlags |= MODEL_SENS;
  pF = NULL;
  kStep = 0;
  WasCalcT = false;
  Pin = Pout = 0.;
  F_mol = 0.;
  kAnalyzData = MAX_ANALYZ_POINTS;
  UseMin = false;
  UseMax = false;
	for ( int n = 0; n < MAX_ANALYZ_POINTS; n++ )
    k_Analyz[n] = 1.;
  pControlVal = NULL;
  МассовыеДоли = false;
  КривыеКипения = false;
  CycleTime = 1e10;
  CoefTBP = NULL;
  kCoefTBP = 0;
  AddCoefTBP = false;
  Defect = (eDefect)-1;
  //RoAppr = NULL;
  //MwAppr = NULL;
  //LogMwAppr = NULL;
  InvalidMin = false;
  InvalidMax = false;
  pGamma = NULL;
	}

CSens::~CSens()
	{
	}

//int Mes_abs_1 = -1;
//int Mes_abs_2 = -1;

int CSens::Init( int )
{
	int Res = 0;
	if ( AllMes == NULL )
		AllMes = MesNames();
	char * Name = ObjName;
	if ( *Name == '_' )
		Name++;
	return 0;
}

extern char * AddComp[];
extern int kAdd;

int CSens::Step0( )
	{
	if(!pCompName)
	{
		int nCompNameLen = 0;
		for ( int n = 0; n < LAST_GAS; n++ )
			nCompNameLen += lstrlen(pGas[n]->Name) + 1;
		for ( int n = 0; n < kAdd; n++ )
			nCompNameLen += lstrlen(AddComp[n]) + 1;

		nCompNameLen += 1;
		pCompName = new char[nCompNameLen];
		int nPos = 0;
		for ( int n = 0; n < LAST_GAS; n++ )
		{
			lstrcpy(&pCompName[nPos], pGas[n]->Name);
			nPos += lstrlen(pGas[n]->Name);
			lstrcat(&pCompName[nPos], ";");
			nPos += 1;
		}
		for ( int n = 0; n < kAdd; n++ )
      {
			lstrcpy(&pCompName[nPos], AddComp[n]);
			nPos += lstrlen(AddComp[n]);
			lstrcat(&pCompName[nPos], ";");
			nPos += 1;
      }
		pCompName[nPos] = 0;
	}
//  Output = 1;
  if ( Анализатор )
    {
    if ( КривыеКипения )
      {
      NewPrepareAnalyz( ); 
      }
    else
      {
      PrepareAnalyz( );
      SetAnalyzData( ); 
      }
    }
/* Чтобы убрать - убери первый /
  if (( MesZ >= 21 && MesZ <= 25 ) || MesZ == 126 )
    Tau = 1.0;
/**/
//  Fsend("Sensors.csv","%s;\n", (char*)ObjName);
  if ( Use_Mix_H2O_NH3_CO2 )
    {
    pGamma = GetPropInt<IGamma*>( "Gamma", "H2ONH3CO2" );
    pGamma ->Init( this, NULL );
    }
  return 0;
	}

int NewMes[21] = { 
	1 ,
	2 ,
	11,
	12,
	13,
	21,
	22,
	23,
	24,
	25,
	31,
	15,
	21 ,
	25 ,
	31 ,
	14 ,
	126,
	1  ,
	31 ,
	21 ,
	126,
};   

int ConvMes ( int OldMes )
  {
  if ( OldMes <= 0 )
    return OldMes;
  if ( OldMes > 21 )
    {
    ASS(0)
    return 0;
    }
  return NewMes[OldMes-1];
  }

int CSens::Step1( )
{    
	if ( Type == Нет_типа )									//Если тип не указан
	{
		int nObjNameLen = lstrlen(ObjName);			
		for(int i = 0; i < nObjNameLen;i ++)				
		{
			if( ObjName[i] < '0' || ObjName[i] > '9')		//Если имя объека не равно цифре
			{

				switch(ObjName[i])
				{
				case 'T':
					Type = Тип_Температура;
					break;
				case 'F':
					Type = Тип_Поток;
					break;
				case 'P':
					Type = Тип_Давление_и;
					break;
				case 'A':
					Type = Тип_Давление_а;
					break;
				case 'D':
					Type = Тип_Перепад;
					break;
				case 'R':
					Type = Тип_Плотность;
					break;
				case 'Q':
					Type = Тип_Концентрация;
					break;
				case 'L':
					Type = Тип_Уровень;
					break;
				}
				break;
			}		
		}
	}
	Сумма = 0.;
	//if ( Type[0] == 'D' )
	//  {
	//  Omega = 0.;
	//  }
	//  MesZ = MesZold;
	//if ( Convert )
	//  {
	//  MesZ = ConvMes ( MesZold );
	//  Convert = false;
	//  }
	if ( MesZ <= 0 )
	{
		if ( Type == Тип_Давление_и )
			MesZ = 21;
		if ( Type == Тип_Температура )
			MesZ = 31;
		if ( Type == Тип_Поток )
			MesZ = 13;
		if ( Type == Тип_Давление_а )
			MesZ = 126;
	}
	if ( Показания.Mes >= 0 )
		MesZ = Показания.Mes;
	Перепад.Mes = -1;
	Показания.Mes = -1;
	if ( Test_V )
		pControlVal = new CAnalogControl ( "Показания", this, &_Value, &V_test_nom, &V_test_delta );
	////////////////////////////
	CNoErrGasID NoErr;
	if ( HaveDefects )
		CreateDefects( );
	return 0;
} 

int CSens::StepT ( double dt )
	{
  Calc ( dt );
  if ( pControlVal )
    pControlVal->Test();
  kStep++;
	return 0;
	}

int CSens::SetData( int TypeData, void * pData )
{
	switch ( TypeData )
	{
	case sd_SetInitState:
		{
			if ( Type == Тип_Температура )
				Val_old = Phys = Value = 15.;
			else
				Val_old = Phys = Value = 0.;
			return 0;
		}
	case sd_SetSensMes:
		{
			int M = *(int*)pData;
			if ( M < 0 )
				return 0;
			//      MesZ = *(int*)pData;
			MesZ = M;		
			Перепад.Mes = -1;		//Чтобы не учитывать коэффициент в точке он учитывается в MesZ 						
			Показания.Mes = -1;	//если убрать получится что выставляется идиница изменрения и на нее умножается коэффициент переводящий в эту единицу
			//      Convert = false;
			return 0;
		}
	case sd_SetSensMin:
		{
			MinV = *(double*)pData;
			UseMin = true;
			return 0;
		}
	case sd_SetSensMax:
		{
			MaxV = *(double*)pData;
			UseMax = true;
			return 0;
		}
	case sd_AddToTrends:
		{
#include "IO_Trends.h"
			PARM_D( _Value, "Показания", 0. )
				return 0;
		}
	case sd_GetPressKIP:
		{
			Type = Тип_Давление_и;
			*(double**)pData = &Val_old;
			return 0;
		}
	case sd_SetEquipParam:
		{
			CEquipParam * pP = (CEquipParam*)pData;
			if ( strcmp ( pP->Name, "Номинал" ) == 0 )
			{
				Nominal = atof( pP->Value );
				return 0;
			}
			return 1;
		}
	case sd_ModelCmd:
		{
			CModelCmd * pCmd = (CModelCmd*)pData;
			if ( lstrcmp ( pCmd ->Cmd, "Ускорить" ) == 0 )
			{
				Tau = 1e-3;
				return sizeof ( CModelCmd );
			}
			return 1;
		}
	case sd_GetHelpPath:
		{
			lstrcpy((char*)pData, HELP_PATH);
			lstrcat((char*)pData, g_szHelpFileName);
			return 1;
		}
  case sd_GetMes:
    {
    *(int*)pData = MesZ;
    return 1;
    }
  case sd_GetSensInfo:
    {
    *(double*)pData = Показания;
    return 1;
    }
	}
	return 1;
}

int CSens::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CSens::RestoreState( char * StrName )
  {
	R_CLASS("W",_W)
  return 1;
  }
