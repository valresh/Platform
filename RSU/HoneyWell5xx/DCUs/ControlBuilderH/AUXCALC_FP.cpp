#include <rsuErr.h>
#include "H_Class.h"
#include "../ScriptoH/Trans.h"
#include <SR_DCU.h>
#include <crosslock.h>
#include <limits>
#include <math.h>

using namespace std;
using namespace ns_translator;

double trunc(double d){ return (d>0) ? floor(d) : ceil(d) ; }


static KCalcRes Stack[100];
static int kStack = 0;

void push_back( KCalcVar &var )
{
  ASS( kStack < _countof(Stack) );
  Stack[kStack++] = var;
}

void push_back( KCalcVar *var )
{
  ASS( kStack < _countof(Stack) );
  Stack[kStack] = *var;
  Stack[kStack].m_bAddrVar = TRUE;
  Stack[kStack].m_pTypeVar = &var->m_TypeVar;
  kStack++;
}

KCalcRes pop_back()
{
  ASS( kStack >= 1 );
  KMethodOnDestroy<KCalcRes> C(Stack[--kStack], &KCalcRes::Reset );
  return C;
}

static std::recursive_mutex s_CS;
static bool bInitedCS = false;

SAuxCalcCommon::SAuxCalcCommon()
{
  bInitedCS = true;
  bErrorInitialization = false;
}

bool SAuxCalcCommon::Calc( SExprCode &expr, double &r )
{
  LOCK( s_CS );

  KCalcVar *Var = expr.pVars;
  SOPElement* OP = expr.pOPs;
  int nOP = 0;
  kStack = 0;
  while( nOP < expr.cOPs )
  {
    SOPElement CurrOP = (SOPElement)OP[nOP++];
    switch( CurrOP.OP )
    {
    default:
      ASS( 0 );
      break;
    case OP_LoadVarIndex:
      {
        KCalcRes iv = pop_back();
        int index = iv;

        int N = OP[nOP++];
        ASS( N < expr.cVars );
        push_back( &Var[N] );
        KCalcRes V = pop_back();
        V.shift2index( index );
        push_back( V );
      }
      break;
    case OP_GOTO_OTHER_ELSE:
      {
        int OnEndif = OP[nOP++];// Переход на endif
        ASS(OnEndif>0 && OnEndif <= expr.cOPs);
        nOP = OP[OnEndif];
      }
      break;
    case OP_NOP:
      break;
    case OP_AddrVar:
      {
        int N = OP[nOP++];
        ASS( N < expr.cVars );
        push_back( &Var[N] );
      }
      break;
    case OP_LoadVar:
      {
        int N = OP[nOP++];
        ASS( N < expr.cVars );
        push_back( Var[N] );
      }
      break;
    case OP_Operation:
      {
        eYToken_value CMD = OP[nOP++];
        Operation( CMD );
      }
      break;
    case OP_Function:
      {
        eYFunctions nF = (eYFunctions)(int)OP[nOP++];
        Function( nF );
      }
      break;
    case OP_IF:
      {
        int OnFalse = OP[nOP++];// Переход по false
        ASS(OnFalse>0 && OnFalse <= expr.cOPs); 
        int OnEndif = OP[nOP++];// Переход на endif
        ASS(OnEndif>0 && OnEndif <= expr.cOPs);
        KCalcRes V1 = pop_back();
        int Next = -1;
        int R = V1;

        if( !R )
          Next = OnFalse;

        if ( Next > 0 )
          nOP = Next;
      }
      break;
    }
  }
  bool res = false;
  if( kStack>0 )
  {
    KCalcRes V = pop_back();
    r = V;
    res = true;
  }
  while( kStack )
  {
    pop_back();
  }
  return res;
}

