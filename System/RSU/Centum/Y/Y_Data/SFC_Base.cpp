#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <_CALCU.h>
#include "../Y_TransSEBOL/SebolParser/SebolParser.h"
#include <macros/StrHelps.h>
#include <yokoheaders.h>



bool C_SFC_BASE_Impl::ReadUserDefinition( SUserDef *pUserDefinitions, int udCount, int memSize, char * ParmName, char * ParmValue, BYTE * pData )
{
  if( strncmp(ParmName, "UDIT", 4) )
    return false;

  SYokogawa200 *pHdrY = (SYokogawa200*)pData;
  SYokogawaID* pUDIT = FindYokoID( pHdrY, "UDIT" );
  if( !pUDIT )
    return false;

  int index = 0;
  USHORT curOffsetInUSER = 0;
  ASS( sizeof(SUDIT_SFC)==pUDIT->dwSize );
  for ( DWORD i=0; i<pUDIT->dwCount; i++ )
  {
    ASS( udCount > index );
    SUserDef &ud = pUserDefinitions[index];
    ud.szDataItem[0] = 0;
    SUDIT_SFC *p = (SUDIT_SFC*)pUDIT->Item( pData, i );
    if( !p->DataItem[0] )
      continue;
    ASS( p->CheckDataType( p->DataType ) );
    index++;
    ZeroMemory( &ud, sizeof(ud) );
    CopyMemory( ud.szDataItem, p->DataItem, sizeof(p->DataItem) );
    CopyMemory( ud.szComment, p->Comment, sizeof(p->Comment) );
    CopyMemory( ud.szEngUnit, p->EngUnitSymbol, sizeof(ud.szEngUnit) );
    ud.Array1 = p->Array1;
    ud.Array2 = p->Array2;
    ud.DataType = p->DataType;
    ud.elementSize = p->GetElementSize( p->DataType );
    ASS( ud.offsetInUSER < memSize );
    ud.offsetInUSER = curOffsetInUSER;
    if( ud.Array1 || ud.Array2 )
    {
      if( !ud.Array2 )
        ud.elementSize *= ud.Array1;
      else
        ud.elementSize *= (ud.Array1 * ud.Array2);
    }
    ASS( (ud.offsetInUSER+ud.elementSize) <= memSize );
    curOffsetInUSER += ud.elementSize;
  }

  return true;
}


namespace ns_SFC_Impl
{
  void SaveInState( IYokoStateSer* pStateSer, BYTE DataType, LPSTR pszName, char *pMem, USHORT elSize )
  {
    pStateSer->WriteStr( pszName );
    pStateSer->SimpleWrite( DataType );
    pStateSer->SimpleWrite( elSize );
    pStateSer->SimpleWrite( pMem, elSize );
  }

  void AddParam( KYBridge2SysParam &params, BYTE DataType, LPSTR pszName, char *pMem, USHORT elSize )
  {
    switch( DataType )
    {
    case SUDIT_SFC::edt1I32:
    case SUDIT_SFC::edt1I16:
      PARM( *(int*)pMem, pszName );
      break;
    case SUDIT_SFC::edt1F64:
    case SUDIT_SFC::edt1F32:
      PARM( *(double*)pMem, pszName );
      break;
    case SUDIT_SFC::edt1CHR:
      params.AddL( (LPSTR)pMem, pszName, elSize );
      break;
    default:
      ASS(0);
      return;
    }
  }
};

