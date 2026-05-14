#pragma once
#pragma warning( disable: 4996)

#ifdef LINUX
#include "RsuX.h"
#else
#include "RsuX.h"
#endif
#include "ACS_Def.h"

// ВСе типы 'A' 'D'
struct IMM
  {
  int ID;
  void * pData;
  CStr ObjName;
  CStr Description;
  IBaseModel * pSrc;
  IBaseModel * pDst;
  IMM() { ID = 0; pData = NULL; pSrc = NULL; pDst = NULL; }; 
  virtual char GetType( ) { return ' '; };
  virtual bool GetValue( char Type, void * pValue ) { return false; };
  virtual bool SetValue( char Type, void * pValue ) { return false; };
  };
//////////////////////////////////////////////////////
struct CACSPnt : public IMM
  {
// Ссылка на ACS 
  enum { TypeID = 1 };
  CExtern_Pnt * pV;
//
  CACSPnt()
    {
    ID = TypeID;
    pV = NULL;
    }
  char GetType( ) 
    {
    if ( pV == NULL )
      return ' '; 
    return pV->Type[0];
    };
  bool GetValue( char Type, void * pValue ) 
    { 
    if ( pV == NULL )
      return false; 
    if ( pV->Type[0] == 'A' )
      {
      CAnalog * pA = (CAnalog*)pV;
      if ( Type == 'A' )
        {
        *(double*)pValue = pA->Value;
        return true;
        }
      if ( Type == 'D' )
        {
        *(int*)pValue = (int)pA->Value;
        return true;
        }
      if ( Type == 'B' )
        {
        if ( pA->Value )
          *(bool*)pValue = true;
        else
          *(bool*)pValue = false;
        return true;
        }
      return false;
      }
    if ( pV->Type[0] == 'D' )
      {
      CDiscrete * pD = (CDiscrete*)pV;
      if ( Type == 'A' )
        {
        *(double*)pValue = pD->Value;
        return true;
        }
      if ( Type == 'D' )
        {
        *(int*)pValue = pD->Value;
        return true;
        }
      if ( pD->Value )
        *(bool*)pValue = true;
      else
        *(bool*)pValue = false;
      return false;
      }
    return false;
    };
  bool SetValue( char Type, void * pValue ) 
    { 
    if ( pV == NULL )
      return false; 
    if ( pV->Type[0] == 'A' )
      {
      CAnalog * pA = (CAnalog*)pV;
      if ( Type == 'A' )
        {
        pA->Value = *(double*)pValue;
        return true;
        }
      if ( Type == 'D' )
        {
        pA->Value = *(int*)pValue;
        return true;
        }
      if ( Type == 'B' )
        {
        if ( *(bool*)pValue )
          pA->Value = 1.;
        else
          pA->Value = 0.;
        }
      return true;
      }
    if ( pV->Type[0] == 'D' )
      {
      CDiscrete * pD = (CDiscrete*)pV;
      if ( Type == 'A' )
        {
        pD->Value = (int)*(double*)pValue;
        return true;
        }
      if ( Type == 'D' )
        {
        pD->Value = *(int*)pValue;
        return true;
        }
      if ( Type == 'B' )
        {
        if ( *(bool*)pValue )
          pD->Value = 1;
        else
          pD->Value = 0;
        }
      return true;
      }
    return false; 
    };
  NEW
  };
//////////////////////////////////////////////////////
struct CRSUPnt : public IMM
  {
  enum { TypeID = 2 };
  char Type;
  KRsuX Rsu;
  CRSUPnt()
    {
    ID = TypeID;
    Type = ' ';
    }
  char GetType( ) 
    {
    return Type; 
    };
  bool GetValue( char ExtType, void * pValue ) 
    { 
    if ( Type == 'A' )
      {
      double A = Rsu;
      if ( ExtType == 'A' )
        {
        *(double*)pValue = A;
        return true;
        }
      if ( ExtType == 'D' )
        {
        *(int*)pValue = (int)A;
        return true;
        }
      if ( ExtType == 'B' )
        {
        if ( A )
          *(bool*)pValue = true;
        else
          *(bool*)pValue = false;
        }
      return false;
      }
    if ( Type == 'D' )
      {
      int D = Rsu;
      if ( ExtType == 'A' )
        {
        *(double*)pValue = D;
        return true;
        }
      if ( ExtType == 'D' )
        {
        *(int*)pValue = D;
        return true;
        }
      if ( ExtType == 'B' )
        {
        if ( D )
          *(bool*)pValue = true;
        else
          *(bool*)pValue = false;
        }
      return false;
      }
    return false;
    };
  bool SetValue( char ExtType, void * pValue ) 
    { 
    if ( ExtType == 'A' )
      {
      double A = *(double*)pValue;
      if ( Type == 'A' )
        {
        Rsu = A;
        return true;
        }
      if ( Type == 'D' )
        {
        Rsu = (int)A;
        return true;
        }
      return true;
      }
    if ( ExtType == 'D' )
      {
      int D = *(int*)pValue;
      if ( Type == 'A' )
        {
        Rsu = (double)D;
        return true;
        }
      if ( Type == 'D' )
        {
        Rsu = D;
        return true;
        }
      if ( ExtType == 'B' )
        {
        if ( *(bool*)pValue )
          Rsu = 1;
        else
          Rsu = 0;
        }
      return true;
      }
    return false; 
    };
  int LinkTo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pszFile )
    {
    int r = Rsu.LinkTo( pszPointName, pszIOtype, pszFile );
    Type = pszIOtype[0];
    return r;
    }
  NEW
  };
