#pragma once

#include "SetDataTypes.h"
#include "CommProc.h"

struct CSetColData
{
	int TypeData;//-1,0,1
	struct CFlow * pFlow;
};

struct CAutoCommand
{
	struct IBaseModel * pModel;
	char Cmd_1[16];
	char Cmd_2[16];
	double Parm1;
	double Parm2;
	double MaxTime;
	int Result;
	int State;
};

struct CSensToRoom
  {
  char * SensName;
  double * FlowQ;
  };

struct CVentToRoom
  {
  double * Oborot;
  };

#define MAX_OBJ_NAME 64*4 //utf-8

struct COldName
	{
	char Name[MAX_OBJ_NAME+1];
	};

struct CFldData
	{
	char NameFrom[MAX_OBJ_NAME+1];
	char PntFrom[33];
	char TypeFrom[8];
	char OptFrom[17];
//
	char NameTo[MAX_OBJ_NAME+1];
	char PntTo[33];
	char TypeTo[9];
	char OptTo[17];
	};

struct CConnectPoint
	{
	char Point[17];
	enum eType { None, Byte, Int, Dbl };
	eType Type;
	void * pAddress;
	bool * pUseVar;
	};

#undef CONN_PNT
#define CONN_PNT(Var,Name) \
	if ( TypeData == sd_GetConnectPoint )\
		{\
		CConnectPoint * pCP = (CConnectPoint *)pData;\
		if ( lstrcmp ( Name, pCP->Point ) == 0 )\
			{\
			pCP->Type = CConnectPoint::Byte;\
			pCP->pAddress = &Var;\
			pCP->pUseVar = NULL;\
			return sd_GetConnectPoint;\
			}\
		}

#undef CONN_PNT_USE
#define CONN_PNT_USE(Var,Name) \
	if ( TypeData == sd_GetConnectPoint )\
		{\
		CConnectPoint * pCP = (CConnectPoint *)pData;\
		if ( lstrcmp ( Name, pCP->Point ) == 0 )\
			{\
			pCP->Type = CConnectPoint::Byte;\
			pCP->pAddress = &Var;\
			pCP->pUseVar = &Use_##Var;\
			return sd_GetConnectPoint;\
			}\
		}

typedef double (*tAppr) ( void * ApprArg,	double V );

struct CCompData
  {
  struct IBaseModel * pModel;
	double * pFlow_mol;
  struct CComp * pComp;
	double P, T;
	char CompName[128];
	tAppr pAppr;
	void * ApprArg;
	CCompData() { memset ( this, 0, sizeof ( CCompData ));	};
  };

struct C_ParamSelected
	{
	struct CParams * pParam;
	char ParamName[256];
	enum TypeAction { NoAction, SelectPipe, SelectObject, DialObject,
		                DialComp, DialNH3H2O, GotoDV, ShowTBP }; 
	TypeAction Action;
	void * ActionData;
	struct COutData * pOutData;
	CCompData * pCompData;
	C_ParamSelected() { pParam = NULL;
											pOutData = NULL;
	                    memset ( ParamName, 0, sizeof ( ParamName ));
	                    Action = NoAction;	ActionData = NULL; };
	};

struct C_TestSubModel
	{
	char * SubName;
	bool Res;
	PADDING(7);
	C_TestSubModel ( char * _SubName )
		{
		SubName = _SubName;
		Res = false;
		}
	};

struct CSetPPK
	{
	double P;
	double Flow_kg, Flow_m3;
	};

struct C_FlowInOut
	{
	char * ObjName;
	struct CFlow * pFlow[100];
	char TypeFlow[100];
	PADDING(4);
	C_FlowInOut( )
		{
		memset ( this, 0, sizeof ( C_FlowInOut ));
		}
	};

struct CSensMes
	{
	char Name[256];
  int Mes;
	CSensMes( )
		{
		memset ( Name, 0, 256 );
		Mes = -1;
		}
	};

struct CModelCmd
	{
	char * Cmd;
  double Parm;
	CModelCmd( )
		{
		Cmd = NULL;
		Parm = 0.;
		}
	};

struct CDrainInfo
	{
	struct CZadv * pZadv;
	double Dy;
	};

struct CSetParams
	{
	char ObjName[128];
	char ParamName[128];
	char Value[256];
  char Type;
	};

