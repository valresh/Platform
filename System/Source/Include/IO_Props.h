#pragma once

#undef PARM
#define PARM(Name,Type,Addr) \
  {\
  CParams & __P = CParams::Get();\
  __P.SetAddr ( this, Name, Addr, Type, 0 );\
  }
  
struct CProps
  {
  char Type;
  char * Name;
  char * Label;
  char * Default;
  void * Addr;
  };

typedef 
int (*tGetProps)( CProps * pProps, IBaseModel * pObj );

#define BEGIN_PROPS(Class) \
  extern "C"\
  {\
  int GetProps( CProps Properties[], IBaseModel * pModel )\
  {\
  Class * pObj = (Class*)pModel;\
  int N = 0;

#undef PROP
#define PROP(_Type,_Name,_Label,_Default,Param) \
  ASS(N<64)\
  Properties[N].Type = _Type;\
  Properties[N].Name = _Name;\
  Properties[N].Label = _Label;\
  Properties[N].Default = _Default;\
  Properties[N].Addr = NULL;\
  if ( pObj ) Properties[N].Addr = &(pObj->Param);\
  N++;

#define END_PROPS return N; } }