//////////////////////////////////////////////////////
struct CHSPnt : public IMM
  {
  enum { TypeID = 3 };
  char Type;
  CHs * pHS;
  int Defect;
  CHSPnt()
    {
    ID = TypeID;
    pHS = NULL;
    Defect = -1;
    }
  char GetType( ) 
    {
    return 'D'; 
    };
  bool GetValue( char ExtType, void * pValue ) 
    { 
    int D;
    if ( pHS->mOn )
      D = 1;
    else
      D = 0;
    if ( Defect >= 0 )
      D = Defect;
    if ( ExtType == 'A' )
      {
      *(double*)pValue = D;
      return true;
      }
    if ( ExtType == 'D' )
      {
      *(int*)pValue = D;
      return true;
      }
    if ( ExtType == 'B' )
      {
      *(bool*)pValue = pHS->mOn;
      return true;
      }
    return false;
    };
  bool SetValue( char ExtType, void * pValue ) 
    { 
    if ( ExtType == 'A' )
      {
      double A = *(double*)pValue;
      if ( A == 0. )
        pHS->mOn = false;
      else
        pHS->mOn = true;
      return true;
      }
    if ( ExtType == 'D' )
      {
      int D = *(int*)pValue;
      if ( D == 0 )
        pHS->mOn = false;
      else
        pHS->mOn = true;
      return true;
      }
    if ( ExtType == 'B' )
      {
      pHS->mOn = *(bool*)pValue;
      return true;
      }
    return false; 
    };
  int Create ( LPCSTR HsName )
    {
    pHS = NULL;
    GiveCommData ( HsName, CHs::TypeID, (void**)&pHS );
    if ( pHS )
      return 0;
    return 1;
    }
  NEW
  };

//////////////////////////////////////////////////////
#ifndef LINUX
  struct CPolyHSPnt : public IMM
  {
  enum { TypeID = 4 };
  char Type;
  CPolyHs * pHS;
//
  int Value_1;
  bool Inverse;
//
  CPolyHSPnt()
    {
    Value_1 = 0;
    Inverse = false;
    ID = TypeID;
    pHS = NULL;
    }
  char GetType( ) 
    {
    return 'D'; 
    };
//
bool GetValue( char ExtType, void * pValue ) 
  { 
  if ( ExtType != 'D' )
    return false;
  int D = pHS->SP;
  int Res;
  if ( D == Value_1 )
    Res = 1;
  else
    Res = 0;
  if ( Inverse )
    Res = 1 - Res;
  *(int*)pValue = Res;
  return true;
  };

bool SetValue( char ExtType, void * pValue ) 
  { 
  if ( ExtType != 'D' )
    return false;
  int D = *(int*)pValue;
  bool Set = D != 0;
  if ( Set )
    pHS->SP = Value_1;
  return true;
  };
  bool GetValue( char ExtType, void * pValue ) 
    { 
    int D = pHS->SP;
    if ( ExtType == 'A' )
      {
      *(double*)pValue = D;
      return true;
      }
    if ( ExtType == 'D' )
      {
      *(int*)pValue = D;
      return true;
      }
    if ( ExtType == 'B' )
      {
      if ( D )
        *(bool*)pValue = true;
      else
        *(bool*)pValue = false;
      return true;
      }
    return false;
    };
  bool SetValue( char ExtType, void * pValue ) 
    { 
    if ( ExtType == 'A' )
      {
      double A = *(double*)pValue;
      pHS->SP = (int)A;
      return true;
      }
    if ( ExtType == 'D' )
      {
      int D = *(int*)pValue;
      pHS->SP = D;
      return true;
      }
    if ( ExtType == 'B' )
      {
      if ( *(bool*)pValue )
        pHS->SP = 1;
      else
        pHS->SP = 0;
      return true;
      }
    return false; 
    };
    int Create ( LPCSTR HsName )
    {
        pHS = NULL;
        //GiveCommData ( HsName, CPolyHs::TypeID, (void**)&pHS );
        //if ( pHS )
        //  return 0;
        return 1;
    }
    NEW
  };