struct CModelParam
	{
  double * D;
  BYTE * B;
  char * NameObj;
  char * NameParm;
  bool Найден;
  PADDING(7);
  };

struct CModelPointRef
	{
	char ObjName[65];
	char RefName[65];
	void * pVar;
	CModelPointRef()
		{
		memset ( ObjName, 0, sizeof ( ObjName ));
		memset ( RefName, 0, sizeof ( RefName ));
		pVar = NULL;
		}
	};

struct CGetSensInfo
	{
	double * pData;
	COLORREF Col;
	char ParmName[65];
	char Fmt[65];
	char ModelName[129];
	double A;
	double B;
	double Min;
	double Max;
	CGetSensInfo()
		{
		memset ( this, 0, sizeof ( CGetSensInfo ));
		A = 1.;
		Min = -1e10;
		Max = 1e10;
		}
	};

#undef MP_D
#define MP_D(ObjName,ParmName,Var) \
	if ( TypeData == sd_GetModelParam	)\
    {\
    CModelParam * pMP = (CModelParam*)pData;\
    if ( lstrcmp ( ObjName, pMP->NameObj ) == 0 &&\
         lstrcmp ( ParmName, pMP->NameParm ) == 0 )\
      {\
      pMP->D = &Var;\
      pMP->Найден = true;\
      return sd_GetModelParam;\
      }\
    }

#undef MP_B
#define MP_B(ObjName,ParmName,Var) \
	if ( TypeData == sd_GetModelParam	)\
    {\
    CModelParam * pMP = (CModelParam*)pData;\
    if ( lstrcmp ( ObjName, pMP->NameObj ) == 0 &&\
         lstrcmp ( ParmName, pMP->NameParm ) == 0 )\
      {\
      pMP->B = &Var;\
      pMP->Найден = true;\
      return sd_GetModelParam;\
      }\
    }

struct CAVOMotor
  {
  bool On;
  double Oborot;
  double Angle;
  bool ErrBlk;
  PADDING(7);
  };

struct CColLevData
  {
  double * pLevCub;
  double * pHCub; 
  double * _pT_max;
  };

struct CSetGroupData
  {
  int FirstObj;
  int LastObj;
  CMem<struct CObjRef,256,256> * pObjRefs;
  };

struct CParamClicked
  {
//  sd_ParamClicked,
  void * ParamAddr;
  char * ParamName;
  enum eCommand { Нет, ВыделитьТрубу, ВыделитьОбъект, ОкноСоставов, ПараметрыОбъекта, Окно_KT };
  eCommand Command;
  char Object[64];
  BYTE Data[256];
  PADDING(4);
  CParamClicked()
    {
    ParamName = "";
    Command = Нет;
    CLEAR(Object)
    }
  };

struct CExtCompInfo
  {
  double * Cmol;
  bool * IsComp;
  double * P;
  double * T;
  int * pLAST_GAS;
  class CGas ** pGas;
  char * CompName;
//
  CExtCompInfo()
    {
    memset ( this, 0, sizeof(*this));
    }
  };

struct CExtFlowInfo
{
	char *szObjName;
	double Flow;
	CExtFlowInfo()
	{
		memset ( this, 0, sizeof(*this));
	}
};

struct CExtCompData
  {
  double * Cmol;
  bool * IsComp;
  char * CompName;
  bool SaveComp;
  PADDING(7);
  CExtCompData()
    {
    memset ( this, 0, sizeof(*this));
    }
  };

struct CEquipParam
  {
  char * Name;
  char * Value;
  char * Msg;
  };

struct CRegParams
  {
  char * Name;
  double K, T;
  char * Цель;
  double SP;
  double MaxШкалы,MinШкалы;
  char * Reg_1;
  char * Reg_2;
  char * Reg_3;
  char * Reg_4;
  char * Reg_5;
  double MaxРег, MinРег; 
  };

struct CGetCompCompareInfo
  {
  double * pSample;
  double * pEtalon;
  char ** Names;
  int kComp;
  PADDING(4);
  CGetCompCompareInfo()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  };

//  sd_FindPath,
struct CFindPathParams
  {
  const char * ObjFrom;
  const char * ObjTo;
  int nGroup;
  PADDING(4);
  };

//#define INIT_STATE

struct CGetObjData
  {
  char * Name;
  double * Addr;
  };

struct CCSVParamsInfo
  {
  bool Local;
  const char * File;
  };