void C_SFC_BASE_Impl::GetParamsFromUserDefinition( char *pUSERmem, SUserDef *pUserDefinitions, int udCount, KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  int c = 0;
  for( int i=0; i<udCount; i++ )
  {
    if( pUserDefinitions[i].szDataItem[0] )
      c++;
  }
  if( !c )
    return;
  char szItemName[32] = { 0 };
  MENU("Data Item Definition");
  for( int i=0; i<udCount; i++ )
  {
    SUserDef &ud = pUserDefinitions[i];
    if( !ud.szDataItem[0] )
      continue;
    sprintf_s( szItemName, "#%s", ud.szDataItem);
    size_t left = strlen(szItemName);
    char *pszIndex = szItemName + left;
    left = _countof(szItemName) - left - 1;
    char *pData = pUSERmem + ud.offsetInUSER;
    const USHORT elSize = SUDIT_SFC::GetElementSize( ud.DataType );
    if( ud.Array1 || ud.Array2 )
    {
      for( USHORT x=1; x<=ud.Array1; x++ )
      {
        if( !ud.Array2 )
        {
          sprintf_s( pszIndex, left, "[%d]", x );
          ns_SFC_Impl::AddParam( params, ud.sizedType.type, szItemName, pData, elSize );
          pData += elSize;
        }
        else
        {
          for( USHORT y=1; y<=ud.Array2; y++ )
          {
            sprintf_s( pszIndex, left, "[%d][%d]", x, y );
            ns_SFC_Impl::AddParam( params, ud.sizedType.type, szItemName, pData, elSize );
            pData += elSize;
          }
        }
      }
    }
    else
    {
      ns_SFC_Impl::AddParam( params, ud.sizedType.type, szItemName, pData,elSize );
    }
    KKK();
  }
  E_MENU("Data Item Definition");
}

bool C_SFC_BASE_Impl::GetVarFromUserDefinition( char *pUSERmem, SUserDef *pUserDefinitions, int udCount, char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/  )
{
  LPCSTR pIndex = strchr( VarName, '[' );
  const size_t N = pIndex ? (pIndex - VarName) : 0;

  int Array1 = -1, Array2 = -1;
  if( pIndex )//еще не реализовал
  {
    pIndex++;
    Array1 = atoi( pIndex )-1;
    LPCSTR pT = strchr( pIndex, ',' );
    if( pT )
      pIndex = pT;
    else
    {
      pT = strstr( pIndex, "][" );
      if( pT )
        pIndex = pT+1;
      else
        pIndex = NULL;
    }
    if( pIndex )
    {
      pIndex++;
      Array2 = atoi( pIndex )-1;
    }
  }
  //ASS( !pIndex );

  for( int i=0; i<udCount; i++ )
  {
    SUserDef &ud = pUserDefinitions[i];
    if( !ud.szDataItem[0] )
      continue;
    if( N>0 )
    {
      if( '\0'!=ud.szDataItem[N] )
        continue;
      if( strncmp( VarName, ud.szDataItem, N) )
       continue;
    }
    else 
    {
      if( strcmp( VarName, ud.szDataItem) )
        continue;
    }
    char *pData = pUSERmem + ud.offsetInUSER;
    const USHORT elSize = SUDIT_SFC::GetElementSize( ud.DataType );

    if( ud.Array1 && ud.Array2 )
    {
      //ASS( Array1>=0 && Array2>=0 );
      if( Array1 < 0 )
        Array1 = 0;
      if( Array2 < 0 )
        Array2 = 0;
      USHORT offset = (ud.Array2*elSize)*Array1 + elSize*Array2;
      pData += offset;
    }
    else if( ud.Array1 )
    {
      ASS( Array2<=0 );
      if( Array1 < 0 )
        Array1 = 0;
      USHORT offset = Array1 * elSize;
      pData += offset;
    }

    if( Type )
      *Type = SUDIT_SFC::GetType( ud.DataType );
    if( pVar )
      *pVar = pData;
    if( pVarSize )
      *pVarSize = elSize;
    /*if( Work )
      *Work = true;*/
    return true;
  }
  return false;
}

bool C_SFC_BASE_Impl::ReadSfc( char * ParmName, char * ParmValue )
{
  if( strncmp(ParmName, "HISR", 4) )
    return false;

  if( !ParmValue )
    return false;

  char * Beg = strstr ( ParmValue, ":::SOURCE" );
  ASS(Beg==ParmValue);
  for( int i=0;i<1; i++ )
  {
    LPSTR pDA = strstr( Beg, "\r\n" );
    Beg = pDA + 2;
  }

  char * End = strstr ( Beg, "::::SOURCE" );
  TStringTerminator st2( End );

  /*KSebolParser parser;
  parser.ParseSebol( Beg );*/

  return true;
}

