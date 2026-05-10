#include "stdafx.h"
#define DLL_ACS
#include "ACS_Def.h"
#include "math.h"
#include "Err.h"
#include "DataTypes.h"
#include "Defects.h"
//#include "SetData.h"
//#include "Start.h"
#include "SysDataTypes.h"
#include "Connect.h"
#include "CommProc.h"
#include "Extern_Pnt.h"

CAnalog::CAnalog( IBaseModel * _pModel, char * _Point, char * _Type, char * _Descr ) :
    CExtern_Pnt( _pModel, _Point, _Type, _Descr )
{
    IsConnection = false;
    pSrcAddr = NULL;
    Value = 0.;
    A = 1.;
    B = 0.;
    Mes = -1;
    Scale = 1.;
}


CAnalogD::CAnalogD( IBaseModel * _pModel, char * _Point, char * _Type, char * _Descr ) : 
    CAnalog( _pModel, _Point, "AOD", _Descr )
{
    SH = 1e10;
    SL = 1e10;
    HH = 1e10;
    LL = 1e10;
    PH = 1e10;
    PL = 1e10;
    Defect = -1;
    UseCoef = false;
    Coef = 1.;
    Mes = -1;
    Scale = 1.;
}

void CAnalog::SetMes( int _Mes, double _Scale )
{
    Mes = _Mes;
    Scale = _Scale;
    if ( Scale < 0. )
        Mes = -Mes;
}

bool CAnalog::Connect( CConnectInfo * pInfo )
{
    if ( !CompTypes( pInfo->Type ))
        return false;
    if ( Type[1] == 'I' )
    {
        pSrcAddr = (double*)pInfo->pSrcAddr;
        pInfo->pSrcAddr = NULL;
    }
    else
    {
        pSrcAddr = NULL;
        pInfo->pSrcAddr = &Value;
    }
    Connection = pInfo->ConnName;
    IsConnection = true;
    //  Mes = pInfo->M;
    //  Scale = 1.;
    return true;
}

bool CAnalogD::Connect( CConnectInfo * pInfo )
{
    if ( !CompTypes( pInfo->Type ))
        return false;
    if ( Type[1] == 'I' )
    {
        pSrcAddr = (double*)pInfo->pSrcAddr;
        pInfo->pSrcAddr = NULL;
    }
    else
    {
        pSrcAddr = NULL;
        pInfo->pSrcAddr = &Value;
    }
    Connection = pInfo->ConnName;
    IsConnection = true;
    Defect = 0;
    return true;
}

void CAnalog::GetParams( )
{
    char Name[256];
    //  if ( pGetAllConnections == NULL )
    //    return;
    if ( CParams::SaveAllParams )
        return;
    char * RSU[32];
    char * Desc[32];
    CLEAR(RSU)
    CLEAR(Desc)
    char Pnt[256];
    lstrcpy ( Pnt, Point );
    char * Dvd = strchr ( Pnt, '|' );
    char * Ref = Pnt;
    if ( Dvd )
    {
        *Dvd = 0;
        Ref = Dvd + 1;
    }
    char IO = SD;
    int K = 0;
    //##
    if ( pGetAllConnections )
    {
        K = (*pGetAllConnections)( pModel->ObjName, Ref, IO, RSU, Desc );
        if ( K == 0 && IsConnection && Connection.Str && Connection.Str[0] == '#' )
        {
            K = 1;
            IO = SD = 'D';
            RSU[0] = "Задано";
            Desc[0] = "Задается вручную";
        }
    }
    if ( K <= 0 )
    {
        if ( CParams::GetParamsList )
        {
            sprintf_s ( Name, 256, "@%s", Pnt );
        }
        else
        {
            if ( Type[1] == 'I' )
                sprintf_s ( Name, 256, "@%s«-*", Pnt );
            else
                if ( Type[1] == 'O' )
                    sprintf_s ( Name, 256, "@%s–»*", Pnt );
                else
                    sprintf_s ( Name, 256, "@%s", Pnt );
        }
        CParams & P = CParams::Get();
        P.SetDefaultValue(NULL);
        P.Set( pModel, Name, Value, 8 );
        P.pModelDescr = Descr;
        P.pDefValue = Type;
        P.Class = 'A';
        return;
    }
    else
    {
        ASS( IO == 'S' || IO == 'D' )
        for ( int n = 0; n < K; n++ )
        {
            if ( IO == 'D' )
                sprintf_s ( Name, 256, "@%s«-(%s)", Pnt, RSU[n] );
            else
                sprintf_s ( Name, 256, "@%s–»(%s)", Pnt, RSU[n] );
            CParams & Param = CParams::Get();
            Param.SetDefaultValue(NULL);
            Param.Set( pModel, Name, Value, 8 );
            Param.pModelDescr = Desc[n];
            Param.Class = 'A';
        }
    }
}


