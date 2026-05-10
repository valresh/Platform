#include "Trans.h"
#include <string>

using namespace std;
using namespace ns_translator;


bool KAaTranslator::AnalizeTok( eYToken_value &tok, std::string &szVal )
{
  switch( tok )
  {
  case eytEndExpr:
    while( PosSt > 0 )
    {
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      AddToOP( OP_Operation, (STokenElement)C );
      Parm.Init();
    }
  case eytTAB:
  case unknown:
  case eytCOMMENT:
  case eytSKIP:
    return true;
  case eytTEXT:
    if( !IsSys( tok, szVal) )
      IsConst( tok, szVal );
    break;
  case endLine:
    if( Parm.ifOpenThenClose )
      return false;
    break;
  }

  switch( tok )
  {
  case eytPROGRAM:
  case eytEND:
    break;
  case eytCOMMA:
    ASSD( Parm.levelLPRP>=0 && Parm.levelLPRP<_countof(Parm.LPRP) );
    Parm.LPRP[Parm.levelLPRP]++;
    while ( PosSt > 0 )
    {
      if( eytLP==Stack[PosSt-1] )
        break;
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      AddToOP( OP_Operation, (STokenElement)C );
    }
    break;
  case endLine:
    if( Parm.skipEndLine )
      break;
    while ( PosSt > 0 )
    {
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      AddToOP( OP_Operation, (STokenElement)C );
    }
    Parm.Init();
    break;
  case eytDEFINE:
    {
      eYToken_value t = GetTokenFromLine();
      KTHROW( eytTEXT==t );
      string def = m_szValue;
      m_szValue.clear();
      char ch = 0;
      if(!m_pLineStream->get(ch)) 
        break;
      do 
      {
        if(!m_pLineStream->get(ch)) 
          break;
        m_szValue += ch;
      }while( ch!='\n' );
      m_Defines[def] = m_szValue;
    }
    break;
  case eytALIAS:
    {
      eYToken_value t = GetTokenFromLine();
      tAliasByStrCont_::iterator it = m_Aliases.find( m_szValue );
      KTHROW( m_Aliases.end()==it );
      KTHROW( eytTEXT==t );
      string alias = m_szValue;
      t = GetTokenFromLine();
      KVar::eType type = KVar::None;
      if( eytTEXT==t )
      {
        int N = AddVar( m_szValue.c_str(), KVar::Reference );
        type = KVar::Reference;
        KTHROW( N>=0 );
      }
      /*else if( eytCONDITION==t )
      {
        int N = AddVar( m_szValue.c_str(), KVar::Condition );
        type = KVar::Condition;
        KTHROW( N>=0 );
      }*/
      else
      {
        KTHROW( !"чудеса в alias" );
      }
      SAliasInfo ai = {m_szValue,type};
      m_Aliases[alias] = ai;
    }
    break;

  case eytSWITCH:
    Parm.LevSwitch++;
    KTHROWm( Parm.AddrSwitch[Parm.LevSwitch] == 0, "Неверный SWITCH" );
    Parm.LastCase[Parm.LevSwitch] = 0;
    AddToOP( OP_SWITCH );
    NextTokens();
    break;
  case eytEND_SWITCH:
    while ( PosSt > 0 )
    {
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      AddToOP( OP_Operation, (STokenElement)C );
    }
    while( Parm.LevSwitch >=0 )
    {
      AddToOP( OP_BREAKTO, kOP+2 );
      int addrNext = kOP, addrEnd = kOP;
      int &LevSwitch = Parm.LevSwitch;
      for( int index = Parm.LastCase[LevSwitch]-1; index>=0; index-- )
      {
        KTHROWm( Parm.AddrCase[LevSwitch][ index ] != 0, "Неверный CASE" );
        int A = Parm.AddrCase[LevSwitch][ index ];
        KTHROW( !(OP_OTHERWISE!=static_cast<ns_translator::eStOP>(OP[A].Type)&&OP_CASE!= static_cast<ns_translator::eStOP>(OP[A].Type) ) );
        int delta = 3;//(OP_CASE==OP[A]) ? 3 : 1;
        if( index > 0 )
          OP[A-delta] = addrEnd;// Переход на end_switch
        /*else if( index==0 )
          OP[A-delta] = (int)OP[A-delta]+1;*/
        KTHROW( OP[A+1] == 0 );
        OP[A+1] = addrNext;// Переход к следующему
        KTHROW( OP[A+2] == 0 );
        OP[A+2] = addrEnd;// Переход на end_switch
        addrNext = A-2;
        Parm.AddrBeforCase[LevSwitch][ index ] = 0;
        Parm.AddrCase[LevSwitch][ index ] = 0;
      }
      Parm.LastCase[LevSwitch] = 0;
      Parm.AddrSwitch[LevSwitch] = 0;
      Parm.LevSwitch--;
    }
    return false;
    break;
  case eytNEGATIVE:
    NextTokens( true );
    AddToOP( OP_Operation, tok );
    break;
  case eytELSEIF:
    KKK();
  case eytELSE:
    while( PosSt > 0 )
    {
      if( Stack[PosSt-1] == eytASSIGN )
        break;
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      if( (STokenElement)C == eytLP )
        break;
      AddToOP( OP_Operation, (STokenElement)C );
    }
    if( Parm.wasElse[Parm.LevIF] > 0 )
    {
      for( int L = Parm.LevIF; L > -1; --L )
      {
        if( !Parm.wasElse[L] )
          break;
        if( Parm.wasEndIf[L] > 0 )
          break;
        ASSD( Parm.LevIF>0 );
        string v;
        eYToken_value t = eytEND_IF;
        AnalizeTok( t, v );
      }
    }
    ASSD( Parm.wasElse[Parm.LevIF] < 2 );
    Parm.wasElse[Parm.LevIF]++;
    Parm.lastLevelOnElse = Parm.LevIF;
    {
      KTHROWm( Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1] != 0,"Неверный IF" );
      int A = Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1];
      AddToOP( OP_GOTO_OTHER_ELSE, A+2);
      KTHROW( OP[A+1] == 0 );
      if( OP[A+1]==0 )
        OP[A+1] = kOP;// Переход по false
    }
    if( eytELSE==tok )
      break;
  case eytIF:
    {
      while( PosSt > 0 )
      {
        if( Stack[PosSt-1] == eytASSIGN )
          break;
        KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
        PosSt--;
        if( (STokenElement)C == eytLP )
          break;
        AddToOP( OP_Operation, (STokenElement)C );
      }

      m_bIsLogicOperation = TRUE;
      Parm.ifOpenThenClose++;
      
      if( eytELSE!=Parm.tokAfterAnalize && eytELSEIF!=tok )
      {
        Parm.LevIF++;
        Parm.lastLevelOnElse = -1;
      }

      Parm.wasEndIf[Parm.LevIF] = 0;
      Parm.wasElse[Parm.LevIF] = 0;

      //NextTokens();
      ASS( _countof(Parm.AddrIfElseIfOnLevel)>Parm.LevIF );
      ASS( _countof(Parm.CountIfElseIf)>Parm.LevIF );
      ASS( _countof(Parm.AddrIfElseIfOnLevel[Parm.LevIF])>Parm.CountIfElseIf[Parm.LevIF] );
      KTHROWm( Parm.AddrIfElseIfOnLevel[Parm.LevIF][ Parm.CountIfElseIf[Parm.LevIF] ] == 0, "Неверный IF" );
      int &_kOP = Parm.AddrIfElseIfOnLevel[Parm.LevIF][ Parm.CountIfElseIf[Parm.LevIF] ];
      Parm.CountIfElseIf[Parm.LevIF]++;
      _kOP = kOP;
      Parm.ifPoses[Parm.LevIF] = kOP;
      AddToOP( OP_IF, 0, 0 ); // Переход по false, Переход на endif

      int curLevIF = Parm.LevIF;
      NextTokens();
      m_bIsLogicOperation = FALSE;

      if( Parm.ifOpenThenClose )
      {
        /*while( PosSt > 0 )
        {
          KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
          PosSt--;
          if( (STokenElement)C == eytLP )
            break;
          AddToOP( OP_Operation, (STokenElement)C );
        }*/
        if( curLevIF == Parm.LevIF )
        {
          string v;
          eYToken_value t = eytEND_IF;
          AnalizeTok( t, v );
        }
        Parm.ifOpenThenClose--;
        ASS( Parm.ifOpenThenClose>=0 );
      }
      /*if ( PosTestSt <= 0 )
        throw "Неверная лог операция";
      PosTestSt--;*/
    }
    break;
  case eytTHEN:
    Parm.ifOpenThenClose--;
    ASS( Parm.ifOpenThenClose>=0 );
  case eytEND_IF:
    while ( PosSt > 0 )
    {
      if( Stack[PosSt-1] == eytASSIGN )
        break;
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      AddToOP( OP_Operation, (STokenElement)C );
    }
    if( eytTHEN==tok)
      return false;
    //while
    if( Parm.LevIF >=0 )
    {
      ASSD( !Parm.wasEndIf[Parm.LevIF] );
      while( Parm.CountIfElseIf[Parm.LevIF] )
      {
        KTHROWm( Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1] != 0, "Неверный IF" );
        int A = Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1];
        Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1] = 0;
        if( OP[A+1] == 0 )
          OP[A+1] = kOP;
        KTHROW( OP[A+2] == 0 )
        OP[A+2] = kOP;// Переход на endif
        Parm.CountIfElseIf[Parm.LevIF]--;
      }
      Parm.wasEndIf[Parm.LevIF]++;
      --Parm.LevIF;
      Parm.lastLevelOnElse = -1;
    }
    if( Parm.LevIF < 0 )
      Parm.Init();
    break;
  /*case eytCONDITION:
    {
      int N = AddVar( szVal.c_str(), KVar::Condition );
      KTHROW( N>=0 );
      AddToOP( OP_LoadVar, N );
    }
    break;*/
  case eytSTRING:
    {
      int N = AddVar( szVal.c_str(), KVar::Const_String );
      KTHROW( N>=0 );
      AddToOP( OP_LoadVar, N );
    }
    break;
  case eytINDEX_CLOSE:
    return false;
    break;
  case eytTEXT_INDEX_OPEN:
    {
      string lVal = szVal;

      Parm.StartOperator = false;
      NextTokens();

      bool b = IsVar( lVal.c_str() );
      ASS( b );
      int N = AddVar( lVal.c_str(), /*b ? */KVar::Reference/* : KVar::UnknownTypeReference*/ );
      KTHROW( N>=0 );
      if( Parm.StartOperator )
      {
        AddToOP( OP_AddrVarIndex, N );
        Parm.StartOperator = false;
      }
      else
      {
        AddToOP( OP_LoadVarIndex, N );
      }
    }
    break;
  case eytTEXT:
    {
      LPCSTR pszDefine = IsDefine( szVal );
      if( pszDefine )
      {
        Parm.skipEndLine++;
        Parm.skipShiftTokHistory = 1;
        string str(pszDefine);
        AnalizeLine( str );
        Parm.skipEndLine--;
        break;
      }
      int N = IsFunc( szVal.c_str() );
      if( N >= 0 )
      {
        nFunction[kFunction++] = (eYFunctions)N;
        ArithOP( eytFUNCTION );
        break;
      }
      bool b = IsVar( szVal.c_str() );
      ASS( b );
      N = AddVar( szVal.c_str(), /*b ? */KVar::Reference/* : KVar::UnknownTypeReference*/ );
      KTHROW( N>=0 );
      if( Parm.StartOperator )
      {
        AddToOP( OP_AddrVar, N );
        Parm.StartOperator = false;
      }
      else
      {
        AddToOP( OP_LoadVar, N );
      }
    }
    break;
  case eytNUMBER:
    {
      int N = AddVar( szVal.c_str(), KVar::Const );
      KTHROW( N>=0 );
      AddToOP( OP_LoadVar, N );
    }
    break;
  case eytDIM:
    {
      eYToken_value t = unknown;
      t = GetTokenFromLine();
      KTHROW( eytTEXT==t );
      string name = m_szValue;
      t = GetTokenFromLine();
      KTHROW( eytTEXT==t );
      KTHROW( !_strcmpi("AS", m_szValue.c_str() ) );
      t = GetTokenFromLine();
      ASS( eytTEXT==t );
      KVar::eType type = KVar::None;
      if( !_strcmpi("FLOAT", m_szValue.c_str() ) )
        type = KVar::Double;
      else if( !_strcmpi("DOUBLE", m_szValue.c_str() ) )
        type = KVar::Double;
      else if( !_strcmpi("REAL", m_szValue.c_str() ) )
        type = KVar::Double;
      else if( !_strcmpi("INTEGER", m_szValue.c_str() ) )
        type = KVar::Integer;
      else if( !_strcmpi("Message", m_szValue.c_str() ) )
        type = KVar::String;
      else if( !_strcmpi("STRING", m_szValue.c_str() ) )
        type = KVar::String;
      else
      {
        KTHROW( !"Определить тип" );
      }
      int N = AddVar( name.c_str(), type );
      KTHROW( N>=0 );
    }
    break;
  case eytDOUBLEkeyword:
    {
      eYToken_value t = unknown;
      do
      {
        t = GetTokenFromLine();
        if( eytTEXT==t )
        {
          int N = AddVar( m_szValue.c_str(), KVar::Double );
          KTHROW( N>=0 );
        }
      }while( t!=endLine );
    }
    break;
  case eytINTEGERkeyword:
    {
      eYToken_value t = unknown;
      do
      {
        t = GetTokenFromLine();
        if( eytTEXT==t )
        {
          int N = AddVar( m_szValue.c_str(), KVar::Integer );
          KTHROW( N>=0 );
        }
      }while( t!=endLine );
    }
    break;
  case eytLP:
    ++Parm.levelLPRP;
    ASSD( Parm.levelLPRP < _countof(Parm.LPRP) );
    Parm.LPRP[Parm.levelLPRP] = 0;
  case eytASSIGN:
    ArithOP( tok );
    break;
  case eytRP:
    {
      ASSD( Parm.levelLPRP>=0 && Parm.levelLPRP<_countof(Parm.LPRP) );
      Parm.LPRP[Parm.levelLPRP]++;
      int params = Parm.LPRP[Parm.levelLPRP];
      // Если ')' - Все из стека трансляции до '('
      while( PosSt > 0 )
      {
        KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
        PosSt--;
        if( (STokenElement)C == eytLP )
        {
          Parm.LPRP[Parm.levelLPRP] = 0;
          ASSD( Parm.levelLPRP >= 0);
          --Parm.levelLPRP;
          break;
        }
        AddToOP( OP_Operation, (STokenElement)C );
      }
      if( PosSt > 0 )
      {
        STokenElement C = Stack[PosSt-1];
        if( C==eytFUNCTION )
        {
          KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
          KTHROW( kFunction > 0 );
          if( eyfAVG==nFunction[kFunction-1] )
          {
            char szNumb[8] = {};
            sprintf_s( szNumb, "%d", params );
            int N = AddVar( szNumb, KVar::Const );
            KTHROW( N>=0 );
            AddToOP( OP_LoadVar, N );
          }
          AddToOP( OP_Function, nFunction[--kFunction] );
          PosSt--;
        }
      }
    }
    break;
  case eytDIV:
  case eytNOT:
  case eytSHIFT_RIGHT:
  case eytSHIFT_LEFT:
  case eytBITAND:
  case eytBITOR:
  case eytPLUS:
  case eytMINUS:
  case eytMUL:
  case eytPERCENT:
  case eytMOD:
  case '^':
  case eytAND:
  case eytOR:
    //
  case eytBIG:
  case eytLIT:
  case eytEQUAL:
  case eytGE:
  case eytNE:
  case eytLE:
    {
      const int Prty = Prior( tok );
      while ( PosSt > 0 )
      {
        STokenElement C = Stack[PosSt-1];
        if ( !IsArith( C ) )
          break;
        const int P = Prior( C );
        if( P < Prty )
          break;
        AddToOP( OP_Operation, C);
        PosSt--;
      }
      ArithOP( tok );
    }
    break;
  case eytEXIT:
    AddToOP( OP_EXIT );
    break;
  case eytLABEL:
    KTHROW( Parm.LevSwitch==-1 );
    AddToOP( OP_LABEL, kOP );
    break;
  default:
    tok;
    KTHROW( !"надо реализовать обработку токена");
  }
  return true;
}
