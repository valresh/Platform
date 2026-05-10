#include "stdafx.h"
#include "CtrlConn.h"
#include "Err.h"
#include <CommProc.h>
#include <Connect.h>
#include "SetData.h"
#define DLL_CTRL
#include "Trans.h"

//Err.Head();Fprintf( Err.F, ";;;Ошибка задания min, max\n" );
void Operation ( char CMD );
void Function ( int nF );

CTrans Trans;

bool CCtrlConn::Translate ( CConnect & Conn, char * Формула )
  {
  //if ( Conn.Type[1] != 'I' )
  //  {
  //    Err.Head();Fprintf( Err.F, ";;;Использование формул возможно только для 'DI', 'AI'\n" );
  //  return false;
  //  }
  char Txt[256];
  if ( strcpy_s ( Txt, Формула ))
    return false;
  Trans.Str = Txt;
  Trans.Init();
  if ( Conn.Type[0] == 'A' )
    Trans.SetAction();
  else
    Trans.SetCondition();
  if ( Trans.kOP <= 0 )
    return false;
  Conn.OP = NewArr(COP,Trans.kOP);
  Conn.kOP = Trans.kOP;
  memmove ( Conn.OP,Trans.OP, Trans.kOP * sizeof ( COP ));
  return true;
  }


void CTrans::Init( )
  {
  CLEAR(OP)
  kOP = 0;
  memset( Stack, 0xFF, sizeof( Stack ) );//CLEAR(Stack);
  PosSt = 0; 
  memset( TestStack, 0xFF, sizeof( TestStack ) );//CLEAR(TestStack)
  PosTestSt = 0; 
  memset( nFunction, 0xFF, sizeof ( nFunction ));
  kFunction = 0;
  Pos = 0;
  Len = strlen ( Str );
//  Fprintf ( CPP, "%d/%d)Text [%s]\n", kOP, PosSt, Str );
  }

bool CTrans::SetCondition()
  {
  Init( );
/////////
  Pos = 0;
  Len = strlen ( Str );
  try
    {
    Logical( );
    }
  catch ( char * ErrMsg )
    {
    if ( lstrcmp ( ErrMsg, "EOF" ) != 0 )
      {
      Err->Head();Fprintf( Err->F, ";;;%d Error:[%s]\n", Pos, ErrMsg );
      return false;
      }
    //////////////////////////////////////////////
    // Выгрузка стека трансляци
    while ( PosSt > 0 )
      {
      char C = Stack[PosSt-1];
      PosSt--;
      Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
      SetOP( C );
      TestOP( C );
      }
    return true;
    }
  return true;
  }

bool CTrans::SetAction()
  {
  Init( );
  try
    {
    ObrAction( );
    // Выгрузка стека трансляци
    while ( PosSt > 0 )
      {
      char C = Stack[PosSt-1];
      PosSt--;
      Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
      SetOP( C );
      TestOP( C );
      }
    }
  catch ( char * ErrMsg )
    {
    if ( lstrcmp ( ErrMsg, "EOF" ) != 0 )
      {
      Fprintf ( CPP, "%d/%d Error:[%s]\n", kOP, PosSt, ErrMsg );
      Err->Head();Fprintf( Err->F, ";;;%d Error:[%s]\n", Pos, ErrMsg );
      return false;
      }
    // Выгрузка стека трансляци
    while ( PosSt > 0 )
      {
      char C = Stack[PosSt-1];
      PosSt--;
      Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
      SetOP( C );
      TestOP( C );
      }
    //////////////////////////////////////////////
    return true;
    }
  return true;
  }


