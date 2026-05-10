#include "QbTranslator.h"
#include <rsuIsType.h>

bool KQbTranslator::AnalizeTok( eToken_value &tok, std::string &szVal )
{
  switch( tok )
  {
  default:
    ASSD(0);
    break;
  case etEndInData:
    while( !m_Stack.empty() )
    {
      eToken_value C = m_Stack.top();
      m_Stack.pop();
      AddToOP( OP_Operation, C );
    }
  case etEndLine:
    return false;
  case etNUMBER:
    {
      int N = -1;
      if( ::IsInt( szVal.c_str() ) )
      {
        szVal.append( "(numb)" );
        N = AddVar( szVal.c_str(), KVar::ConstInteger );
      }
      else if( ::IsFlt( szVal.c_str() ) )
      {
        szVal.append( "(numb)" );
        N = AddVar( szVal.c_str(), KVar::ConstFloat );
      }
      else
      {
        ASSD(0);
      }
      AddToOP( OP_LoadVar, N );
    }
    break;
  case etASSIGN:
    m_Stack.push( tok );
    break;
  case etNOT:
  case etBITAND:
  case etBITOR:
  case etAND:
  case etOR:
  case etPLUS:
  case etMINUS:
  case etMUL:
  case etDIV:
  case etGT:
  case etLT:
  case etGE:
  case etLE:
  case etEQ:
  case etNE:
    {
      int Prty = Prior( tok );
      while( !m_Stack.empty() )
      {
        eToken_value C = m_Stack.top();
        if( !IsArith( C ) )
          break;
        int P = Prior( C );
        if ( Prty > P )
          break;
        AddToOP( OP_Operation, C);
        m_Stack.pop();
      }
      m_Stack.push( tok );
    }
    break;
  case etRealVarField:
    KKK();
  case etVarFieldAsTxt:
    {
      LPCSTR pszName = szVal.c_str();
      int N = AddVar( pszName, etVarFieldAsTxt==tok ? KVar::VarFieldText : KVar::Ref2FieldVar );
      ASSD( N>=0 );
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
  case etCOMMA:
    while( !m_Stack.empty() )
    {
      eToken_value C = m_Stack.top();
      if( etLP==C )
        break;
      m_Stack.pop();
      AddToOP( OP_Operation, C );
    }
    break;
  case etNEGATIVE:
  case etLP:
    m_Stack.push( tok );
    break;
  case etRP:
    while( !m_Stack.empty() )
    {
      eToken_value C = m_Stack.top();
      if( etLP==C )
      {
        m_Stack.pop();
        if( !m_Stack.empty() )
        {
          eToken_value test4IF = m_Stack.top();
          if( etIF!=test4IF )
            break;
          ASSD(0);
        }
        break;
      }
      m_Stack.pop();
      AddToOP( OP_Operation, C );
    }
    if( !m_Stack.empty() )
    {
      eToken_value C = m_Stack.top();
      if( etFUNCTION_HardCoded==C )
      {
        AddToOP( OP_Function, m_Functions.top() );
        m_Stack.pop();
        m_Functions.pop();
      }
    }
    break;
  case etFUNCTION_HardCoded:
    {
      int N = IsFunc( szVal.c_str() );
      ASSD( N>=0 );
      m_Functions.push( (eFunctions)N );
      m_Stack.push( etFUNCTION_HardCoded );
    }
    break;
  case etIF:
    {
      m_bIsLogicOperation = TRUE;
      Parm.ifOpenThenClose++;

      if( etELSE!=Parm.tokAfterAnalize )
        Parm.LevIF++;

      Parm.StartOperator = 0;
      NextTokens();

      ASSD( Parm.LevIF < _countof(Parm.AddrIfElseIfOnLevel) );
      ASSD( Parm.AddrIfElseIfOnLevel[Parm.LevIF][ Parm.CountIfElseIf[Parm.LevIF] ] == 0 );
      int &_kOP = Parm.AddrIfElseIfOnLevel[Parm.LevIF][ Parm.CountIfElseIf[Parm.LevIF] ];
      Parm.CountIfElseIf[Parm.LevIF]++;
      _kOP = (int)m_OPs.size();
      AddToOP( OP_IF, 0, 0 ); // Переход по false, Переход на endif
    }
    break;
  case etELSE:
    {
      while( !m_Stack.empty() )
      {
        eToken_value C = m_Stack.top();
        AddToOP( OP_Operation, C );
        m_Stack.pop();
      }

      ASSD( Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1] != 0);//,"Неверный IF" );
      int A = Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1];
      AddToOP( OP_ELSE, A+2);
      ASSD( m_OPs[A+1] == 0 );
      m_OPs[A+1] = (int)m_OPs.size();// Переход по false
    }
    break;
  case etTHEN:
    if( tok==etTHEN )
    {
      Parm.ifOpenThenClose--;
      ASSD( Parm.ifOpenThenClose>=0 );
    }
  case etEND_IF:
    CloseIf( tok );
    if( tok==etTHEN )
      return false;
    break;
  }
  return true;
}

void KQbTranslator::CloseIf( eToken_value tok )
{
  while( !m_Stack.empty() )
  {
    eToken_value C = m_Stack.top();
    m_Stack.pop();
    AddToOP( OP_Operation, C );
  }
  if( etTHEN==tok)
  {
    Parm.StartOperator = true;
    return;
  }
  if( Parm.LevIF >=0 )
  {
    while( Parm.CountIfElseIf[Parm.LevIF] )
    {
      ASSD( Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1] != 0);//, "Неверный IF" );
      int A = Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1];
      Parm.AddrIfElseIfOnLevel[Parm.LevIF][Parm.CountIfElseIf[Parm.LevIF]-1] = 0;
      if( m_OPs[A+1] == 0 )
        m_OPs[A+1] = (int)m_OPs.size();
      ASSD( m_OPs[A+2] == 0 )
        m_OPs[A+2] = (int)m_OPs.size();// Переход на endif
      Parm.CountIfElseIf[Parm.LevIF]--;
    }
    Parm.LevIF--;
  }
  if( Parm.LevIF < 0 )
    Parm.Init();
}