void SAuxCalcCommon::Function( ns_translator::eYFunctions nF )
{
  switch( nF )
  {
  default:
    ASS( 0 );
    break;
  case eyfAVG:
    {
      KCalcRes C = pop_back();
      ASSD( C.IsInteger() );
      KCalcRes r;
      double avg = 0;
      int c = C;
      for( int i=0; i<c; ++i )
      {
        avg += (double)pop_back();
      }
      r = avg / c;
      push_back( r );
    }
    break;
  case eyfSQRT:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      r = sqrt( (double)V1 );
      push_back( r );
    }
    break;
  case eyfINT:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      if( V1.IsInteger() )
        r = (int)V1;
      else
        r = (int)(double)V1;
      push_back( r );
    }
    break;
  case eyfABS:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      if( V1.IsInteger() )
        r = abs( (int)V1 );
      else
        r = fabs( (double)V1 );
      push_back( r );
    }
    break;
  case eyfLOG:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      r = log10((double)V1 );
      push_back( r );
    }
    break;
  case eyfLN:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      r = log((double)V1 );
      push_back( r );
    }
    break;
  case eyfMAX:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      if( (double)V1 >= (double)V2 )
        push_back( V1 );
      else
        push_back( V2 );
    }
    break;
  case eyfMIN:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      if( (double)V1 < (double)V2 )
        push_back( V1 );
      else
        push_back( V2 );
    }
    break;
  case eyfSIN:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      r = sin( (double)V1 );
      push_back( r );
    }
    break;
  case eyfATN:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      r = atan( (double)V1 );
      push_back( r );
    }
    break;
  case eyfEXP:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      r = exp( (double)V1 );
      push_back( r );
    }
    break;
  case eyfISNAN:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      if( IsNaN((double)V1) )
        r = true;
      else
        r = false;
      push_back( r );
    }
    break;
  case  eyfNOW:
    {
      KCalcRes r;
      //r = (int)(pSys->dModelT * 3600);
      push_back( r );
    }
    break;
  case eyfTRUNC:
    {
      KCalcRes V1 = pop_back();
      V1 = trunc( (double)V1);
      push_back( V1 );
    }
    break;
  case eyfLOGDATACHANGEEVENT:
    for( int i=0; i<4; i++ )
    {
      KCalcRes V = pop_back();
      KKK();
    }
    break;
  case eyfISFIN:
    {
      KCalcRes V1 = pop_back();
      KCalcRes r;
      if( finite((double)V1) )
        r = true;
      else
        r = false;
      push_back( r );
    }
    break;
  }
}

