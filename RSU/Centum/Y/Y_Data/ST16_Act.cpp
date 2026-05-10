#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include "Y_Draw.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <macros/StrHelps.h>
#include "FlagsSupport.h"

void CAct_ST16::Act( bool Y, double dt )
{
  if( pPreActionApply )
    pPreActionApply->Work( 0 );
  if( pA )
    (this->*pA)( Y );
  else if( pAI )
    (this->*pAI)( Y, Iparam );
  else if( pAB )
    (this->*pAB)( Y, Iparam );
  else if( pAD )
    (this->*pAD)( Y, Dparam );
  else if( pAS )
    (this->*pAS)( Y, Sparam );
  else if( pASA )
    (this->*pASA)( Y, Sparam );
  if( pObj )
    pObj->OnAssigned( pVar );
}

void CAct_ST16::SET_0( bool Y )
{
  if ( !Y )
    return;
  Active = true;
  if( VarType == 'I' )
  {
    int &PV = *(int*)pVar;
    PV = 0;
  }
  else if( VarType == 'W' )
  {
    BYTE &PV = *(BYTE*)pVar;
    PV = 0;
  }
  else
  {
    ASSD(0);
  }
}

void CAct_ST16::SET_H( bool Y )
{
  int V;
  if ( Y )
    V = 1;
  else
    V = 0;
  Active = true;
  ASS( VarType == 'I' )
  int &PV = *(int*)pVar;
  PV = V;
}

void CAct_ST16::SetInt( bool Y, int thr )
{
  if ( !Y )
    return;
  Active = true;
  ASS( VarType == 'I' );
  int &PV = *(int*)pVar;
  PV = thr;
}

void CAct_ST16::SetMaskInt( bool Y, int thr )
{
  Active = true;
  ASS( VarType == 'I' );
  int &PV = *(int*)pVar;
  if( Y )
    PV |= thr;
  else
    PV &= ~thr;
}

void CAct_ST16::SetMaskIntInvert( bool Y, int thr )
{
  Active = true;
  ASS( VarType == 'I' );
  int &PV = *(int*)pVar;
  if( !Y )
    PV |= thr;
  else
    PV &= ~thr;
}

void CAct_ST16::SetIntYN( bool Y, int thr )
{
  Active = true;
  ASS( VarType == 'I' );
  int &PV = *(int*)pVar;
  if( Y )
    PV = thr;
  else
    PV = Ex.Iparam2;
}

void CAct_ST16::SetIntP_CSV( bool Y, int thr )//D3-73
{
  Active = true;
  ASS( VarType == 'I' );
  int &PV = *(int*)pVar;

  if( Y )
    PV = thr;
  else
  {
    switch( thr )
    {
    case 0:
      PV = 2;
      break;
    case 2:
      PV = 0;
      break;
    }
  }
}

void CAct_ST16::SetByte( bool Y, BYTE thr )
{
  if ( !Y )
    return;
  Active = true;
  ASS( VarType == 'W' );
  BYTE &M = *(BYTE*)pVar;
  M = thr;
}

void CAct_ST16::SetByte4ACT( bool Y, BYTE thr )
{
  if ( !Y )
    return;
  Active = true;
  ASS( VarType == 'W' );
  BYTE &M = *(BYTE*)pVar;
  M = thr;
  if( pObj )
    pObj->Work( 0 );
}

void CAct_ST16::SetByteYN( bool Y, BYTE thr )
{
  Active = true;
  ASS( VarType == 'W' );
  BYTE &PV = *(BYTE*)pVar;
  if( Y )
    PV = thr;
  else
    PV = Ex.Iparam2;
}

void CAct_ST16::SetDouble( bool Y, double thr )
{
  if ( !Y )
    return;
  Active = true;
  ASS( VarType == 'D' );
  double &M = *(double*)pVar;
  M = thr;
}

void CAct_ST16::SetString( bool Y, LPCSTR thr )
{
  if ( !Y )
    return;
  Active = true;
  ASS( VarType == 'S' );
  LPSTR psz = (LPSTR)pVar;
  strncpy_s( psz, varSize, thr, varSize );
}


void CAct_ST16::CallSA( bool Y, LPCSTR thr )
{
  if ( !Y )
    return;
  Active = true;
  if( pObj )
  {
    if( VarType == 'S' )
    {
      pObj->CallSA( Sparam );
      return;
    }
  }
  ASSD(0);
}