int C_SFC_BASE_Impl::UserStateSave( IYokoStateSer* pStateSer, char *pUSERmem, SUserDef *pUserDefinitions, int udCount )
{
  if( !pStateSer->SimpleWrite( eKeyLocalStruct ) ) 
    return -1;

  int c = 0;
  for( int i=0; i<udCount; i++ )
  {
    SUserDef &ud = pUserDefinitions[i];
    if( !ud.szDataItem[0] )
      continue;
    if( ud.Array1 || ud.Array2 )
    {
      if( !ud.Array2 )
        c += ud.Array1;
      else
        c += ( ud.Array1 * ud.Array2 );
    }
    else
      c++;
  }
  pStateSer->SimpleWrite( c );
  if( !c )
    return -2;
  char szItemName[32] = { 0 };

  for( int i=0; i<udCount; i++ )
  {
    SUserDef &ud = pUserDefinitions[i];
    if( !ud.szDataItem[0] )
      continue;
    strcpy_s( szItemName, ud.szDataItem);
    size_t left = strlen(szItemName);
    char *pszIndex = szItemName + left;
    left = _countof(szItemName) - left - 1;
    char *pData = pUSERmem + ud.offsetInUSER;
    const USHORT elSize = SUDIT_SFC::GetElementSize( ud.DataType );
    if( ud.Array1 || ud.Array2 )
    {
      for( USHORT x=1; x<=ud.Array1; x++ )
      {
        if( !ud.Array2 )
        {
          sprintf_s( pszIndex, left, "[%d]", x );
          ns_SFC_Impl::SaveInState( pStateSer, ud.sizedType.type, szItemName, pData, elSize );
          pData += elSize;
        }
        else
        {
          for( USHORT y=1; y<=ud.Array2; y++ )
          {
            sprintf_s( pszIndex, left, "[%d][%d]", x, y );
            ns_SFC_Impl::SaveInState( pStateSer, ud.sizedType.type, szItemName, pData, elSize );
            pData += elSize;
          }
        }
      }
    }
    else
    {
      ns_SFC_Impl::SaveInState( pStateSer, ud.sizedType.type, szItemName, pData, elSize );
    }
    KKK();
  }

  return 0;
}

int C_SFC_BASE_Impl::UserStateRestore( IYokoStateSer* pStateSer, char *pUSERmem, SUserDef *pUserDefinitions, int udCount )
{
  EStateKeys key = sKeyNull;
  pStateSer->SimpleRead( key );
  if( eKeyLocalStruct!=key )
  {
    pStateSer->Shift( -LONG(sizeof(key)) );
    return 0;
  }
  int c = 0;
  pStateSer->SimpleRead( c );

  char szItemName[32] = { 0 };
  BYTE DataType;
  USHORT elSize;
  for( int i=0; i<c; ++i )
  {
    Read( pStateSer, szItemName );
    Read( pStateSer, DataType );
    Read( pStateSer, elSize );
    void *pVar = NULL;
    char type = 0;
    USHORT varSize = 0;
    bool b = GetVarFromUserDefinition( pUSERmem, pUserDefinitions, udCount, szItemName, &pVar, NULL, &type, NULL, NULL, &varSize );
    if( !b )
    {
      pStateSer->Shift( elSize );
      continue;
    }
    ASS( elSize==varSize );
    if( elSize!=varSize )
    {
      pStateSer->Shift( elSize );
      continue;
    }
    bool bSkip = false;
    switch( type )
    {
    case 'S':
      switch( DataType )
      {
      case SUDIT_SFC::edt1CHR:
        break;
      default:
        bSkip = true;
        break;
      }
      break;
    case 'I':
      switch( DataType )
      {
      case SUDIT_SFC::edt1I32:
      case SUDIT_SFC::edt1I16:
        break;
      default:
        bSkip = true;
        break;
      }
      break;
    case 'D':
      switch( DataType )
      {
      case SUDIT_SFC::edt1F32:
      case SUDIT_SFC::edt1F64:
        break;
      default:
        bSkip = true;
        break;
      }
      break;
    default:
      bSkip = true;
      break;
    }
    if( bSkip )
    {
      ASSD( !bSkip );
      pStateSer->Shift( elSize );
      continue;
    }
    pStateSer->SimpleRead( pVar, varSize );
  }

  return 0;
}