bool CTrans::Numb( )
  {
  char Type; 
  double dVar; 
  int iVar; 
  if ( Numb( Type, iVar, dVar ))
    {
    CanMin = false;
    TestStack[PosTestSt++] = 'V'; 
    if ( Type == 'D' && iVar == 1 )
      {
      Fprintf ( CPP, "%d/%d)Load_1 [1]\n", kOP, PosSt );
      OP[kOP].Type = COP::Load_1;
      OP[kOP].I = 0;
      kOP++;
      return true;
      }
    if ( Type == 'D' && iVar == 0 )
      {
      Fprintf ( CPP, "%d/%d)Load_0 [0]\n", kOP, PosSt );
      OP[kOP].Type = COP::Load_0;
      OP[kOP].I = 0;
      kOP++;
      return true;
      }
    if ( Type == 'D' )
      {
      OP[kOP].Type = COP::Load_I;
      OP[kOP].I = iVar;
      kOP++;
      Fprintf ( CPP, "%d/%d)Const [%d]\n", kOP, PosSt, iVar );
      }
    else
      {
      OP[kOP].Type = COP::Load_D;
      OP[kOP].D = dVar;
      kOP++;
      Fprintf ( CPP, "%d/%d)Const  [%4.2lf]\n", kOP, PosSt, dVar );
      }
    return true;
    }
  return false;
  }

void CTrans::ObrAction( )
  {
//  CTransParm Parm;
  CanNumb = true;
  while ( Pos < Len )
    {
    DelBlank( );
    char S;
    if ( Str[Pos] == '-' && CanMin )
      {
      if ( Numb( ))
        continue;
      }
    if ( Symb( S ))
      {
      CanNumb = false;
      if ( S == ';' )
        {
        while ( PosSt > 0 )
          {
          char C = Stack[PosSt-1];
          PosSt--;
          Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
          SetOP( C );
          TestOP( C );
          }
        continue;
        }
      ArithOP( S );
      continue;
      }
    CanNumb = false;
    char Name[64];
    CLEAR(Name)
/***
    if ( Reference( Name ))
      {
      bool EnumConst( int & iVar, char * Name );
      int iVar;
      if ( EnumConst( iVar, Name ))
        {
        TestStack[PosTestSt++] = 'V'; 
        int N = AddVar( "", CVar::Const, 'I', 0., iVar );
        Var[N].iVal = iVar;
        Var[N].piVal = &Var[N].iVal;
        Fprintf ( CPP, "%d/%d)OP_LoadVar [enum %d(%s)]\n", kOP, PosSt, iVar, Name );
        OP[kOP++] = OP_LoadVar;
        OP[kOP++] = N;
        UniMin = false;
        CanMin = false;
        continue;
        }
      CVar::eType Type = CVar::Reference;
      char * pREF = strstr ( Name, ".$REF" );
      if ( pREF )
        Type = CVar::$REF;
      int N = AddVar( Name, Type, eRef, 0., 0 );
      if ( Parm.StartOperator )
        {
        if ( strstr ( File, "INTERLOCK_" ) && strstr ( Name, "OUT2" ) )
          KKK();
        Fprintf ( CPP, "%d/%d)OP_Address [ссылка'%s']\n", kOP, PosSt, Name );
        TestStack[PosTestSt++] = 'R'; 
        OP[kOP++] = OP_AddrVar;
        OP[kOP++] = N;
        Parm.StartOperator = false;
        DelBlank( );
        char S;
        CanNumb = false;
        if ( !Symb( S ) || S != ':' )
          throw "Нет ':='";
        ArithOP( S );
        Test_QQ( Parm );
        }
      else
        {
        Fprintf ( CPP, "%d/%d)OP_LoadVar [ссылка'%s']\n", kOP, PosSt, Name );
        TestStack[PosTestSt++] = 'V'; 
        OP[kOP++] = OP_LoadVar;
        OP[kOP++] = N;
        UniMin = false;
        CanMin = false;
        }
      continue;
      }
*******/
    if ( Identificator( Name ))
      {
      CanMin = false;
      int nF = IsFunc( Name );
      if ( nF >= 0 )
        {
        nFunction[kFunction++] = nF;
        ArithOP( 'F' );
        CanMin = false;
        continue;
        }
      Fprintf ( CPP, "%d/%d)OP_LoadVar [переменная'%s']\n", kOP, PosSt, Name );
      TestStack[PosTestSt++] = 'V'; 
      OP[kOP].Type = COP::Load_X;
      OP[kOP].I = 0;
      kOP++;
      continue;
      }
    if ( Numb() )
      continue;
    throw "Ошибочный символ";
    }
  }

#define MAX_STACK 32
static CRes Stack[MAX_STACK];
static int kStack = 0;