void SAuxCalcCommon::Operation( ns_translator::eYToken_value cmd )
{
  switch( cmd )
  {
  default:
    ASS( 0 );
    break;
  case eytNEGATIVE:
    {
      KCalcRes V1 = pop_back();
      KCalcRes res;
      if( V1.IsInteger() )
        res = -(int)V1;
      else
        res = -(double)V1;
      push_back( res );
    }
    break;
  case eytPOW:
    {
      KCalcRes y = pop_back();
      KCalcRes x = pop_back();
      KCalcRes r;
      if( y.IsInteger() )
        r = (double)::pow( (double)x, (int)y );
      else
        r = (double)::pow( (double)x, (double)y );
      push_back( r );
    }
    break;
  case eytBITOR:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = (int)V2 | (int)V1;
      push_back( res );
    }
    break;
  case eytNOT:
    {
      KCalcRes V = pop_back();
      KCalcRes res;
      res = ((int)V) ? 0 : 1;
      push_back( res );
    }
    break;
  case eytBITAND:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = (int)V2 & (int)V1;
      push_back( res );
    }
    break;
  case eytSHIFT_RIGHT:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = ((int)V2) >> ((int)V1);
      push_back( res );
    }
    break;
  case eytOR:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = ((int)V2 || (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytAND:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      res = ((int)V2 && (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytNE:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
        res = ((double)V2 != (double)V1) ? 1 : 0;
      else
        res = ((int)V2 != (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytLIT:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
        res = ((double)V2 < (double)V1) ? 1 : 0;
      else
        res = ((int)V2 < (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytBIG:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
        res = ((double)V2 > (double)V1) ? 1 : 0;
      else
        res = ((int)V2 > (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytEQUAL:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
        res = ((double)V2 == (double)V1) ? 1 : 0;
      else
        res = ((int)V2 == (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytLE:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
        res = ((double)V2 <= (double)V1) ? 1 : 0;
      else
        res = ((int)V2 <= (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytGE:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
        res = ((double)V2 >= (double)V1) ? 1 : 0;
      else
        res = ((int)V2 >= (int)V1) ? 1 : 0;
      push_back( res );
    }
    break;
  case eytPLUS:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        double d1 = V1;
        res = (double)V2 + d1;
      }
      else
      {
        int d1 = V1;
        res = (int)V2 + d1;
      }
      push_back( res );
    }
    break;
  case eytMUL:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        double d1 = V1;
        res = (double)V2 * d1;
      }
      else
      {
        int d1 = V1;
        res = (int)V2 * d1;
      }
      push_back( res );
    }
    break;
  case eytMINUS:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        double d1 = V1;
        res = (double)V2 - d1;
      }
      else
      {
        int d1 = V1;
        res = (int)V2 - d1;
      }
      push_back( res );
    }
    break;
  case eytDIV:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      if( V1.IsReal() || V2.IsReal() )
      {
        double d1 = V1;
        double r = std::numeric_limits<double>::max();
        if( d1!=0 )
          r = (double)V2 / d1;
        res = r;
      }
      else
      {
        int d1 = V1;
        int r = INT_MAX;
        if( d1!=0 )
          r = (int)V2 / d1;
        res = r;
      }
      push_back( res );
    }
    break;
  case eytPERCENT:
  case eytMOD:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      KCalcRes res;
      int d1 = V1;
      int r = INT_MAX;
      if( d1!=0 )
        r = (int)V2 % d1;
      res = r;
      push_back( res );
    }
    break;
  case eytASSIGN:
    {
      KCalcRes Vr = pop_back();
      KCalcRes Vl = pop_back();
      if( Vr.IsReal() )
        Vl = (double)Vr;
      else if( Vr.IsInteger() )
        Vl = (int)Vr;
      else if( evtString==Vr.m_TypeVar && evtString==Vl.m_TypeVar )
        Vl.AssignMeString( Vr );
      else
      {
        ASSD(0);
      }
      KKK();
    }
    break;
  }
}

void SAuxCalcCommon::CreateCodes( KBmBase *pRoot, KBmBase *pModule, SExprCode *pCodes, LPSTR pExprs[], int N )
{
  for( int n=0; n<N; ++n )
  {
    if( !pExprs[n][0] )
      continue;
    _Ktoupper(pExprs[n]);
    KAaTranslator trans( pModule );
    stringstream ss;
    ss << pExprs[n];
    trans.Translate( ss, true );
    trans.GetResult( pCodes[n].cVars, pCodes[n].cOPs);
    if( !pCodes[n].cVars && !pCodes[n].cOPs )
      return;
    vector<KVar> vVars;
    if( pCodes[n].cVars )
      vVars.resize( pCodes[n].cVars );
    if( pCodes[n].cOPs )
      pCodes[n].pOPs = new (KMemoryServiceH5xx::Instance().NewMem( sizeof(SOPElement) * pCodes[n].cOPs )) SOPElement[pCodes[n].cOPs];
    trans.GetResult( pCodes[n].cVars ? &vVars[0] : NULL, pCodes[n].cOPs ? (SOPElement*)pCodes[n].pOPs : NULL );
    pCodes[n].pVars = new (KMemoryServiceH5xx::Instance().NewMem( sizeof(KCalcVar) * pCodes[n].cVars )) KCalcVar[pCodes[n].cVars];
    for( int i=0; i<pCodes[n].cVars; i++ )
    {
      KVar& var = vVars[i];
      KCalcVar &cv = ((KCalcVar*)pCodes[n].pVars)[i];
      strcpy_s( cv.m_NameValue, var.szName );
      switch( var.Type )
      {
      case KVar::Const:
        cv.m_Type = ns_CALCU::Const;
        cv.Numb();
        break;
      case KVar::Reference:
        cv.m_Type = ns_CALCU::ReferenceToField;
        break;
      case KVar::Double:
        cv.m_Type = ns_CALCU::Var;
        cv.m_TypeVar = evtDouble;
        break;
      case KVar::Integer:
        cv.m_Type = ns_CALCU::Var;
        cv.m_TypeVar = evtInt;
        break;
      case KVar::UnknownTypeReference:
        ASS(0);
        cv.m_Type = ns_CALCU::Var;
        cv.m_TypeVar = evtHZ;
        break;
      default:
        ASS( 0 );
      }
    }
    pCodes[n].cLocalVars = 0;
    for( int i=0; i<pCodes[n].cVars; i++ )
    {
      KCalcVar &cv = ((KCalcVar*)pCodes[n].pVars)[i];
      if( ns_CALCU::Var==cv.m_Type)
        pCodes[n].cLocalVars++;
    }
    if( pCodes[n].cLocalVars )
    {
      pCodes[n].pLocalVars = new (KMemoryServiceH5xx::Instance().NewMem( sizeof(SLocalVar) * pCodes[n].cLocalVars )) SLocalVar[pCodes[n].cLocalVars];

      pCodes[n].cLocalVars = 0;
      for( int i=0; i<pCodes[n].cVars; i++ )
      {
        KCalcVar &cv = ((KCalcVar*)pCodes[n].pVars)[i];
        if( ns_CALCU::Var==cv.m_Type)
        {
          SLocalVar &lv = ((SLocalVar*)pCodes[n].pLocalVars)[pCodes[n].cLocalVars++];
          strcpy_s( lv.NameVar, cv.m_NameValue );
          lv.m_TypeVar = cv.m_TypeVar;
          lv.dValue = cv.dValue;
          lv.nInVars = i;
        }
      }
    }
  }
}

void SAuxCalcCommon::SaveBlock(std::fstream& hFile, SExprCode *pCodes, LPSTR pExprs[], int N )
{
  DWORD Lw;
  for( int n=0; n<N; ++n )
  {
    if( !pExprs[n][0] )
      continue;
    WRT_VAR( pCodes[n].cVars );
    WRT_VAR( pCodes[n].cOPs );
    WRT_VAR( pCodes[n].cLocalVars );
    if( pCodes[n].cVars )
    {
      DWORD size = sizeof(*pCodes[n].pVars) * pCodes[n].cVars;
      WRT_VAR( size );
      WRT_DATA( size, pCodes[n].pVars );
    }
    if( pCodes[n].cOPs )
    {
      DWORD size = sizeof(*pCodes[n].pOPs) * pCodes[n].cOPs;
      WRT_VAR( size );
      WRT_DATA( size, pCodes[n].pOPs );
    }
    if( pCodes[n].cLocalVars )
    {
      DWORD size = sizeof(*pCodes[n].pLocalVars) * pCodes[n].cLocalVars;
      WRT_VAR( size);
      WRT_DATA( size, pCodes[n].pLocalVars );
    }
  }
}

bool SAuxCalcCommon::RestoreBlock(std::fstream& hFile, SExprCode *pCodes, LPSTR pExprs[], int N )
{
  DWORD Lr;
  for( int n=0; n<N; ++n )
  {
    if( !pExprs[n][0] )
      continue;
    READ_VAR( pCodes[n].cVars );
    READ_VAR( pCodes[n].cOPs );
    READ_VAR( pCodes[n].cLocalVars );
    if( pCodes[n].cVars )
    {
      DWORD sizeT = 0;
      READ_VAR( sizeT );
      const DWORD size = sizeof(*pCodes[n].pVars) * pCodes[n].cVars;
      ASS( sizeT==size );
      pCodes[n].pVars = new (KMemoryServiceH5xx::Instance().NewMem( size )) KCalcVar[pCodes[n].cVars];
      READ_DATA( size, pCodes[n].pVars );
    }
    if( pCodes[n].cOPs )
    {
      DWORD sizeT = 0;
      READ_VAR( sizeT );
      const DWORD size = sizeof(*pCodes[n].pOPs) * pCodes[n].cOPs;
      ASS( sizeT==size );
      pCodes[n].pOPs = new (KMemoryServiceH5xx::Instance().NewMem(size)) SOPElement[pCodes[n].cOPs];
      READ_DATA( size, pCodes[n].pOPs );
    }
    if( pCodes[n].cLocalVars )
    {
      DWORD sizeT = 0;
      READ_VAR( sizeT );
      const DWORD size = sizeof(*pCodes[n].pLocalVars) * pCodes[n].cLocalVars;
      ASS( sizeT==size );
      pCodes[n].pLocalVars = new (KMemoryServiceH5xx::Instance().NewMem(size)) SLocalVar[pCodes[n].cLocalVars];
      READ_DATA( size, pCodes[n].pLocalVars );
    }
  }
  return true;
}
