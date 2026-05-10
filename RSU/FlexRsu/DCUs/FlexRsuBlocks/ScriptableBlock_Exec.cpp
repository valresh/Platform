#include "ScriptableBlock.h"
#include <math.h>
#include <time.h>
#include <crosslock.h>

using namespace ns_translator;
using namespace ns_Script;

class KCalcRes : public KCalcVar
{
public:
  BOOL m_bAddrVar;
  KCalcRes()
  {
    memset( this, 0, sizeof(*this) );
    type = evtHZ;
  }
  void Reset()
  {
    KCalcVar empty;
    *this = empty;
  }
  KCalcRes& operator = ( const KCalcVar &thr )
  {
    *(static_cast<KCalcVar*>(this)) = thr;
    return *this;
  }
  operator tDigital32()
  {
    if( evtAnalog32==type /*|| evtConstFloat==type*/ )
    {
      if( pMem )
        return (tDigital32)*(tAnalog32*)pMem;
      return (tDigital32)fltValue;
    }
    ASSD( evtDigital32==type /*|| evtConstInt==type*/ );
    if( pMem )
      return *(tDigital32*)pMem;
    return intValue;
  }
  operator tAnalog32()
  {
    if( evtDigital32==type /*|| evtConstInt==type*/ )
    {
      if( pMem )
        return (tAnalog32)*(tDigital32*)pMem;
      return (tAnalog32)intValue;
    }
    ASSD( evtAnalog32==type /*|| evtConstFloat==type*/ );
    if( pMem )
      return *(tAnalog32*)pMem;
    return fltValue;
  }
  KCalcRes& SetConst( tDigital32 thr )
  {
    if( pMem && m_bAddrVar )
    {
      ASSD( 0 );
    }
    else
    {
      type = evtDigital32;//evtConstInt;
      intValue = thr;
      pMem = NULL;//(BYTE*)&intValue;
    }
    return *this;
  }
  KCalcRes& operator = ( tAnalog32 thr )
  {
    if( pMem && m_bAddrVar && type > evtHZ )
    {
      ASSD( evtAnalog32==type );
      *(tAnalog32*)pMem = thr;
    }
    else
    {
      type = evtAnalog32;
      fltValue = thr;
      pMem = NULL;
    }
    return *this;
  }
  KCalcRes& operator = ( tDigital32 thr )
  {
    if( pMem && m_bAddrVar && type > evtHZ )
    {
      if( evtDigital32==type )
        *(tDigital32*)pMem = thr;
      else if( evtAnalog32==type )
        *(tAnalog32*)pMem = (tAnalog32)thr;
      else
      {
        ASSD(0);
      }
    }
    else
    {
      type = evtDigital32;
      intValue = thr;
      pMem = NULL;
    }
    return *this;
  }
};

static KCalcRes Stack[100];
static int kStack = 0;

static void push_back( KCalcVar &var )
{
  ASS( kStack < countof(Stack) );
  Stack[kStack++] = var;
}

static void push_back( KCalcVar *var )
{
  ASS( kStack < countof(Stack) );
  Stack[kStack] = *var;
  Stack[kStack].m_bAddrVar = true;
  kStack++;
}

static KCalcRes pop_back()
{
  ASS( kStack >= 1 );
  KMethodOnDestroy<KCalcRes> C(Stack[--kStack], &KCalcRes::Reset );
  return C;
}

//static CRITICAL_SECTION s_CS;
static std::recursive_mutex s_CS;
static bool bInitedCS = false;

void KScriptableBlock::InitSync()
{
  //if( !bInitedCS )
    //InitializeCriticalSection( &s_CS );
  bInitedCS = true;
  srand( (unsigned)time( NULL ) );
}