bool WorkOP ( int kOP, COP OP[], CRes & X, CRes & Y )
  {
  ZeroMemory( Stack, sizeof(Stack) );
  kStack = 0;
  for ( int nOP = 0; nOP < kOP; nOP++ )
    {
    COP::eType CurrOP = OP[nOP].Type;
    switch ( CurrOP )
      {
      case COP::Load_X:
        {
        ASS( kStack < MAX_STACK )
        Stack[kStack++] = X;
        }
      break;
      case COP::Load_D:
        {
        ASS( kStack < MAX_STACK )
        Stack[kStack].Type = 'D';
        Stack[kStack].dVal = OP[nOP].D;
        kStack++;
        }
      break;
      case COP::Load_I:
        {
        ASS( kStack < MAX_STACK )
        Stack[kStack].Type = 'I';
        Stack[kStack].iVal = OP[nOP].I;
        kStack++;
        }
      break;
      case COP::Load_1:
        {
        ASS( kStack < MAX_STACK )
        Stack[kStack].Type = 'I';
        Stack[kStack].iVal = 1;
        kStack++;
        }
      break;
      case COP::Load_0:
        {
        ASS( kStack < MAX_STACK )
        Stack[kStack].Type = 'I';
        Stack[kStack].iVal = 0;
        kStack++;
        }
      break;
      case COP::Operation:
        {
        char CMD = OP[nOP].OP;
        Operation ( CMD );
        }
      break;
      case COP::Func:
        {
        int nF =  OP[nOP].I;
        Function ( nF );
        }
      break;
      default:
        ASS(FALSE)
      break;
      }
    }
  ASS ( kStack >= 0 && kStack <= 1 )
  if ( kStack == 1 )
    {
    CRes * pV1 = &Stack[0];
    memmove( &Y, pV1, sizeof ( Y ));
    kStack = 0;
    return true;
    }
  return false;
  }


