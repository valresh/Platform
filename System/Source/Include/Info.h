#pragma once

#include "err.h"
#include "BaseModel.h"
#include "Param.h"

#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define EXP Q_DECL_EXPORT
#else
#define EXP Q_DECL_IMPORT
#endif


#undef MODEL_INFO_2
#define MODEL_INFO_2(Class,_Flags,_Model,_Desc) \
  Class * pClass = (Class*)0;\
  extern "C" {\
  EXP IBaseModel * CreateObject(LPCTSTR ObjName, LPCTSTR Param )\
    { return new Class((char*)ObjName,(char*)Param); }\
  EXP int Project = 1;\
  EXP int Flags = _Flags;\
  EXP char * Model = _Model;\
  EXP char * Descr = _Desc; };

#undef MODEL_INFO
#define MODEL_INFO(Class,_Flags,_Model,_Desc) \
  Class * pClass = (Class*)0;\
  extern "C" {\
  EXP IBaseModel * CreateObject(LPCTSTR ObjName )\
    { return new Class((char*)ObjName); }\
    EXP int Project = 1;\
    EXP int Flags = _Flags;\
    EXP char * Model = _Model;\
    EXP char * Descr = _Desc; };

#undef MODEL_INFO_UZK
#define MODEL_INFO_UZK(Class,_Flags,_Model,_Desc) \
  Class * pClass = (Class*)0;\
  extern "C" {\
  EXP IBaseModel * CreateObject(LPCTSTR ObjName )\
    { return new Class((char*)ObjName); }\
    EXP int Project = 2;\
    EXP int Flags = _Flags;\
    EXP char * Model = _Model;\
    EXP char * Descr = _Desc; };

#undef DEF_D
#define DEF_D(Var,Comment,Val,Min,Max)
#undef DEFECT
#define DEFECT(Name,Comment)
#undef MODEL
#define MODEL(Name,Default)


struct CNodeInfo
  {
  char * Name;
  int Group;
  char Type;
  char Class;
  char SM;
  char * Desc;
  };



#pragma warning( disable : 4311 )

#define OFF(Var) (int)(int64_t)&reinterpret_cast<const volatile char&>((pClass->Var))

#undef TAB
#define TAB(TabName,Lev)

#undef DESC
#define DESC( Desc )

#undef PARM_D
#define PARM_D(Var,VarName,Default)

#undef PARM_I
#define PARM_I(Var,VarName,Default)

#undef PARM_B
#define PARM_B(Var,VarName,Default)

#undef PARM_S
#define PARM_S(Var,MaxL,VarName,Default)

#undef PARM_U
#define PARM_U(Var,VarName,Default)

#undef PARM_L
#define PARM_L(Var,VarName,Default)

#undef TAB_S
#define TAB_S(TabName,Lev) \
CParmInfo("TAB",TabName,'T',0,Lev,"", 1 ),