template<class T>
void AssignAction( CAct_ST16 * pVA, T valParam )
{
  switch( pVA->VarType )
  {
  case 'I':
    pVA->pAI = &CAct_ST16::SetInt;
    pVA->Iparam = valParam;
    break;
  case 'W':
    pVA->pAB = &CAct_ST16::SetByte;
    pVA->Iparam = valParam;
    break;
  case 'D':
    pVA->pAD = &CAct_ST16::SetDouble;
    pVA->Dparam = valParam;
    break;
  default:
    ASS(0);
    break;
  }
}

void CorrectST16ActFunc( char * Act, CAct_ST16 * pVA, CY_Base *pObj )
{
  if( !pObj )
    return;
  LPSTR pParam = strchr( Act, '.' );
  LPSTR pPointTest = strrchr( Act, '.');
  ASS( pParam );
  ASS( pParam==pPointTest );
  TStringTerminator pointRestore( *pParam++ );
  /*if( !_strcmpi( Act, "PV") && !_strcmpi( pParam, "NON") )
  {
    switch( pObj->Y_Type )
    {
    case id__RQ:
      KKK();
    case id__OG:
      ASS( &CAct_ST16::SetInt==pVA->pAI );
      break;
    }
  }*/
  /*if( id_ST16==pObj->Y_Type && !_strcmpi(Act, "ACT") )
  {
    if( &CAct_ST16::SetByte==pVA->pAB )
    {
      pVA->pAB = &CAct_ST16::SetByte4ACT;
    }
  }*/
  if( !_strcmpi( Act, "OP") )
  {
    switch( pObj->Y_Type )
    {
    case id_TM:
// Отдельная логика для VS2008 и выше, чтобы избежать ошибки компиляции C2051 (VM: 18.02.1018)
#if _MSC_VER >= 1500
		if ( pVA->Iparam == ((CY_TM*)pObj)->W->OP.START )
		{
			ASS( &CAct_ST16::SetByte==pVA->pAB );
			pVA->pAB = &CAct_ST16::SetByteYN;
			pVA->Ex.Iparam2 = ((CY_TM*)pObj)->W->OP.STOP;
		}
		else if ( pVA->Iparam == ((CY_TM*)pObj)->W->OP.WAIT )
		{
			ASS( &CAct_ST16::SetByte==pVA->pAB );
			pVA->pAB = &CAct_ST16::SetByteYN;
			pVA->Ex.Iparam2 = ((CY_TM*)pObj)->W->OP.RSTR;
		}
#else
      switch( pVA->Iparam )
      {
      case ((CY_TM*)pObj)->W->OP.START:
        ASS( &CAct_ST16::SetByte==pVA->pAB );
        pVA->pAB = &CAct_ST16::SetByteYN;
        pVA->Ex.Iparam2 = ((CY_TM*)pObj)->W->OP.STOP;
        break;
      case ((CY_TM*)pObj)->W->OP.WAIT:
        ASS( &CAct_ST16::SetByte==pVA->pAB );
        pVA->pAB = &CAct_ST16::SetByteYN;
        pVA->Ex.Iparam2 = ((CY_TM*)pObj)->W->OP.RSTR;
        break;
      }
#endif // _MSC_VER >= 1500
      break;
    }
  }
  if( !_strcmpi( Act, "CSV") && 'P'==*pParam )
  {
    switch( pObj->Y_Type )
    {
    case id_SI_2:
    case id_SIO_21:
    case id_SIO_22:
    case id_SIO_22P:
    case id_SI_2E:
    case id_SIO_21E:
    case id_SIO_22E:
    case id_SIO_22PE:
    case id_MC_2:
    case id_MC_2E:
    case id_MC_3:
    case id_MC_3E:
      {
        ASS( &CAct_ST16::SetInt==pVA->pAI );
        pVA->pAI = &CAct_ST16::SetIntP_CSV;
      }
    break;
    }
  }
}