//////////////////////////////////////////////////////
struct CValuePnt : public IMM
  {
  // Ссылка на Value 
  enum { TypeID = 5 };
  CValue * pV;
  //
  CValuePnt()
    {
    ID = TypeID;
    pV = NULL;
    }
  char GetType( ) 
    {
    if ( pV == NULL )
      return ' '; 
    if ( pV->eType == enumValueDbl )
      return 'A'; 
    if ( pV->eType == enumValueInt )
      return 'D'; 
    if ( pV->eType == enumValueBol )
      return 'B'; 
    return ' ';
    };
  bool GetValue( char Type, void * pExtValue ) 
    { 
    if ( pV == NULL )
      return false; 
    if ( pV->eType == enumValueDbl )
      {
      if ( Type == 'A' )
        {
        *(double*)pExtValue = pV->dVal;
        return true;
        }
      if ( Type == 'D' )
        {
        *(int*)pExtValue = (int)pV->dVal;
        return true;
        }
      if ( Type == 'B' )
        { 
        if ( pV->dVal )
          *(bool*)pExtValue = true;
        else
          *(bool*)pExtValue = false;
        return true;
        }
      return false;
      }
    if ( pV->eType == enumValueInt )
      {
      if ( Type == 'A' )
        {
        *(double*)pExtValue = pV->nVal;
        return true;
        }
      if ( Type == 'D' )
        {
        *(int*)pExtValue = pV->nVal;
        return true;
        }
      if ( Type == 'B' )
        { 
        if ( pV->nVal )
          *(bool*)pExtValue = true;
        else
          *(bool*)pExtValue = false;
        return true;
        }
      return false;
      }
    if ( pV->eType == enumValueBol )
      {
      if ( Type == 'A' )
        {
        if ( pV->bVal )
          *(double*)pExtValue = 1.;
        else
          *(double*)pExtValue = 0.;
        return true;
        }
      if ( Type == 'D' )
        {
        if ( Type == 'A' )
          {
          if ( pV->bVal )
            *(int*)pExtValue = 1;
          else
            *(int*)pExtValue = 0;
          return true;
          }
        return true;
        }
      if ( Type == 'B' )
        { 
        *(bool*)pExtValue = pV->bVal;
        return true;
        }
      return false;
      }
    return false;
    };
  bool SetValue( char Type, void * pExtValue ) 
    { 
    if ( pV == NULL )
      return false; 
    if ( pV->eType == enumValueDbl )
      {
      if ( Type == 'A' )
        {
        pV->dVal = *(double*)pExtValue;
        return true;
        }
      if ( Type == 'D' )
        {
        pV->dVal = *(int*)pExtValue;
        return true;
        }
      if ( Type == 'B' )
        {
        if ( *(bool*)pExtValue )
          pV->dVal = 1.;
        else
          pV->dVal = 0.;
        return true;
        }
      return true;
      }
    if ( pV->eType == enumValueInt )
      {
      if ( Type == 'A' )
        {
        pV->nVal = (int)*(double*)pExtValue;
        return true;
        }
      if ( Type == 'D' )
        {
        pV->nVal = *(int*)pExtValue;
        return true;
        }
      if ( Type == 'B' )
        {
        if ( *(bool*)pExtValue )
          pV->nVal = 1;
        else
          pV->nVal = 0;
        return true;
        }
      return true;
      }
    if ( pV->eType == enumValueBol )
      {
      if ( Type == 'A' )
        {
        if ( *(double*)pExtValue )
          pV->bVal = true;
        else
          pV->bVal = false;
        return true;
        }
      if ( Type == 'D' )
        {
        if ( *(int*)pExtValue )
          pV->bVal = true;
        else
          pV->bVal = false;
        return true;
        }
      if ( Type == 'B' )
        {
        pV->bVal = *(bool*)pExtValue;
        return true;
        }
      return true;
      }
    return false; 
    };
  int Create ( LPCSTR ValueName, char Type )
    {
    pV = NULL;
//    GiveCommData ( ValueName, CValue::TypeID, (void**)&pV );
    if ( pV == NULL )
      return 1;
    if ( pV->eType == enumValueUnk )
      {
      if ( Type == 'A' )
        {
        pV->eType = enumValueDbl;
        return 0;
        }
      if ( Type == 'D' )
        {
        pV->eType = enumValueInt;
        return 0;
        }
      if ( Type == 'B' )
        {
        pV->eType = enumValueBol;
        return 0;
        }
      }
    else
      {
      if ( Type == 'A' && pV->eType == enumValueDbl )
        return 0;
      if ( Type == 'D' && pV->eType == enumValueInt )
        return 0;
      if ( Type == 'B' && pV->eType == enumValueBol )
        return 0;
      return 1;
      }
    return 1;
    }
  NEW
  };