void Operation ( char CMD )
  {
  switch ( CMD )
    {
    case ':': 
      {
      ASS( 0 )
      }
    break;
    case '~': //~
      {
      ASS( kStack >= 1 )
      CRes * pV1 = &Stack[kStack-1];
      int V = 0;
      if ( pV1->Type == 'I' )
        V = pV1->iVal;
      else
        V = (DWORD)pV1->dVal;
      V = ~V;
      pV1->iVal = V;
      pV1->Type = 'I';
      }
    break;
    case '&': // Bit AND
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      int V1; 
      if ( pV1->Type == 'I' )
        V1 = pV1->iVal;
      else
        V1 = (DWORD)pV1->dVal;
      int V2; 
      if ( pV2->Type == 'I' )
        V2 = pV2->iVal;
      else
        V2 = (DWORD)pV2->dVal;
      int V = V1 & V2;
      pV2->iVal = V;
      pV2->Type = 'I';
      kStack--;
      }
    break;
    case EQ:
    case '=':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      int R = -1;
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          R = pV2->dVal == pV1->dVal;
        else
          R = pV2->dVal == pV1->iVal;
        }
      else
        {                                 
        if ( pV1->Type == 'D' )
          R = pV2->iVal == pV1->dVal;
        else
          R = pV2->iVal == pV1->iVal;
        }
      pV2->Type = 'I';
      pV2->iVal = R;
      kStack--;
      }
    break;
    case NE:
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      int R = -1;
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          R = pV2->dVal != pV1->dVal;
        else
          R = pV2->dVal != pV1->iVal;
        }
      else
        {                                 
        if ( pV1->Type == 'D' )
          R = pV2->iVal != pV1->dVal;
        else
          R = pV2->iVal != pV1->iVal;
        }
      pV2->Type = 'I';
      pV2->iVal = R;
      kStack--;
      }
    break;
    case GE:
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      int R = -1;
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          R = pV2->dVal >= pV1->dVal;
        else
          R = pV2->dVal >= pV1->iVal;
        }
      else
        {                                 
        if ( pV1->Type == 'D' )
          R = pV2->iVal >= pV1->dVal;
        else
          R = pV2->iVal >= pV1->iVal;
        }
      pV2->Type = 'I';
      pV2->iVal = R;
      kStack--;
      }
    break;
    case LE:
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      int R = -1;
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          R = pV2->dVal <= pV1->dVal;
        else
          R = pV2->dVal <= pV1->iVal;
        }
      else
        {                                 
        if ( pV1->Type == 'D' )
          R = pV2->iVal <= pV1->dVal;
        else
          R = pV2->iVal <= pV1->iVal;
        }
      pV2->Type = 'I';
      pV2->iVal = R;
      kStack--;
      }
    break;
    case '<':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      int R = -1;
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          R = pV2->dVal < pV1->dVal;
        else
          R = pV2->dVal < pV1->iVal;
        }
      else
        {                                 
        if ( pV1->Type == 'D' )
          R = pV2->iVal < pV1->dVal;
        else
          R = pV2->iVal < pV1->iVal;
        }
      pV2->Type = 'I';
      pV2->iVal = R;
      kStack--;
      }
    break;
    case '>':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      int R = -1;
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          R = pV2->dVal > pV1->dVal;
        else
          R = pV2->dVal > pV1->iVal;
        }
      else
        {                                 
        if ( pV1->Type == 'D' )
          R = pV2->iVal > pV1->dVal;
        else
          R = pV2->iVal > pV1->iVal;
        }
      pV2->Type = 'I';
      pV2->iVal = R;
      kStack--;
      }
    break;
    case '+':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          pV2->dVal += pV1->dVal;
        else
          pV2->dVal += pV1->iVal;
        kStack--;
        break;
        }
      else
        {
        if ( pV1->Type == 'D' )
          {
          double R = pV2->iVal + pV1->dVal;
          pV2->dVal = R;
          pV2->Type = 'D';
          }
        else
          pV2->iVal += pV1->iVal;
        kStack--;
        break;
        }
      }
    break;
    case '-':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          pV2->dVal -= pV1->dVal;
        else
          pV2->dVal -= pV1->iVal;
        kStack--;
        break;
        }
      else
        {
        //      pV2->Type == 'I' )
        if ( pV1->Type == 'D' )
          {
          double R = pV2->iVal - pV1->dVal;
          pV2->dVal = R;
          pV2->Type = 'D';
          }
        else
          pV2->iVal -= pV1->iVal;
        kStack--;
        break;
        }
      }
    break;
    case '#':
      {
      ASS( kStack >= 1 )
      CRes * pV1 = &Stack[kStack-1];
      if ( pV1->Type == 'D' )
        pV1->dVal = -pV1->dVal;
      else
        pV1->iVal = -pV1->iVal;
      }
    break;
    case '*':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          pV2->dVal *= pV1->dVal;
        else
          pV2->dVal *= pV1->iVal;
        kStack--;
        break;
        }
      else
        {
        if ( pV1->Type == 'D' )
          {
          double R = pV2->iVal * pV1->dVal;
          pV2->dVal = R;
          pV2->Type = 'D';
          }
        else
          pV2->iVal *= pV1->iVal;
        kStack--;
        break;
        }
      }
    break;
    case '%':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      ASS ( pV1->Type == 'I' ) 
      if ( pV2->Type == 'I' )
        {
        pV2->iVal = pV2->iVal % pV1->iVal;
        }
      else
        {
        int S = (int)(pV2->dVal);
        pV2->dVal = S % pV1->iVal;
        }
      kStack--;
      }
    break;
    case '/':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          {
          double R = pV2->dVal; 
          if ( pV1->dVal != 0. )
            R /= pV1->dVal;
          pV2->dVal = R;
          }
        else
          {
          double R = pV2->dVal; 
          if ( pV1->iVal != 0 )
            R /= pV1->iVal;
          pV2->dVal = R;
          }
        kStack--;
        break;
        }
      else
        {
        //      pV2->Type == 'I' )
        if ( pV1->Type == 'D' )
          {
          double R = pV2->iVal; 
          if ( pV1->dVal != 0. )
            R /= pV1->dVal;
          else
            KKK();
          pV2->dVal = R;
          pV2->Type = 'D';
          }
        else
          {
          if ( pV1->iVal )
            pV2->iVal /= pV1->iVal;
          else
            KKK();
          }
        kStack--;
        break;
        }
      }
    break;
    case '^':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      if ( pV2->Type == 'D' )
        {
        if ( pV1->Type == 'D' )
          pV2->dVal = pow ( pV2->dVal, pV1->dVal );
        else
          pV2->dVal = pow ( pV2->dVal, pV1->iVal );
        kStack--;
        break;
        }
      else
        {
        //      pV2->Type == 'I' )
        double R = pV2->iVal;
        if ( pV1->Type == 'D' )
          pV2->dVal = pow ( R, pV1->dVal );
        else
          pV2->dVal = pow ( R, pV1->iVal );
        pV2->Type = 'D';
        kStack--;
        break;
        }
      }
    break;
    case '|':
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      int V1;
      if ( pV1->Type == 'I' )
        V1 = pV1->iVal;
      else
        V1 = (int)pV1->dVal;
      int V2;
      if ( pV2->Type == 'I' )
        V2 = pV2->iVal;
      else
        V2 = (int)pV2->dVal;
      int R = V1 || V2;
      if ( R )
        pV2->iVal = 1;
      else
        pV2->iVal = 0;
      kStack--;
      }
    break;
    case '$': // AND
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      int I1 = 0;
      int I2 = 0;
      if ( pV1->Type == 'I' )
        I1 = pV1->iVal;
      if ( pV1->Type == 'D' )
        I1 = (int)pV1->dVal;
      if ( pV2->Type == 'I' )
        I2 = pV2->iVal;
      if ( pV2->Type == 'D' )
        I2 = (int)pV2->dVal;
      int R = I1 && I2;
      pV2->Type = 'I';
      if ( R )
        pV2->iVal = 1;
      else
        pV2->iVal = 0;
      kStack--;
      }
    break;
    case '@':
      {
      ASS( kStack >= 1 )
      CRes * pV1 = &Stack[kStack-1];
      if ( pV1->Type == 'I' )
        {
        if ( pV1->iVal )
          pV1->iVal = 0;
        else
          pV1->iVal = 1;
        }
      else
        {
        if ( pV1->dVal )
          pV1->iVal = 0;
        else
          pV1->iVal = 1;
        pV1->Type = 'I';
        }
      }
    break;
    default:
      ASS(FALSE)
    break;
    }
  }