void CAnalogD::GetParams( )
{
    if ( CParams::SaveAllParams )
        return;
    CAnalog::GetParams( );
}

/*
bool ConnToPnt ( IBaseModel * pRefModel, char * Point, double * pVar )
  {
  char Pnt[256];
  lstrcpy ( Pnt, Point );
  char * P = strchr ( Pnt, ';' );
  if ( P == NULL )
    return false;
  *P = 0;
  P++;
  IBaseModel * pModel = IBaseModel::Find ( Pnt );
	if ( pModel == NULL )
    return false;
	CACS_Pnt * pPnt = CACS_Pnt::pFirst;
/////////////////////////////////////////////////
	while ( pPnt )
	  {
		if ( pPnt->pModel != pModel )
		  {
			pPnt = pPnt->pNext;
			continue;
			}
	  if ( lstrcmp ( pPnt->Point, P )	== 0 )
      break;
	  pPnt = pPnt->pNext;
    }
  if ( pPnt == NULL )
    return false;
  CConnectInfo Info;
  Info.Type = "?";
  if ( pRefModel )
    Info.pACSName = pRefModel->ObjName;
  else
    Info.pACSName = "";
  Info.pVar = pVar;
  Info.Param = "";
  Info.pRefModel = pRefModel;
  pPnt->Connect( &Info );
  return true;
  }
*/

///////////////////////////////////////////////////////////////////////////
enum eDefect { eИсправен = 0, eПоказания_не_изменяются, eЗавышение_показаний, eЗанижение_показаний,
               eУход_показаний_на_максимум, eУход_показаний_на_начало_шкалы, eСамопроизвольное_изменение_показаний,
               eХаотическое_изменение_показаний,
               eСигнализация_HH, eСигнализация_H, eСигнализация_L, eСигнализация_LL,
               eСигнализация_IOPp, eСигнализация_IOPm,
               };

#define IF_DEFECT(Def,Comment) \
if ( pDefect == NULL || pDefect == Def )\
    {\
            Defect = e##Def;

#define END_IF \
        if ( pDefect )\
                return 1;\
    }