//////////////////////////////////////////////////////
struct CSensorPnt : public IMM
  {
  // Ссылка на CSensor 
  enum { TypeID = 6 };
  CSensor * pV;
  //
  CSensorPnt()
    {
    ID = TypeID;
    pV = NULL;
    }
  char GetType( ) 
    {
    if ( pV == NULL )
      return ' '; 
    return 'A'; 
    };
  bool GetValue( char Type, void * pExtValue ) 
    { 
    if ( pV == NULL || Type != 'A' )
      return false; 
    *(double*)pExtValue = pV->ShowValue;
    return true;
    };
  bool SetValue( char Type, void * pExtValue ) 
    { 
    if ( pV == NULL || Type != 'A' )
      return false; 
    pV->SensValue = pV->PhysValue = pV->ShowValue = *(double*)pExtValue;
    return true;
    };
  int Create ( LPCSTR SensName )
    {
    pV = NULL;
//    GiveCommData ( SensName, CSensor::TypeID, (void**)&pV );
    if ( pV == NULL )
      return 1;
    return 0;
    }
  NEW
  };

struct CKlapanPnt : public IMM
  {
  // Ссылка на CKlapan 
  enum { TypeID = 7 };
  CKlapan * pV;
  bool WorkValueZ; 
  //
  CKlapanPnt()
    {
    ID = TypeID;
    pV = NULL;
    WorkValueZ = false;
    }
  char GetType( ) 
    {
    return 'A'; 
    };
  bool GetValue( char Type, void * pExtValue ) 
    { 
    if ( pV == NULL || Type != 'A' )
      return false; 
    if ( WorkValueZ )
      *(double*)pExtValue = pV->VentZ;
    else
      *(double*)pExtValue = pV->Vent;
    return true;
    };
  bool SetValue( char Type, void * pExtValue ) 
    { 
    if ( pV == NULL || Type != 'A' )
      return false; 
    if ( WorkValueZ )
      pV->VentZ = *(double*)pExtValue;
    else
      pV->Vent = *(double*)pExtValue;
    return true;
    };
  int Create ( LPCSTR KlapName, char * Point )
    {
    pV = NULL;
//    GiveCommData ( KlapName, CKlapan::TypeID, (void**)&pV );
    if ( pV == NULL )
      return 1;
    WorkValueZ = false;
    if ( strcmpi ( Point, "Задание" ) == 0 )
      WorkValueZ = true;
    return 0;
    }
  NEW
  };
