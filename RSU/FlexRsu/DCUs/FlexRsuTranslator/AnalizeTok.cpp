#include "FlexTranslator.h"
#include <rsuErr.h>

using namespace ns_translator;
using namespace std;

bool KFlexTranslator::AnalizeTok( eYToken_value &tok, const std::string &szVal )
{
  switch( tok )
  {
  case eytEndExpr:
    while( PosSt > 0 )
    {
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      HandleInvers( C );
      AddToOP( OP_Operation, (STokenElement)C );
    }
    Parm.Init();
  case eytTAB:
  case unknown:
  case eytSKIP:
    return true;
  case eytCOMMA:
    while( PosSt > 0 )
    {
      STokenElement t = Stack[PosSt-1];
      if( t == eytLP )
        break;
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      HandleInvers( C );
      AddToOP( OP_Operation, (STokenElement)C );
    }
    return true;
  case eytTEXT:
    if( !IsSys( tok, szVal) )
    {
      //IsConst( tok, szVal );
    }
    break;
  case endLine:
    if( Parm.ifOpenThenClose )
      return false;
    break;
  }

  switch( tok )
  {
  default:
    KTHROW( !"надо реализовать обработку токена");
    break;
  case eytEND:
    break;
  case eytCOMMA:
    while ( PosSt > 0 )
    {
      if( eytLP==Stack[PosSt-1] )
        break;
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      HandleInvers( C );
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
      HandleInvers( C );
      AddToOP( OP_Operation, (STokenElement)C );
    }
    Parm.Init();
    break;
  case eytASSIGN:
  case eytLP:
    ArithOP( tok );
    break;
  case eytPLUS:
  case eytMINUS:
  case eytMUL:
  case eytDIV:
  case eytPOW:
  case eytBITAND:
  case eytBITOR:
  case eytOR:
  case eytXOR:
  case eytAND:
  case eytGT:
  case eytLT:
  case eytEQUAL:
  case eytNE:
  case eytGE:
  case eytLE:
  case eytNegation:
    {
      int Prty = Prior( tok );
      while( PosSt > 0 )
      {
        STokenElement C = Stack[PosSt-1];
        if ( !IsArith( C ) )
          break;
        int P = Prior( C );
        if ( Prty > P )
          break;
        HandleInvers( C );
        AddToOP( OP_Operation, C);
        PosSt--;
      }
      ArithOP( tok );
    }
    break;
  case eytRP:
    // Если ')' - Все из стека трансляции до '('
    /*if( Parm.falseOn100RP )
      Parm.falseOn100RP--;*/
    while( PosSt > 0 )
    {
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      if( (STokenElement)C == eytLP )
        break;
      HandleInvers( C );
      AddToOP( OP_Operation, (STokenElement)C );
    }
    if( PosSt > 0 )
    {
      STokenElement C = Stack[PosSt-1];
      if( C==eytFUNCTION )
      {
        KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
        KTHROW( kFunction > 0 );
        AddToOP( OP_Function, nFunction[--kFunction] );
        PosSt--;
      }
    }
    /*if( 100==Parm.falseOn100RP )
      return false;*/
    break;
  case eytInverseByVar:
    {
      bool b = IsVar( szVal.c_str() );
      ASS( b );
      int N = AddVar( szVal.c_str(), KVar::Reference );
      KTHROW( N>=0 );
      StackInversVars[kInvers++] = N;
      ArithOP( eytInverseByVar );
    }
    break;
  case eytNUMBER:
    {
      int N = AddVar( szVal.c_str(), KVar::Const );
      KTHROW( N>=0 );
      AddToOP( OP_LoadVar, N );
    }
    break;
  case eytLoadAddr:
    Parm.StartOperator = true;
  case eytTEXT:
    {
      int N = IsFunc( szVal.c_str() );
      if( N >= 0 )
      {
        nFunction[kFunction++] = (eYFunctions)N;
        ArithOP( eytFUNCTION );
        break;
      }
      bool b = IsVar( szVal.c_str() );
      ASS( b );
      N = AddVar( szVal.c_str(), KVar::Reference );
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
  case eytELSEIF:
    KKK();
  case eytELSE:
    while( PosSt > 0 )
    {
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      if( (STokenElement)C == eytLP )
        break;
      AddToOP( OP_Operation, (STokenElement)C );
    }
    {
      KTHROWm( Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1] != 0,"Неверный IF" );
      int A = Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1];
      AddToOP( OP_GOTO_OTHER_ELSE, A+2);
      KTHROW( OP[A+1] == 0 );
      OP[A+1] = kOP;// Переход по false
    }
    if( eytELSE==tok )
      break;
  case eytIF:
    {
      while( PosSt > 0 )
      {
        KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
        PosSt--;
        if( (STokenElement)C == eytLP )
          break;
        AddToOP( OP_Operation, (STokenElement)C );
      }

      m_bIsLogicOperation = true;
      Parm.ifOpenThenClose++;
      
      if( eytELSE!=Parm.tokAfterAnalize && eytELSEIF!=tok )
        Parm.LevIF++;

      Parm.StartOperator = 0;
      NextTokens();

      ASS( countof(Parm.AddrIfElseIfOnLevel)>Parm.LevIF );
      ASS( countof(Parm.CountIfElseIf)>Parm.LevIF );
      ASS( countof(Parm.AddrIfElseIfOnLevel[Parm.LevIF])>Parm.CountIfElseIf[Parm.LevIF] );
      KTHROWm( Parm.AddrIfElseIfOnLevel[Parm.LevIF][ Parm.CountIfElseIf[Parm.LevIF] ] == 0, "Неверный IF" );
      int &_kOP = Parm.AddrIfElseIfOnLevel[Parm.LevIF][ Parm.CountIfElseIf[Parm.LevIF] ];
      Parm.CountIfElseIf[Parm.LevIF]++;
      _kOP = kOP;
      AddToOP( OP_IF, 0, 0 ); // Переход по false, Переход на endif

      NextTokens();
      m_bIsLogicOperation = false;

      if( Parm.ifOpenThenClose )
      {
        ns_translator::eYToken_value et(eytEND_IF);
        AnalizeTok( et, "" );
        Parm.ifOpenThenClose--;
        ASS( Parm.ifOpenThenClose>=0 );
      }
    }
    break;
  case eytTHEN:
    Parm.ifOpenThenClose--;
    ASS( Parm.ifOpenThenClose>=0 );
  case eytEND_IF:
    KKK();
    while ( PosSt > 0 )
    {
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      AddToOP( OP_Operation, (STokenElement)C );
    }
    if( eytTHEN==tok)
      return false;
    //while
    if( Parm.LevIF >=0 )
    {
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
      Parm.LevIF--;
    }
    if( Parm.LevIF < 0 )
      Parm.Init();
    break;
  }
  return true;
}

int KFlexTranslator::ArithOP( ns_translator::eYToken_value AOP )
{
  switch( AOP )
  {
  default:
  case eytFUNCTION:
    if( PosSt >= countof(Stack) )
      KTHROW( !"Стек мал" );
    Stack[PosSt++] = AOP;
    break;
  }
  return PosSt-1;
}

void KFlexTranslator::HandleInvers( STokenElement C )
{
  if( eytInverseByVar!=C )
    return;
  KTHROW( kInvers > 0 );
  AddToOP( OP_LoadVar, StackInversVars[--kInvers] );
}