/*
  "ABS",    // 0
    "SQRT",   // 1
    "EXP",    // 2
    "LEV",    // 3
    "MIN",    // 4
    "MAX",    // 5
    "RND",    // 6
*/
void Function ( int nF )
  {
  switch ( nF )
    {
    case 0: //"ABS"
      {
      // 1arg -> 1res            
      ASS( kStack >= 1 )
      CRes * pV1 = &Stack[kStack-1];
      if ( pV1->Type == 'D' )
        {
        double V = pV1->dVal;
        if ( V < 0. )
          V = -V;
        pV1->dVal = V;
        break;
        }
      if ( pV1->Type == 'I' )
        {
        int V = pV1->iVal;
        if ( V < 0 )
          V = -V;
        pV1->iVal = V;
        break;
        }
      }
    break;
    case 1: //"SQRT"
      {
      // 1arg -> 1res            
      ASS( kStack >= 1 )
      CRes * pV1 = &Stack[kStack-1];
      if ( pV1->Type == 'D' )
        {
        double V = pV1->dVal;
        if ( V <= 0. )
          V = 0.;
        else
          V = sqrt ( V );
        pV1->dVal = V;
        break;
        }
      if ( pV1->Type == 'I' )
        {
        double V = pV1->iVal;
        if ( V < 0 )
          V = 0;
        else
          V = sqrt ( V );
        pV1->dVal = V;
        pV1->Type = 'D';
        break;
        }
      }
    break;
    case 2: //"EXP"
      {
      // 1arg -> 1res            
      ASS( kStack >= 1 )
      CRes * pV1 = &Stack[kStack-1];
      ASS ( pV1->Type == 'D' )
      pV1->dVal = exp ( pV1->dVal );
      }
    break;
    case 3: //"LEV"
      {
      // 3arg -> 1res            
      if( kStack < 3 )
        break;
      CRes * pV1 = &Stack[kStack-1]; //макс
      double Max = 90.;  
      if ( pV1->Type == 'I' )
        Max = pV1->iVal;
      if ( pV1->Type == 'D' )
        Max = pV1->dVal;
      CRes * pV2 = &Stack[kStack-2]; //мин
      double Min = 10.;  
      if ( pV2->Type == 'I' )
        Min = pV2->iVal;
      if ( pV2->Type == 'D' )
        Min = pV2->dVal;
      CRes * pV3 = &Stack[kStack-3]; //L
      double L = Min;  
      if ( pV3->Type == 'I' )
        L = pV3->iVal;
      if ( pV3->Type == 'D' )
        L = pV3->dVal;
      kStack -= 2;
      double U = ( L - Min ) / ( Max - Min );
      if ( U < 0. )
        U = 0.;
      if ( U > 1. )
        U = 1.;
      pV3->dVal = U * 100.;
      pV3->Type = 'D';
      }
    break;
    case 4: //"MIN"
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      switch ( pV1->Type )
        {
        case 'I':
          {
          switch( pV2->Type )
            {
            case 'I':
              {
              int R = pV1->iVal;
              if ( pV2->iVal < R )
                R = pV2->iVal;
              pV2->iVal = R;
              pV2->Type = 'I';
              }
              break;
            case 'D':
              {
              double R = pV1->iVal;
              if ( pV2->dVal < R )
                R = pV2->dVal;
              pV2->dVal = R;
              pV2->Type = 'D';
              }
            break;
            default:
              ASS(0)
            break;
            }
          kStack--;
          return;
          }
        break;
        case 'D':
          {
          switch( pV2->Type )
            {
            case 'I':
              {
              double R = pV1->dVal;
              if ( pV2->iVal < R )
                R = pV2->iVal;
              pV2->dVal = R;
              pV2->Type = 'D';
              }
              break;
            case 'D':
              {
              double R = pV1->dVal;
              if ( pV2->dVal < R )
                R = pV2->dVal;
              pV2->dVal = R;
              pV2->Type = 'D';
              }
              break;
            default:
              ASS(0)
            break;
            }
          kStack--;
          return;
          }
        break;
        default:
          ASS(0)
          kStack--;
        return;
        }
      ASS(0)
      }
    break;
    case 5: //"MAX"
      {
      ASS( kStack >= 2 )
      CRes * pV1 = &Stack[kStack-1];
      CRes * pV2 = &Stack[kStack-2];
      switch ( pV1->Type )
        {
        case 'I':
          {
          switch( pV2->Type )
            {
            case 'I':
              {
              int R = pV1->iVal;
              if ( pV2->iVal > R )
                R = pV2->iVal;
              pV2->iVal = R;
              pV2->Type = 'I';
              }
            break;
            case 'D':
              {
              double R = pV1->iVal;
              if ( pV2->dVal > R )
                R = pV2->dVal;
              pV2->dVal = R;
              pV2->Type = 'D';
              }
            break;
            default:
              ASS(0)
            break;
            }
          kStack--;
          return;
          }
        break;
        case 'D':
          {
          switch( pV2->Type )
            {
            case 'I':
              {
              double R = pV1->dVal;
              if ( pV2->iVal > R )
                R = pV2->iVal;
              pV2->dVal = R;
              pV2->Type = 'D';
              }
            break;
            case 'D':
              {
              double R = pV1->dVal;
              if ( pV2->dVal > R )
                R = pV2->dVal;
              pV2->dVal = R;
              pV2->Type = 'D';
              }
            break;
            default:
              ASS(0)
            break;
            }
          kStack--;
          return;
          }
          break;
        default:
          ASS(0)
        break;
        }
      int R = pV1->iVal >> pV2->iVal;
      pV2->iVal = R;
      kStack--;
      }
      break;
    case 6: //"RND"
      {
      // 1arg -> 1res            
      ASS( kStack >= 1 )
      CRes * pV1 = &Stack[kStack-1];
      double Delta;
      if ( pV1->Type == 'D' )
        Delta = pV1->dVal;
      else
        Delta = pV1->iVal;
      pV1->dVal = 1. + Delta * ( 1. - 2. * r01());
      pV1->Type = 'D';
      }
    break;
    default:
      ASS(0)
    break;
    }
  }