#endif

  struct CStructPnt : public IMM
  {
  enum { TypeID = 8 };
  char Type;
  void * pV;
  DWORD Flag;
  bool Reset;
  CStructPnt()
    {
    Reset = false;
    Type = ' ';
    ID = TypeID;
    pV = NULL;
    Flag = 0;
    }
  void SetType( EValueType eType ) 
    {
    if ( eType == enumValueDbl )
      Type = 'A'; 
    if ( eType == enumValueInt )
      Type = 'D'; 
    if ( eType == enumValueBol )
      Type = 'B'; 
    };
  char GetType( ) 
    {
    return Type; 
    };
  bool GetValue( char ExtType, void * pExtValue ) 
    { 
    if ( pV == NULL )
      return false; 
    if ( Type == 'A' )
      {
      double V = *(double*)pV;
      if ( ExtType == 'A' )
        {
        *(double*)pExtValue = V;
        return true;
        }
      if ( ExtType == 'D' )
        {
        if ( V )
          *(int*)pExtValue = 1;
        else
          *(int*)pExtValue = 0;
        return true;
        }
      if ( ExtType == 'B' )
        { 
        if ( V )
          *(bool*)pExtValue = true;
        else
          *(bool*)pExtValue = false;
        return true;
        }
      return false;                              
      }
///////////////////////////////////////
    if ( Type == 'D' )
      {
      int V = *(int*)pV;
      if ( Flag )
        {
        if ( V & Flag )
          V = 1;
        else
          V = 0;
        }
      if ( ExtType == 'A' )
        {
        *(double*)pExtValue = V;
        return true;
        }
      if ( ExtType == 'D' )
        {
        *(int*)pExtValue = V;
        return true;
        }
      if ( ExtType == 'B' )
        { 
        if ( V )
          *(bool*)pExtValue = true;
        else
          *(bool*)pExtValue = false;
        return true;
        }
      return false;
      }
    if ( Type == 'B' )
      {
      bool V = *(bool*)pV;
      if ( Reset )
        *(bool*)pV = false;
      if ( ExtType == 'A' )
        {
        if ( V )
          *(double*)pExtValue = 1.;
        else
          *(double*)pExtValue = 0.;
        return true;
        }
      if ( ExtType == 'D' )
        {
        if ( V )
          *(int*)pExtValue = 1;
        else
          *(int*)pExtValue = 0;
        return true;
        }
      if ( ExtType == 'B' )
        { 
        *(int*)pExtValue = V;
        return true;
        }
      return false;
      }
    return false;
    };
///////////////////////
  bool SetValue( char ExtType, void * pExtValue ) 
    { 
    if ( pV == NULL )
      return false; 
    if ( Type == 'A' )
      {
      if ( ExtType == 'A' )
        {
        *(double*)pV = *(double*)pExtValue;
        return true;
        }
      if ( ExtType == 'D' )
        {
        *(double*)pV = *(int*)pExtValue;
        return true;
        }
      if ( ExtType == 'B' )
        {
        if ( *(bool*)pExtValue )
          *(double*)pV = 1.;
        else
          *(double*)pV = 0.;
        return true;
        }
      return true;
      }
    if ( Type == 'D' )
      {
      if ( Flag == 0 )
        {
        if ( ExtType == 'A' )
          {
          *(int*)pV = (int)*(double*)pExtValue;
          return true;
          }
        if ( ExtType == 'D' )
          {
          *(int*)pV = *(int*)pExtValue;
          return true;
          }
        if ( ExtType == 'B' )
          {
          if ( *(bool*)pExtValue )
            *(int*)pV = 1;
          else
            *(int*)pV = 0;
          return true;
          }
        return false;
        }
      else
        {
        if ( ExtType == 'A' )
          {
          if ( (int)*(double*)pExtValue )
            *(int*)pV |= Flag;
          else
            *(int*)pV &= ~Flag;
          return true;
          }
        if ( ExtType == 'D' )
          {
          if ( *(int*)pExtValue )
            *(int*)pV |= Flag;
          else
            *(int*)pV &= ~Flag;
          return true;
          }
        if ( ExtType == 'B' )
          {
          if ( *(bool*)pExtValue )
            *(int*)pV |= Flag;
          else
            *(int*)pV &= ~Flag;
          return true;
          }
        return false;
        }
      }
    if ( Type == 'B' )
      {
      if ( ExtType == 'A' )
        {
        if ( *(double*)pExtValue )
          *(bool*)pV = true;
        else
          *(bool*)pV = false;
        return true;
        }
      if ( ExtType == 'D' )
        {
        if ( *(int*)pExtValue )
          *(bool*)pV = true;
        else
          *(bool*)pV = false;
        return true;
        }
      if ( ExtType == 'B' )
        {
        *(bool*)pV = *(bool*)pExtValue;
        return true;
        }
      return true;
      }
    return false; 
    };
  int Create ( LPCSTR ValueName, int TypeID, char _Type )
    {
    pV = NULL;
//    GiveCommData ( ValueName, TypeID, (void**)&pV );
    if ( pV == NULL )
      return 1;
    Type = _Type;
    return 0;
    }
  NEW
  };


//////////////////////////////////////////////////////
struct CDoublePnt : public IMM
  {
  // Ссылка на CSensor 
  enum { TypeID = 9 };
  double * pV;
  //
  CDoublePnt()
    {
    ID = TypeID;
    pV = NULL;
    }
  char GetType( ) 
    {
    if ( pV == NULL )
      return ' '; 
    return 'A'; 
    };
  bool GetValue( char Type, void * pExtValue ) 
    { 
    if ( pV == NULL || Type != 'A' )
      return false; 
    *(double*)pExtValue = *pV;
    return true;
    };
  bool SetValue( char Type, void * pExtValue ) 
    { 
    if ( pV == NULL || Type != 'A' )
      return false; 
    *pV = *(double*)pExtValue;
    return true;
    };
  NEW
  };