int CAnalogD::SetDefect ( struct CDef * pDefect )
{
    IF_DEFECT(Исправен, "Исправен" );
    END_IF
        //
        IF_DEFECT(Показания_не_изменяются, "Показания не изменяются" );
    Finish = Value;
    END_IF
        IF_DEFECT(Завышение_показаний, "Завышение показаний" );
    p1 = parm_Завышение_показаний->Dbl;
    p2 = 0.;
    END_IF
        IF_DEFECT(Занижение_показаний, "Занижение показаний" );
    p1 = parm_Занижение_показаний->Dbl;
    p2 =  0.;
    END_IF
        IF_DEFECT(Уход_показаний_на_максимум, "Уход показаний на максимум" );
    Finish = 100000.;//SH * 1.001;
    END_IF
        IF_DEFECT(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
    Defect = eУход_показаний_на_начало_шкалы;
    Finish = 0.;////SL * 0.999;
    END_IF
        IF_DEFECT(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
    Alfa = 0.;
    Start = Value;
    Finish = parm_Самопроизвольное_изменение_показаний->Dbl;// "Конечное значение", 0., 0., 300. )
    Time = time_Самопроизвольное_изменение_показаний->Dbl;// "Время изменения с", 0., 0., 3600. )
    END_IF
        IF_DEFECT(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
    p1 = min_Хаотическое_изменение_показаний->Dbl;// "Минимальное значение", 0., 0., 1000. )
    p2 = max_Хаотическое_изменение_показаний->Dbl;// "Максимальное значение", 0., 0., 1000. )
    Prop = 0.9;
    if ( p1 > p2 )
    {
        double w = p1;
        p1 = p2;
        p2 = w;
    }
    Finish = Value;
    END_IF
        IF_DEFECT(Сигнализация_HH, "Сигнализация HH" );
    Finish = HH * 1.001;
    END_IF
        IF_DEFECT(Сигнализация_H, "Сигнализация HI" );
    Finish = PH * 1.001;
    END_IF
        IF_DEFECT(Сигнализация_L, "Сигнализация LO" );
    Finish = PL * 0.999;
    END_IF
        IF_DEFECT(Сигнализация_LL, "Сигнализация LL" );
    Finish = LL * 0.999;
    END_IF
        IF_DEFECT(Сигнализация_IOPp, "Сигнализация IOP+" );
    Finish = SH + 5.;
    END_IF
        IF_DEFECT(Сигнализация_IOPm, "Сигнализация IOP-" );
    Finish = SL - 5.;
    END_IF
        return 0;
}

#undef IF_DEFECT
#define IF_DEFECT(Def,Comment) \
if ( Defect == e##Def )\
    {

#undef END_IF
#define END_IF \
    }

double CAnalogD::SetDefect( double V )
{
    if ( Defect < 0 )
        return V;
    IF_DEFECT(Исправен, "Исправен" );
    return V;
    END_IF
        //
        IF_DEFECT(Показания_не_изменяются, "Показания не изменяются" );
    return Finish;
    END_IF
        IF_DEFECT(Завышение_показаний, "Завышение показаний" );
    return V * p1;
    END_IF
        IF_DEFECT(Занижение_показаний, "Занижение показаний" );
    return V / p1;
    END_IF
        IF_DEFECT(Уход_показаний_на_максимум, "Уход показаний на максимум" );
    return Finish;
    END_IF
        IF_DEFECT(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
    return Finish;
    END_IF
        IF_DEFECT(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
    double dt = pSys->dt * 3600.;
    if ( Alfa < 1. )
    {
        if ( Time <= dt )
            Alfa = 1.;
        else
        {
            Alfa += dt / Time;
            if ( Alfa > 1. )
                Alfa = 1.;
        }
    }
    return Start + Alfa * ( Finish - Start );
    END_IF
        IF_DEFECT(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
    double Ksi = r01();
    double V = Finish;
    double A = 0.2;
    if ( Ksi < Prop )
        V = ( 1. - A ) * V + A * (double)p1;
    else
        V = ( 1. - A ) * V + A * (double)p2;
    Finish = V;
    if ( fabs ( V - p1 ) < 0.05 * ( p2 - p1 ))
        Prop = 0.1;
    if ( fabs ( V - p2 ) < 0.05 * ( p2 - p1 ))
        Prop = 0.9;
    return Finish;
    END_IF
        IF_DEFECT(Сигнализация_HH, "Сигнализация HH" );
    return Finish;
    END_IF
        IF_DEFECT(Сигнализация_H, "Сигнализация HI" );
    return Finish;
    END_IF
        IF_DEFECT(Сигнализация_L, "Сигнализация LO" );
    return Finish;
    END_IF
        IF_DEFECT(Сигнализация_LL, "Сигнализация LL" );
    return Finish;
    END_IF
        IF_DEFECT(Сигнализация_IOPp, "Сигнализация IOP+" );
    return Finish;
    END_IF
        IF_DEFECT(Сигнализация_IOPm, "Сигнализация IOP-" );
    return Finish;
    END_IF
        return 0.;
}