void __declspec(dllexport) GetST16ActFunc( char * Act, CAct_ST16 * pVA, CY_Base *pObj )
{
  LPSTR pParam = strchr( Act, '.' );
  LPSTR pPointTest = strrchr( Act, '.');
  ASS( pParam );
  ASS( pParam==pPointTest );

  if( !pVA->pVar )
  {
    TStringTerminator pointRestore( *pParam );
    void * pTuneVar = NULL; 
    CY_Base::eVarType Work = CY_Base::evtP;
    BYTE *pFlagUse = NULL;
    bool b = pObj->GetVar( Act, &pVA->pVar, &pTuneVar, &pVA->VarType, &Work, &pFlagUse, &pVA->varSize );
    ASSD( b );
    if( b && pFlagUse )
      pVA->pFlagUse = pFlagUse;
  }

  TStringTerminator pointRestore( *pParam++ );

  if( !_strcmpi( Act, "MODE") )
  {
    pVA->pPreActionApply = pObj;
    AssignAction( pVA, GetModeNumber(pParam, pObj) );
  }
  else if( !_strcmpi( Act, "ALRM") )
    AssignAction( pVA, GetAlrmMask(pParam) );
  else if( !_strcmpi( Act, "AF") )
  {
    AssignAction( pVA, GetAlrmYokoCode(pParam) );
    pVA->pAI = &CAct_ST16::SetMaskInt;
  }
  else if( !_strcmpi( Act, "XAF") )
    AssignAction( pVA, GetAlrmNumber(pParam) );
  else if( !_strcmpi( Act, "AOFS") )
  {
    AssignAction( pVA, GetAlrmNumber(pParam) );
    pVA->pAB = &CAct_ST16::SetByteYN;
  }
  else if( !_strcmpi( Act, "SA") )
  {
    ASS( id_ST16==pObj->Y_Type || id_ST16E==pObj->Y_Type );
    pVA->pASA = &CAct_ST16::CallSA;
    pVA->Sparam = Act;
    pVA->pObj = pObj;
    pVA->VarType = 'S';
  }
  else if( !_strcmpi( Act, "PV") && (id_ST16==pObj->Y_Type || id_ST16E==pObj->Y_Type) )
  {
    pVA->pASA = &CAct_ST16::CallSA;
    pVA->Sparam = Act;
    pVA->pObj = pObj;
    pVA->VarType = 'S';
  }
  else
  {
    bool bShiftedRight = false;
    if( 'P'==*pParam )
    {
      pParam++;
      bShiftedRight = true;
    }
    BOOL bDigitsOnly = TRUE;
    const size_t L = strlen( pParam );
    for( size_t i=0; i<L; i++)
    {
      if( !isdigit(pParam[i]) && '-'!=pParam[i] )
      {
        bDigitsOnly = FALSE;
        break;
      }
    }
    if( !bDigitsOnly && bShiftedRight )
      --pParam;

    if( bDigitsOnly )
    {
      if( 'S'==pVA->VarType )
      {
        pVA->pAS = &CAct_ST16::SetString;
        pVA->Sparam = pParam;
      }
      else
        AssignAction( pVA, atoi(pParam) );
    }
    else if( 1==L )
    {
      switch( pParam[0] )
      {
      case 'L':
        AssignAction( pVA, 1 );
        break;
      case 'H':
        pVA->pA = &CAct_ST16::SET_H;
        pVA->dbgH_Type = true;
        break;
      case 'E':
        break;
      default:
        ASS( 0 );
      }
    }
    else if( !_strcmpi(pParam,"ON") )
    {
      AssignAction( pVA, 1 );
      pVA->pObj = pObj;
    }
    else if( !_strcmpi(pParam,"OFF") )
    {
      AssignAction( pVA, 0 );
      pVA->pObj = pObj;
    }
#pragma message("!!!!!!!!!!!!!!!!! Уточнить действия\n")
    else if( !_strcmpi(pParam,"NON") )
      AssignAction( pVA, INT_MAX );
    else
    {
      int I = GetEnumValue( Act, pParam, pObj );
      if( -1 != I)
      {
        AssignAction( pVA, I );
        pVA->pObj = pObj;
      }
      else
      {
        if( 'S'==pVA->VarType )
        {
          pVA->pAS = &CAct_ST16::SetString;
          pVA->Sparam = pParam;
        }
        else if( ' '==pVA->VarType )
        {
        }
        else
        {
          ASSD( 0 );
        }
      }
    }

  }

}