#undef PARM_D_S
#define PARM_D_S(Var,VarName,Default) \
CParmInfo(#Var,VarName,'D',8,OFF(Var),#Default, 1 ),

#undef PARM_A_S
#define PARM_A_S PARM_D_S

#undef PARM_I_S
#define PARM_I_S(Var,VarName,Default) \
CParmInfo(#Var,VarName,'I',4,OFF(Var),#Default, 1 ),

#undef PARM_B_S
#define PARM_B_S(Var,VarName,Default) \
CParmInfo(#Var,VarName,'B',1,0,#Default, 1 ),

#undef PARM_S_S
#define PARM_S_S(Var,MaxL,VarName,Default) \
CParmInfo(#Var,VarName,'S',MaxL,0,Default, 1 ),

#undef PARM_U_S
#define PARM_U_S(Var,VarName,Default) \
CParmInfo(#Var,VarName,'U',256,0,Default, 1 ),

#undef PARM_L_S
#define PARM_L_S(Var,VarName,Default) \
CParmInfo(#Var,VarName,'L',4,0,Default, 1 ),

struct CACSInfo
  {
  char * VarName;
  char * PntName;
  char * Type;
  DWORD Shift;
  char * Desc;
  };

#undef ACS_AI
#define ACS_AI(Var,PntName,PntDesc) \
#Var,PntName,"AI",(DWORD)OFF(Var),PntDesc,

#undef ACS_AI2
#define ACS_AI2(Var,PntName) \
#Var,PntName,"AI",OFF(Var),NULL,

#undef ACS_AO
#define ACS_AO(Var,PntName,PntDesc) \
#Var,PntName,"AO",(DWORD)OFF(Var),PntDesc,

#undef ACS_AO2
#define ACS_AO2(Var,PntName) \
#Var,PntName,"AO",OFF(Var),NULL,

#undef ACS_DI
#define ACS_DI(Var,PntName,PntDesc) \
#Var,PntName,"DI",OFF(Var),PntDesc,

#undef ACS_DI2
#define ACS_DI2(Var,PntName) \
#Var,PntName,"DI",OFF(Var),NULL,

#undef ACS_DO
#define ACS_DO(Var,PntName,PntDesc) \
#Var,PntName,"DO",(DWORD)OFF(Var),PntDesc,

#undef ACS_DO2
#define ACS_DO2(Var,PntName) \
#Var,PntName,"DO",OFF(Var),NULL,

#undef ACS_DIO
#define ACS_DIO(Var,PntName,PntDesc) \
#Var,PntName,"DIO",OFF(Var),PntDesc,

#undef ACS_DIO2
#define ACS_DIO2(Var,PntName,PntDesc) \
#Var,PntName,"DIO",OFF(Var),PntDesc,

#undef F_AI
#define F_AI(Var,PntName) \
#Var,PntName,"AI",OFF(Var),PntName,

#undef F_AO
#define F_AO(Var,PntName) \
#Var,PntName,"AO",OFF(Var),PntName,

#undef F_DI
#define F_DI(Var,PntName) \
#Var,PntName,"DI",OFF(Var),PntName,

#undef F_DO
#define F_DO(Var,PntName) \
#Var,PntName,"DO",OFF(Var),PntName,

#undef SET_COMP
#define SET_COMP(Var,Descr)

//#undef VAR_D
//#define VAR_D(Var,Descr) \
//#Var,#Var,"A",OFF(Var),Descr,
//
//#undef VAR_I
//#define VAR_I(Var,Descr) \
//#Var,#Var,"D",OFF(Var),Descr,
////////////////////////////////////////////////////////

#undef VAR_D
#define VAR_D(Var,Descr)

#undef VAR_I
#define VAR_I(Var,Descr)

#undef HS
#define HS(Var,Name)
#undef PHS
#define PHS(Var,Name)
#undef SENS
#define SENS(Var,Name)
#undef KLAP
#define KLAP(Var,Name)

#undef INT_REG
#define INT_REG(Var,Descr)

#undef NO_NODE_INFO
#define NO_NODE_INFO \
  extern "C" {\
EXP CNodeInfo * NodeInfo = NULL;\
EXP int kNodeInfo = 0; };

#undef BEGIN_NODE_INFO
#define BEGIN_NODE_INFO \
  extern "C" {\
  EXP CNodeInfo NodeInfoData[] = {

#undef END_NODE_INFO
#define END_NODE_INFO \
    };\
  EXP CNodeInfo * NodeInfo = NodeInfoData;\
  EXP int kNodeInfo = sizeof( NodeInfoData ) / sizeof ( CNodeInfo ); };

#undef NO_PARM_INFO
#define NO_PARM_INFO \
  extern "C" {\
  EXP CParmInfo * ParmInfo = NULL;\
  EXP int kParmInfo = 0; };

#undef BEGIN_PARM_INFO
#define BEGIN_PARM_INFO \
  extern "C" {\
  EXP CParmInfo ParmInfoData[] = {

#undef END_PARM_INFO
#define END_PARM_INFO \
    };\
  EXP CParmInfo * ParmInfo = ParmInfoData;\
  EXP int kParmInfo = sizeof( ParmInfoData ) / sizeof ( CParmInfo ); };

#undef NO_ACS_INFO
#define NO_ACS_INFO \
  extern "C" {\
  EXP CACSInfo * ACSInfo = NULL;\
  EXP int kACSInfo = 0; };

#undef BEGIN_ACS_INFO
#define BEGIN_ACS_INFO \
  extern "C" {\
  EXP CACSInfo ACSInfoData[] = {

#undef END_ACS_INFO
#define END_ACS_INFO \
    };\
  EXP CACSInfo * ACSInfo = ACSInfoData;\
  EXP int kACSInfo = sizeof( ACSInfoData ) / sizeof ( CACSInfo ); };


//typedef
//struct IBaseModel* (*tCreateObject)( LPCTSTR ObjName, LPCTSTR Type );

#define KEY_MODEL 0x3567AE2F
#define KEY_ACS   0xA13B7724
#define KEY_PARM  0x7615AF33
#define KEY_NODE  0x21A7E47B

struct CModelInformation
  {
  char * Model;
  char * SubModel;
  tCreateObject pCO;
  tGetProps pGetProps;
  int Flags;
  char * Descr;
  char ** TypeParam;
  struct CNodeInfo * pNodeInfo;
  int kNodeInfo;
  PADDING(4);
  void Get( CModelInformation * pMI )
    {
    Model = pMI->Model;
    pCO = pMI->pCO;
    Flags = pMI->Flags;
    Descr = pMI->Descr;
    pNodeInfo = pMI->pNodeInfo;
    kNodeInfo = pMI->kNodeInfo;
    }
  };

#define APIENTRY
#define BOOL int
#define HANDLE int
BOOL DllMain( HANDLE hModule,
              DWORD  ul_reason_for_call,
              LPVOID lpReserved	 );

//#define OLD_NODES
