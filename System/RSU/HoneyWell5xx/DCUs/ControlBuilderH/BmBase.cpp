#include <rsuErr.h>
#include <BmBase.h>
#include "ParmVarInfo.h"
#include <macros/StrHelps.h>

KBmBase::KBmBase()
: pObjAddr( NULL )
, pThisAddr( NULL )
{
  ClassVarInfo = NULL;
  kClassVarInfo = 0;
}

void KBmBase::CreateCrcs()
{
  nBlockCrcs = CreateCrcsImpl( BlockName, crcBlockSplitted );
}

static SVarInfo Key;

bool KBmBase::GetVar( LPCSTR pField, BYTE **ppVar, eVarType *pType, USHORT *pVarSize /*= NULL*/, LPCSTR *ppszEnum /*= NULL*/, BYTE *pFlags /*= NULL*/ )
{
  Key.VarName = pField;
  
  SVarInfo * pI = (SVarInfo*)bsearch( &Key, ClassVarInfo, kClassVarInfo, sizeof( SVarInfo ), CompVarInfo );
  if( !pI )
    return false;

  BYTE * pV = NULL;
  if( '^'==pField[0] )
  {
    if( !pThisAddr )
      return false;
    pV = pThisAddr + pI->sVar;
  }
  else 
  {
    if( !pObjAddr )
      return false;
    pV = pObjAddr + pI->sVar;
  }
  if( ppVar )
    *ppVar = pV;
  if( pType )
    *pType = pI->Type;
  if( pFlags )
    *pFlags = pI->flags;
  if( pVarSize )
  {
    ASS( pI->varSize <= USHRT_MAX );
    *pVarSize = pI->varSize;
  }

  if( (pI->flags&SVarInfo::efEnum) && ppszEnum )
    *ppszEnum = pI->pszEnumVals;

  return true;
}

int KBmBase::GetFirstVar( BYTE maskFlags )
{
  for( int i=0; i<kClassVarInfo; ++i )
  {
    SVarInfo &var = ClassVarInfo[i];
    if( var.flags&maskFlags )
      return i;
  }
  return -1;
}

LPCSTR KBmBase::GetVar( int &pos, LPBYTE &pVar, eVarType &pType, USHORT *pVarSize )
{
  if( pos < 0 || pos >= kClassVarInfo )
  {
    pos = -1;
    return NULL;
  }
  SVarInfo &var = ClassVarInfo[pos];
  bool b = KBmBase::GetVar( var.VarName, &pVar, &pType, pVarSize );
  ASS( b );
  BYTE maskFlags = var.flags&SVarInfo::efMaskType;
  
  int i=pos+1;
  pos = -1;
  for( i; i<kClassVarInfo; ++i )
  {
    SVarInfo &var = ClassVarInfo[i];
    if( var.flags&maskFlags )
    {
      pos = i;
      break;
    }
  }

  return var.VarName;
}

bool KBmBase::SetValue( LPCSTR pField, LPCSTR pszVal )
{
  BYTE *pVar = NULL;
  eVarType type = evtHZ;
  USHORT varSize = 0;
  LPCSTR pszEnum = NULL;

  int tmpForEnum = -100;

  bool r = GetVar( pField, &pVar, &type, &varSize, &pszEnum );
  if( !r )
  {
    OnNoSetValue( pField, pszVal );
    return false;
  }
  
  if( !::SetValue( pszVal, type, pVar, varSize, pszEnum ) )
  {
    OnNoSetValue( pField, pszVal );
    return false;
  }

  return true;
}

void KBmBase::OnNoSetValue( LPCSTR pField, LPCSTR pszVal )
{
}

#ifdef _WIN32
void KBmBase::GetFields( KHParamInfo *pPparms, int &nCur, const int nCount, blockVarType type )
{
  ASSD( pObjAddr );
  if( !pObjAddr )
    return;
  int i=0;
  for( i; i<kClassVarInfo && nCur<nCount; ++i, ++nCur )
  {
    KHParamInfo &inf = pPparms[nCur];
    SVarInfo &var = ClassVarInfo[i];
	  if ( (type == blockVar_Params && (var.flags & SVarInfo::efParam) == 0) ||
		  (type == blockVar_States && (var.flags & SVarInfo::efVar) == 0) )
		  nCur--;
	  else
    {
      if( '^'==var.VarName[0] )
        inf.Init( pThisAddr, var );
      else
        inf.Init( pObjAddr, var );
    }
  }
  ASS( i<=kClassVarInfo );
}
#endif // _WIN32

KBmBase* KBmBase::FindObj( LPCSTR pObj )
{
  if( !_strcmpi(BlockName, pObj) )
    return this;
  LPSTR pP = strchr(BlockName,'.');
  if( !pP )
    return NULL;
  {
    TStringTerminator st( pP );
    ++pP;
    if( !_strcmpi(BlockName, pObj) )
    {
      return this;
    }
  }

  pP = strrchr(BlockName,'.');
  if( !pP )
    return NULL;

  ++pP;
  if( !_strcmpi(pP, pObj) )
  {
    size_t l = strlen(pObj);
    if( l==strlen(pP))
      return this;
  }
  return NULL;
}

KBmBase* KBmBase::FindObj( typeHASH32_ *pCrcs, int nCrcs, int nAlgFind )
{
  if( nBlockCrcs==nCrcs )
  {
    bool bOk = true;
    for( int i=0; i<nBlockCrcs; ++i )
    {
      if( pCrcs[i]!=crcBlockSplitted[i] )
      {
        bOk = false;
        break;
      }
    }
    if( bOk )
      return this;
  }
  if( 0==nAlgFind && 1==nCrcs && nBlockCrcs > 1 )
  {
    if( *pCrcs==crcBlockSplitted[nBlockCrcs-1] )
      return this;
  }
  return NULL;
}

KBmBase* KBmBase::QuickFindObj( LPCSTR pszObj, int nAlgFind /*= 0*/ )
{
  typeHASH32_ crcSplits[KBmBase::countNames] = {};
  int cParts = CreateCrcsImpl( pszObj, crcSplits );
  KBmBase *pObj = FindObj( crcSplits, cParts, nAlgFind );
  return pObj;
}

void KBmBase::StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule )
{
}

void KBmBase::StepT( SStepCalcParams &dt )
{
}

KBmBase* KBmBase::WhoHasConnection( LPCSTR pszObj, LPCSTR pszFld, bool bOutput, LPCSTR *ppFld )
{
  return NULL;
}