bool KScriptableBlock::ExecExpression( SCallParams &parm )
{
  if( !m_nInited )
  {
    m_nInited = 1;
    InitVars();
  }
  if( 1!=m_nInited )
    return false;

  //KCsSync _sink( s_CS );
  LOCK(s_CS);
  KCalcVar *Var = m_pVars;
  SOPElement* OP = m_pOPs;

  int nOP = 0;
  kStack = 0;
  while( nOP < m_scriptParams.cOPs )
  {
    SOPElement CurrOP = (SOPElement)OP[nOP++];
    switch( CurrOP.OP )
    {
    default:
      ASSD( 0 );
      break;
    case OP_AddrVar:
      {
        int N = OP[nOP++];
        ASS( N < m_scriptParams.cVars );
        push_back( &Var[N] );
      }
      break;
    case OP_LoadVar:
      {
        int N = OP[nOP++];
        ASS( N < m_scriptParams.cVars );
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
    case OP_GOTO_OTHER_ELSE:
      {
        int OnEndif = OP[nOP++];// Переход на endif
        ASS(OnEndif>0 && OnEndif <= m_scriptParams.cOPs);
        nOP = OP[OnEndif];
      }
      break;
    case OP_IF:
      {
        int OnFalse = OP[nOP++];// Переход по false
        if( !OnFalse || OnFalse >= m_scriptParams.cOPs )
          OnFalse = m_scriptParams.cOPs;
        int OnEndif = OP[nOP++];// Переход на endif
        if( !OnEndif || OnEndif >= m_scriptParams.cOPs )
          OnEndif = m_scriptParams.cOPs;
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

  return true;
}

static void ConvertI2( KCalcRes &change, KCalcRes const &to )
{
}

static void ConvertF2( KCalcRes &change, KCalcRes const &to )
{
}

static void NormalizeType( KCalcRes &vl, KCalcRes &vr )
{
}

void KScriptableBlock::Operation( ns_translator::eYToken_value cmd )
{
  switch( cmd )
  {
  default:
    ASS( 0 );
    break;
  case eytMUL:
    {
      KCalcRes vr = pop_back();
      KCalcRes vl = pop_back();
      NormalizeType( vr, vl );
      KCalcRes res;
      if( evtAnalog32==vl.type || evtAnalog32==vr.type )
      {
          tAnalog32 l = vl, r = vr;
          tAnalog32 o = l * r;
          res = o;
      }
      else
      {
        tDigital32 l = vl, r = vr;
        tDigital32 o = l * r;
        res = o;
      }
      push_back( res );
    }
    break;
  case eytPOW:
    {
      KCalcRes vr = pop_back();
      KCalcRes vl = pop_back();
      NormalizeType( vr, vl );
      KCalcRes res;
      if( evtAnalog32==vl.type || evtAnalog32==vr.type )
      {
        tAnalog32 l = vl, r = vr;
        tAnalog32 o = pow( l, r );
        res = o;
      }
      else
      {
        tDigital32 l = vl, r = vr;
        tDigital32 o = (tDigital32)pow( (tAnalog32)l, r );
        res = o;
      }
      push_back( res );
    }
    break;
  case eytDIV:
    {
      KCalcRes vr = pop_back();
      KCalcRes vl = pop_back();
      NormalizeType( vr, vl );
      KCalcRes res;
      if( evtAnalog32==vl.type || evtAnalog32==vr.type )
      {
        tAnalog32 l = vl, r = vr;
        tAnalog32 o;
        o = 0;
        if( r != 0)
          o = (l / r);
        res = o;
      }
      else
      {
        tDigital32 l = vl, r = vr;
        tDigital32 o;
        o = 0;
        if( r != 0)
          o = (l / r);
        res = o;
      }
      push_back( res );
    }
    break;
  case eytMINUS:
    {
      KCalcRes vr = pop_back();
      KCalcRes vl = pop_back();
      NormalizeType( vr, vl );
      KCalcRes res;
      if( evtAnalog32==vl.type || evtAnalog32==vr.type )
      {
        tAnalog32 l = vl, r = vr;
        tAnalog32 o;
        o = l - r;
        res = o;
      }
      else
      {
        tDigital32 l = vl, r = vr;
        tDigital32 o;
        o = l - r;
        res = o;
      }
      push_back( res );
    }
    break;
  case eytPLUS:
    {
      KCalcRes vr = pop_back();
      KCalcRes vl = pop_back();
      NormalizeType( vr, vl );
      KCalcRes res;
      if( evtAnalog32==vl.type || evtAnalog32==vr.type )
      {
        tAnalog32 l = vl, r = vr;
        tAnalog32 o;
        o = l + r;
        res = o;
      }
      else
      {
        tDigital32 l = vl, r = vr;
        tDigital32 o;
        o = l + r;
        res = o;
      }
      push_back( res );
    }
    break;
  case eytGT:
  case eytGE:
  case eytLT:
  case eytLE:
  case eytNE:
  case eytEQUAL:
    {
      KCalcRes vr = pop_back();
      KCalcRes vl = pop_back();
      NormalizeType( vr, vl );
      KCalcRes res;
      tDigital32 r = 0;
      if( evtAnalog32==vl.type || evtAnalog32==vr.type )
      {
        tAnalog32 curl = vl;
        tAnalog32 curr = vr;
        switch( cmd )
        {
        case eytGE:
          r = curl >= curr ? 1 : 0;
          break;
        case eytGT:
          r = curl > curr ? 1 : 0;
          break;
        case eytLE:
          r = curl <= curr ? 1 : 0;
          break;
        case eytLT:
          r = curl < curr ? 1 : 0;
          break;
        case eytNE:
          r = (abs(curl - curr) > 0.000001f) ? 1 : 0;
          break;
        case eytEQUAL:
          r = (abs(curl - curr) < 0.000001f) ? 1 : 0;
          break;
        }
      }
      else
      {
        tDigital32 curl = vl;
        tDigital32 curr = vr;
        switch( cmd )
        {
        case eytGE:
          r = curl >= curr ? 1 : 0;
          break;
        case eytGT:
          r = curl > curr ? 1 : 0;
          break;
        case eytLE:
          r = curl <= curr ? 1 : 0;
          break;
        case eytLT:
          r = curl < curr ? 1 : 0;
          break;
        case eytNE:
          r = curl != curr ? 1 : 0;
          break;
        case eytEQUAL:
          r = curl == curr ? 1 : 0;
          break;
        }
      }
      res = r;
      push_back( res );
    }
    break;
  case eytOR:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      NormalizeType( V1, V2 );
      KCalcRes res;
      if( evtAnalog32==V1.type || evtAnalog32==V2.type )
      {
        tDigital32 cur1 = V1;
        tDigital32 cur2 = V2;
        tDigital32 r = cur1 || cur2;
        res = r;
      }
      else
      {
        tDigital32 cur1 = V1;
        tDigital32 cur2 = V2;
        tDigital32 r = cur1 || cur2;
        res = r;
      }
      push_back( res );
    }
    break;
  case eytAND:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      NormalizeType( V1, V2 );
      KCalcRes res;
      if( evtAnalog32==V1.type || evtAnalog32==V2.type )
      {
        tDigital32 cur1 = V1;
        tDigital32 cur2 = V2;
        tDigital32 r = cur1 && cur2;
        res = r;
      }
      else
      {
        tDigital32 cur1 = V1;
        tDigital32 cur2 = V2;
        tDigital32 r = cur1 && cur2;
        res = r;
      }
      push_back( res );
    }
    break;
  case eytBITOR:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      NormalizeType( V1, V2 );
      KCalcRes res;
      switch( V1.type )
      {
      case evtDigital32:
        {
          tDigital32 cur1 = V1;
          tDigital32 cur2 = V2;
          tDigital32 r = cur1 | cur2;
          res = r;
        }
        break;
      default:
        ASSD(0);
      }
      push_back( res );
    }
    break;
  case eytBITAND:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      NormalizeType( V1, V2 );
      KCalcRes res;
      switch( V1.type )
      {
      case evtDigital32:
        {
          tDigital32 cur1 = V1;
          tDigital32 cur2 = V2;
          tDigital32 r = cur1 & cur2;
          res = r;
        }
        break;
      default:
        ASSD(0);
      }
      push_back( res );
    }
    break;
  case eytASSIGN:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      NormalizeType( V1, V2 );
      KCalcRes res;
      switch( V1.type )
      {
      case evtAnalog32:
        V2 = (tAnalog32)V1;
        break;
      case evtDigital32:
        V2 = (tDigital32)V1;
        break;
      default:
        ASSD(0);
        KKK();
      }
    }
    break;
  case eytXOR:
    {
      KCalcRes V1 = pop_back();
      KCalcRes V2 = pop_back();
      NormalizeType( V1, V2 );
      KCalcRes res;
      switch( V1.type )
      {
      case evtDigital32:
        {
          tDigital32 cur1 = V1;
          tDigital32 cur2 = V2;
          tDigital32 r = cur1 ^ cur2;
          res = r;
        }
        break;
      default:
        ASSD(0);
      }
      push_back( res );
    }
    break;
  case eytNegation:
    {
      KCalcRes V = pop_back();
      KCalcRes res;
      switch( V.type )
      {
      case evtAnalog32:
        {
          tAnalog32 a = V;
          a = -a;
          res = a;
        }
        break;
      case evtDigital32:
        {
          tDigital32 a = V;
          a = -a;
          res = a;
        }
        break;
      default:
        ASSD(0);
      }
      push_back( res );
    }
    break;
  /*case eytInverseByVar:
    {
      KCalcRes inv = pop_back();
      KCalcRes V = pop_back();
      KCalcRes res;
      ASSD( evtString==inv.type );
      switch( V.type )
      {
      case evtBin:
        {
          LPCSTR pszNOT = (LPCSTR)inv.pMem;
          tBin_ cur = V;
          switch( *pszNOT )
          {
          case 'o':
          case 'O':
            if( cur & 1 )
              cur &= ~1;
            else
              cur |= 1;
            break;
          }
          res = cur;
        }
        break;
      default:
        ASSD(0);
      }
      push_back( res );
    }
    break;*/
  }
}

