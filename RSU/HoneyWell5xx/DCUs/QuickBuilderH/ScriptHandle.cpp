#include "ScriptHandle.h"
#include "QbTranslator.h"
#include <macros/StrHelps.h>
#include "QuickBuilder.h"
#include <rsuStr.h>

extern KQuickBuilder *g_pQuickBuilder;

KScriptHandle::KScriptHandle()
: m_pAddr( NULL )
, m_Type( evtHZ )
, m_varSize( 0 )
, m_bLinked( false )
, m_bScriptError( false )
{
}

void KScriptHandle::AfterStateRestored()
{
  if( !m_bLinked )
  {
    Links();
    m_bLinked = true;
  }
  if( !m_pAddr )
    return;
  memcpy( &dblPrev, m_pAddr, m_varSize );
}

void KScriptHandle::SetOnChangeEvent( LPCSTR pszName, BYTE *pAddr, eVarType Type, USHORT varSize )
{
  if( pszName )
    strcpy_s( m_szObjectID, pszName );
  if( pAddr )
  {
    m_pAddr = pAddr;
    m_Type = Type;
    m_varSize = varSize;
    ASSD( m_varSize<=sizeof(dblPrev) );
  }
}

LPCSTR KScriptHandle::GetObjectID()
{
  return m_szObjectID;
}

void KScriptHandle::Links()
{
  if( !m_szObjectID[0] )
    return;
  char szParts[4][24*4] = {};
  int C = SplitString( m_szObjectID, szParts, '.', false );
  ASSD( 2==C || 3==C );
  for( int j=0; j<C; ++j )
    _Ktoupper( szParts[j] );
  KQbBase* pFrom = g_pQuickBuilder->Find( szParts[0] );
  if( !pFrom )
    return;
  if( !strcmp(szParts[1],"STATE") )
    return;
  char bigName[48 * 4];
  if( 2==C )
    strcpy_s( bigName, szParts[1] );
  else if( 3==C )
    sprintf_s( bigName, "%s.%s", szParts[1], szParts[2] );
  szFieldOfObj = bigName;
  BYTE *pAddr = NULL;
  eVarType Type = evtHZ;
  USHORT varSize = 0;
  bool b = pFrom->GetVar( szFieldOfObj, &pAddr, &Type, &varSize );
  ASSD( b );
  if( evtString==Type )
    return;
  SetOnChangeEvent( NULL, pAddr, Type, varSize );
}

int KScriptHandle::SetScript( LPCSTR pszScr, bool textAsName /*= false*/ )
{
  KQbTranslator trans;
  trans.ParseText( pszScr, textAsName );
  tOPCont_ Ops;
  tVarCont_ Vars;
  trans.GetCodeOPs( Ops, Vars );

  m_nOps = (USHORT)Ops.size();
  if( m_nOps )
  {
    SOPElement *pOPs = &Ops.at(0);
    m_pOPs = (SOPElement*)KMemoryServiceQB5xx::Instance().NewMem( sizeof(*m_pOPs)*m_nOps );
    for( USHORT i=0; i<m_nOps; ++i )
      m_pOPs[i] = pOPs[i];
  }
  m_nVars = (USHORT)Vars.size();
  if( m_nVars )
  {
    KVar *pVars = &Vars.at(0);
    m_pVars = new KCalcVar[m_nVars];
    for( USHORT i=0; i<m_nVars; ++i )
    {
      *(KVar*)&m_pVars[i] = pVars[i];
      bool b = m_pVars[i].Init();
      ASSD(b);
    }
  }

  return int(Vars.size() + Ops.size());
}

void KScriptHandle::StepT( double dt )
{
  if( !m_pAddr )
    return;
/*#ifdef _DEBUG
  ExecSt();
#endif*/
  if( !memcmp(&dblPrev, m_pAddr, m_varSize ) )
    return;
  memcpy( &dblPrev, m_pAddr, m_varSize );
  ExecSt();
}

void KScriptHandle::Algo20StepT( double dt )
{
  ExecSt();
}

void KScriptHandle::Algo20Init()
{
  for( USHORT i=0; i<m_nVars; ++i )
  {
    if( KCalcVar::Ref2FieldVar != m_pVars[i].locType )
      continue;
    if( !m_pVars[i].NotLinked() )
      continue;
    char szParts[3][32*4] = {};
    int C = SplitString( m_pVars[i].szName, szParts, '.', false );
    ASSD( 2==C || 3==C );
    for( int j=0; j<C; ++j )
      _Ktoupper( szParts[j] );
    KQbBase* pFrom = g_pQuickBuilder->Find( szParts[0] );
    ASSD( pFrom );
    BYTE *pAddr = NULL;
    eVarType Type = evtHZ;
    USHORT varSize = 0;
	char bigName[48 * 4];
    if( 2==C )
      strcpy_s( bigName, szParts[1] );
    else if( 3==C )
      sprintf_s( bigName, "%s.%s", szParts[1], szParts[2] );
    bool b = pFrom->GetVar( bigName, &pAddr, &Type, &varSize );
    ASSD( b );
    m_pVars[i].InitTo( pAddr, Type, varSize );
  }
}

