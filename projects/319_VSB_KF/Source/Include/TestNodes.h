#pragma once
#ifdef LINUX
#include "Interface/HydroBasicTypes.h"
#else
#include "Interface\HydroBasicTypes.h"
#endif
struct CObjVariablesData // Для авоматизации присвоения переменных
  {
  char SetVarName[32];
  ePipeType TypeVarStruct;
  int GroupInModel;
  int NumbInGroup;
  char IO;
  char Description[128];
  bool WasSet;
  };

#ifdef LINUX
#undef NODE_LIST
#define NODE_LIST \
   static CObjVariablesData VariablesData[] = {

#undef END_LIST
#define END_LIST \
  };\
  static int kVariablesData = sizeof ( VariablesData ) / sizeof ( CObjVariablesData );
#define STATIC static
#else
#undef NODE_LIST
#define NODE_LIST \
extern "C"\
__declspec(dllexport) CObjVariablesData VariablesData[] = {

#undef END_LIST
#define END_LIST \
    };\
  extern "C"\
__declspec(dllexport) int kVariablesData = sizeof ( VariablesData ) / sizeof ( CObjVariablesData );
#define STATIC extern "C" __declspec(dllexport)

#endif


#undef NODE_IN
#define NODE_IN(Group,Numb,Name,Desc) \
  { Name, eГидравлика, Group, Numb, '+', Desc },

#undef NODE_OUT
#define NODE_OUT(Group,Numb,Name,Desc) \
  { Name, eГидравлика, Group, Numb, '-', Desc },

#undef  MOTOR_IN
#define MOTOR_IN(Group,Name,Desc) \
  { Name, eMotor, Group, 0, '+', Desc },

#undef  MOTOR_OUT
#define MOTOR_OUT(Group,Name,Desc) \
  { Name, eMotor, Group, 0, '-', Desc },

#undef  SIGNAL_IN
#define SIGNAL_IN(Group,Name,Desc) \
  { Name, eSignal, Group, 0, '+', Desc },

#undef  SIGNAL_OUT
#define SIGNAL_OUT(Group,Name,Desc) \
  { Name, eSignal, Group, 0, '-', Desc },

#ifdef LINUX
#define EXP Q_DECL_EXPORT
#endif

#undef STD_TEST

#define STD_TEST \
static int StdTestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )\
  {\
  int kIO = 0;\
  int kErr = 0;\
  if ( kNodes > kVariablesData )\
    {\
    ModMsg ( "У '%s' число переменных на входе велико ( %d > %d )", ObjName, kNodes, kVariablesData );\
    return 1;\
    }\
  for ( int v = 0; v < kVariablesData; v++ )\
    VariablesData[v].WasSet = false;\
  for ( int n = 0; n < kNodes; n++ )\
    {\
    CObjectPoint * pObjVar = ppNodes[n];\
    char * Point = pObjVar->PntName;\
    bool OK = false;\
    for ( int v = 0; v < kVariablesData; v++ )\
      {\
      if ( lstrcmpi_m ( VariablesData[v].SetVarName, Point ) == 0 )\
        {\
        OK = true;\
        if ( VariablesData[v].WasSet )\
          {\
          ModMsg ( "У  '%s' переменная '%s' задается повторно", ObjName, Point ), kErr++;\
          }\
        else\
          {\
          VariablesData[v].WasSet = true;\
          pObjVar->Type = VariablesData[v].TypeVarStruct;\
          pObjVar->PntGroup = VariablesData[v].GroupInModel;\
          pObjVar->NumbInGroup = VariablesData[v].NumbInGroup;\
          pObjVar->IO = VariablesData[v].IO;\
          }\
          break;\
        }\
      }\
      if ( !OK )\
        ModMsg ( "У  '%s' ошибочный вход '%s'", ObjName, Point ), kErr++;\
    }\
  return kErr;\
  }


#define Point PntName
#define GrpPoint PntGroup
#define IntNumb NumbInGroup
#define TypeF IO