void KScriptableBlock::Function( ns_translator::eYFunctions nF )
{
  long long _RAND_MAX = RAND_MAX;
  switch( nF )
  {
  default:
    ASS( 0 );
    break;
  case eyfABS:
    {
      KCalcRes s = pop_back();
      switch( s.type )
      {
      case evtAnalog32:
        {
          tAnalog32 a = s;
          tAnalog32 r = abs( a );
          s = r;
        }
        break;
      default:
        ASSD(0);
        break;
      }
      push_back( s );
    }
    break;
  case eyfSQRT:
    {
      KCalcRes s = pop_back();
      switch( s.type )
      {
      case evtAnalog32:
        {
          tAnalog32 a = s;
          tAnalog32 r = sqrtf( a );
          s = r;
        }
        break;
      default:
        ASSD(0);
        break;
      }
      push_back( s );
    }
    break;
  case eyfSR:
    {
      KCalcRes st = pop_back();
      KCalcRes r = pop_back();
      KCalcRes s = pop_back();
      if( (tDigital32)r )
        st = 0;
      if( (tDigital32)s )
        st = 1;
      push_back( st );
    }
    break;
  case eyfRS:
    {
      KCalcRes st = pop_back();
      KCalcRes s = pop_back();
      KCalcRes r = pop_back();
      if( (tDigital32)s )
        st = 1;
      if( (tDigital32)r )
        st = 0;
      push_back( st );
    }
    break;
  case eyfRAND:
    {
      KCalcRes ma = pop_back();
      KCalcRes mi = pop_back();
      tDigital32 range_min = mi, range_max = ma;
      tDigital32 u = (double)rand() / (_RAND_MAX + 1) * (range_max - range_min) + range_min;
      KCalcRes res;
      res = u;
      push_back( res );

    }
    break;
  }
}