static KCalcVar s_Stack[50] = {};
static USHORT s_kStack = 0;


void push_back( KCalcVar &var )
{
  ASSD( s_kStack < _countof(s_Stack) );
  ZeroMemory( &s_Stack[s_kStack], sizeof(s_Stack[s_kStack]) );
  s_Stack[s_kStack] = var;
  s_Stack[s_kStack].m_bAddrVar = false;
  s_kStack++;
}

void push_back( KCalcVar *var )
{
  ASSD( s_kStack < _countof(s_Stack) );
  ZeroMemory( &s_Stack[s_kStack], sizeof(s_Stack[s_kStack]) );
  s_Stack[s_kStack] = *var;
  s_Stack[s_kStack].m_bAddrVar = true;
  s_kStack++;
}

KCalcVar pop_back()
{
  ASSD( s_kStack > 0 );
  KCalcVar C = s_Stack[--s_kStack];
  return C;
}

bool KScriptHandle::Function( eFunctions nF )
{
  switch( nF )
  {
  default:
    ASSD( 0 );
    break;
  case efPARAMVALUE:
    {
      KCalcVar v = pop_back();
      char szParts[3][24*4] = {};
      int C = SplitString( v, szParts, '.', false );
      ASSD( 2==C || 3==C );
      for( int j=0; j<C; ++j )
        _Ktoupper( szParts[j] );
      KQbBase* pFrom = g_pQuickBuilder->Find( szParts[0] );
      //ASSD( pFrom );
      if( !pFrom )
        return false;
      BYTE *pAddr = NULL;
      eVarType Type = evtHZ;
      USHORT varSize = 0;
	  char bigName[48 * 4];
      if( 2==C )
        strcpy_s( bigName, szParts[1] );
      else if( 3==C )
        sprintf_s( bigName, "%s.%s", szParts[1], szParts[2] );
      bool b = pFrom->GetVar( bigName, &pAddr, &Type, &varSize );
      ASSD( b );
      KCalcVar res;
      res.InitTo( pAddr, Type, varSize );
      res.szName = szParts[1];
      res.m_pObj = pFrom;
      push_back( res );
      KKK();
    }
    break;
  }
  return true;
}

void KScriptHandle::Operation( eToken_value cmd )
{
  switch( cmd )
  {
  default:
    ASSD( 0 );
    break;
  case etMINUS:
  case etPLUS:
  case etDIV:
  case etMUL:
    {
      KCalcVar vr = pop_back();
      KCalcVar vl = pop_back();
      KCalcVar res;
      vl.Action( res, vr, cmd );
      push_back( res );
    }
    break;
  case etASSIGN:
    {
      KCalcVar vr = pop_back();
      KCalcVar vl = pop_back();
      vl.AssignFrom( vr );
      KKK();
    }
    break;
  case etGE:
  case etLE:
  case etGT:
  case etLT:
  case etEQ:
  case etNE:
    {
      KCalcVar vr = pop_back();
      KCalcVar vl = pop_back();
      KCalcVar res;
      vl.Compare( res, vr, cmd );
      push_back( res );
    }
    break;
  }
}

void KScriptHandle::ExecSt()
{
  if( m_bScriptError )
    return;
  KCalcVar *Vars = m_pVars;
  SOPElement* OP = m_pOPs;
  USHORT nOP = 0;
  s_kStack = 0;
  while( nOP < m_nOps )
  {
    SOPElement CurrOP = (SOPElement)OP[nOP++];
    switch( CurrOP.OP )
    {
    default:
      ASSD( 0 );
      break;
    case OP_ELSE:
      {
        int OnEndif = OP[nOP++];// Переход на endif
        ASS(OnEndif>0 && OnEndif <= m_nOps);
        nOP = (int)OP[OnEndif];
      }
      break;
    case OP_IF:
      {
        int OnFalse = OP[nOP++];// Переход по false
        int OnEndif = OP[nOP++];// Переход на endif
        ASSD(OnEndif>0 && OnEndif <= m_nOps);
        KCalcVar V1 = pop_back();
        int Next = -1;
        bool R = V1.GetBool();

        if( !R )
          Next = OnFalse;

        if ( Next > 0 )
          nOP = Next;
      }
      break;
    case OP_Operation:
      Operation( OP[nOP++] );
      break;
    case OP_Function:
      {
        eFunctions nF = (eFunctions)(int)OP[nOP++];
        if( !Function( nF ) )
        {
          m_bScriptError = true;
          return;
        }
      }
      break;
    case OP_AddrVar:
      {
        int N = OP[nOP++];
        ASSD( N < m_nVars );
        KCalcVar Var = Vars[N];
        push_back( &Var );
      }
      break;
    case OP_LoadVar:
      {
        int N = OP[nOP++];
        ASSD( N < m_nVars );
        KCalcVar Var = Vars[N];
        push_back( Var );
      }
      break;
    }
  }
}
