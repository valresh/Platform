#include "stdafx.h"
#include "CtrlConn.h"
#include "Err.h"
#include <CommProc.h>
#include <Connect.h>
#include "SetData.h"
#define DLL_CTRL
#include "Trans.h"


//void * CTrans::Msg = NULL;
void * CTrans::CPP = NULL;


CTrans::CTrans()
  {
  memset ( this, 0, sizeof ( CTrans ));
  }

bool CTrans::Int( int & I )
  {
  if ( Pos >= Len )
    throw "EOF";
  char s = Str[Pos];
  if ( s < '0' || s > '9' )
    return false;
  I = 0;
  while ( s >= '0' && s <= '9' )
    {
    I *= 10;
    I += s - '0';
    Pos++;
    s = Str[Pos];
    }
  return true;
  }

bool CTrans::Drob( double & D )
  {
  if ( Pos >= Len )
    throw "EOF";
  char s = Str[Pos];
  if ( s < '0' || s > '9' )
    return false;
  double d = 0.;
  double M = 1.;
  while ( s >= '0' && s <= '9' )
    {
    M *= 0.1;
    d += ( s - '0' ) * M;
    Pos++;
    s = Str[Pos];
    }
  D = d;
  return true;
  }

bool CTrans::Hex( DWORD & I )
  {
  if ( Pos >= Len )
    throw "EOF";
  char s = Str[Pos];
  bool H = false;
  if ( s >= '0' && s <= '9' )
    H = true;
  if ( s >= 'A' && s <= 'F' )
    H = true;
  if ( !H )
    return false;
  I = 0;
  while ( H )
    {
    I *= 16;
    if ( s >= '0' && s <= '9' )
      I += s - '0';
    else
      I += s - 'A' + 10;
    Pos++;
    s = Str[Pos];
    H = false;
    if ( s >= '0' && s <= '9' )
      H = true;
    if ( s >= 'A' && s <= 'F' )
      H = true;
    }
  return true;
  }

char * SyntSymb = ":()+-=><*/,;^?&%!~";

bool CTrans::Symb( char & S )
  {
  if ( Pos >= Len )
    throw "EOF";
  char s1 = Str[Pos];
  if ( s1 == 0 )
    return false;
  if ( strchr ( SyntSymb, s1 ) == NULL )
    return false;
  if ( CanMin && s1 == '-' )
    {
    ASS(0)
    char s2 = Str[Pos+1];
    if ( s2 >= '0' && s2 <= '9' )
      return false;
    S = '#';
    return true;
    }
  Pos++; 
  char s2 = Str[Pos];
  if ( s1 == ':' && s2 == '=' )
    {
    S = ':';
    Pos++; 
    CanNumb = true;  //-0.  после :=
    CanMin = true;
    return true;
    }
  if ( s1 == ':' )
    {
    ASS(0)
    S = 'ы';
    CanNumb = true;  //-0.  :
    CanMin = true;
    return true;
    }
  if ( s1 == '*' && s2 == '*' )
    {
    S = '^';
    Pos++; 
    CanNumb = true;  //-0.  **
    CanMin = false;
    return true;
    }
  if ( s1 == '=' && s2 == '=' )
    {
    S = EQ;
    Pos++; 
    CanNumb = true;  //-0.  после ==
    CanMin = true;
    return true;
    }
  if ( s1 == '<' && s2 == '>' )
    {
    S = NE;
    Pos++; 
    CanNumb = true;  //-0.  после :=
    CanMin = true;
    return true;
    }
  if ( s1 == '!' && s2 == '=' )
    {
    S = NE;
    Pos++; 
    CanNumb = true;  //-0.  после :=
    CanMin = true;
    return true;
    }
  if ( s1 == '>' && s2 == '=' )
    {
    S = GE;
    Pos++; 
    CanNumb = true;  //-0.  после :=
    CanMin = true;
    return true;
    }
  if ( s1 == '<' && s2 == '=' )
    {
    S = LE;
    Pos++; 
    CanNumb = true;  //-0.  после :=
    CanMin = true;
    return true;
    }
  S = s1;
  return true;
  }

bool CTrans::Numb( char & Type, int & iVal, double & dVal )
  {
  if ( Pos >= Len )
    throw "EOF";
  // Константы
  int Pos0 = Pos;
  char s = Str[Pos];
  bool Minus = false;
  int M = 0;
  bool IsM = false;
  switch ( s ) // 1-ый символ
    {
    case '`':
      if ( Str[Pos+1] == '-' )
        {
        Minus = true;
        Pos += 2;
        }
    break;
    case '+':
      {
      Pos = Pos0;
      return false;
      }
    break;
    case '-':
      {
      if ( !CanMin )
        {
        Pos = Pos0;
        return false;
        }
      char s = Str[Pos+1];
      if ( s >= '0' && s <= '9' )
        {
        Minus = true;
        Pos++;
        }
      else
        {
        Pos = Pos0;
        return false;
        }
      }
    break;
    }
  s = Str[Pos];
  double D = 0;
  bool IsD = false;
  if ( s == '.' ) // Сразу '.'
    {
    Pos++;
    bool Res = Drob( D );
    if ( !Res )
      {
      Pos = Pos0;
      return false;
      }
    s = Str[Pos];
    IsM = true;
    IsD = true;
    }
  if ( !IsM )
    {
    bool Res = Int( M );
    if ( !Res )
      {
      Pos = Pos0;
      return false;
      }
    IsM = true;
    }
  s = Str[Pos];
  if ( !IsD && s == '.' )
    {
    Pos++;
    bool Res = Drob( D );
    if ( !Res )
      D = 0.;
    IsD = true;
    }
  bool MinusE = false;
  int E = 0;
  bool IsE = false;
  s = Str[Pos];
  if ( s == 'E' )
    {
    Pos++;
    s = Str[Pos];
    switch ( s )
      {
      case '-':
        MinusE = true;
      case '+':
      Pos++;
      s = Str[Pos];
      }
    bool Res = Int( E );
    if ( !Res )
      {
      Pos = Pos0;
      return false;
      }
    IsE = true;
    }
  if ( IsE || IsD )
    {
// Double
    double V = M;
    if ( IsD )
      V += D;
    if ( IsE )
      {
      if ( MinusE )
        E = -E;
      if ( E > 0 )
        {
        for ( int n = 0; n < E; n++ )
          V *= 10.;
        }
      if ( E < 0 )
        {
        int aE = -E;
        for ( int n = 0; n < aE; n++ )
          V *= 0.1;
        }
      }
    if ( Minus )
      V = -V;
    Type = 'A';
    dVal = V;
    }
  else
    {
    if ( !IsM )
      {
      Pos = Pos0;
      return false;
      }
// Int
    if ( Minus )
      M = -M;
    Type = 'D';
    iVal = M;
    return true;
    }
////////////////////////////////////////////
// *10**N
  DelBlank( );
  Pos0 = Pos;
  s = Str[Pos];
  if ( s != '*' )
    return true;
  Pos++;
  double Mult;
  if ( Pow10 ( Mult ))
    dVal *= Mult;
  else
    Pos = Pos0;
////////////////////////////////////////////
  DelBlank( );
  Pos0 = Pos;
  s = Str[Pos];
  if ( s != '*' )
    return true;
  Pos++;
  if ( Pow100 ( Mult ))
    dVal *= Mult;
  else
    Pos = Pos0;
////////////////////////////////////////////
  return true;
  }

bool CTrans::Pow10 ( double & M )
  {
  DelBlank( );
  if ( Str[Pos] != '1' || Str[Pos+1] != '0' )
    return false;
  Pos += 2;
  if ( Str[Pos] != '*' || Str[Pos+1] != '*' )
    return false;
  Pos += 2;
  bool Sk = false;
  if ( Str[Pos] == '(' )
    {
    Sk = true;
    Pos++;
    }
  bool Min = false;
  if ( Str[Pos] == '-' )
    {
    Min = true;
    Pos++;
    }
  int E;
  bool Res = Int( E );
  ASS(Res)
  DelBlank( );
  if ( Sk )
    {
    ASS( Str[Pos] == ')' )
    Pos++;
    }
  double P = 1.;
  if ( Min )
    {
    for ( int n = 0; n < E; n++ )
      P *= 0.1;
    }
  else
    {
    for ( int n = 0; n < E; n++ )
      P *= 10.;
    }
  M = P;
  return true;
  }

bool CTrans::Pow100 ( double & M )
  {
  DelBlank( );
  if ( Str[Pos] != '1' || Str[Pos+1] != '0' || Str[Pos+2] != '0' )
    return false;
  Pos += 3;
  if ( Str[Pos] != '*' || Str[Pos+1] != '*' )
    return false;
  Pos += 2;
  bool Sk = false;
  if ( Str[Pos] == '(' )
    {
    Sk = true;
    Pos++;
    }
  bool Min = false;
  if ( Str[Pos] == '-' )
    {
    Min = true;
    Pos++;
    }
  int E;
  bool Res = Int( E );
  ASS(Res)
  DelBlank( );
  if ( Sk )
    {
    ASS( Str[Pos] == ')' )
    Pos++;
    }
  double P = 1.;
  if ( Min )
    {
    for ( int n = 0; n < E; n++ )
      P *= 0.01;
    }
  else
    {
    for ( int n = 0; n < E; n++ )
      P *= 100.;
    }
  M = P;
  return true;
  }

bool CTrans::Identificator( char Name[64] )
  {
  if ( Pos >= Len )
    throw "EOF";
  char s = Str[Pos];
  bool ID = false;
  if ( s >= 'A' && s <= 'Z' )
    ID = true;
  if ( s == 'X' || 'x'==s )
    ID = true;
  if ( !ID )
    return false;
/**/
  int L = 0;
  while ( L < 64 )
    {
    Name[L++] = s;
    Pos++;
    s = Str[Pos];
    ID = false;
    if ( s >= 'A' && s <= 'Z' )
      ID = true;
    if ( !ID && s >= '0' && s <= '9' )
      ID = true;
    if ( !ID && ( s == '%' || s == '_' || s == '$' ))
      ID = true;
    if ( !ID )
      break;
    }
  if ( L >= 64 )
    throw "Длинный идентификатор";
  Name[L] = 0;
  return true;
  }

/*
bool CTrans::Reference( char Name[64] )
  {
  if ( Pos >= Len )
    throw "EOF";
  char s = Str[Pos];
  if ( s != '\'' )
    return false;
//............................................
  Pos++;
  int L = 0;
  while ( L < 64 )
    {
    s = Str[Pos++];
    if ( s == '\'' )
      break;
    Name[L++] = s;
    }
  if ( L >= 64 )
    {
    throw "Длинная ссылка";
    }
  Name[L] = 0;
  if ( Trace >= 10 )
    Fprintf ( Msg, "[%s]-Ссылка\n", Name );
  return true;
  }
*/
void CTrans::DelBlank( )
  {
  while ( Str[Pos] == ' ' )
    Pos++;
  if ( Pos >= Len )
    throw "EOF";
  }

void CTrans::TestOP( char OP )
  {
  switch ( OP )
    {
    case ':': 
      {
      if ( PosTestSt != 2 )
        {
        PosTestSt = 0;
        break;
        }
      char C = TestStack[PosTestSt-1];
      ASS( C == 'V' || C == 'B' )
        C = TestStack[PosTestSt-2];
      ASS( C == 'R' )
        PosTestSt = 0;
      }
      break;
    case '@': //NOT
    case '~':
    case '#': //-
      {
      ASS( PosTestSt >= 1 )
        char C = TestStack[PosTestSt-1];
      }
      break;
    case EQ:
    case NE:
    case GE:
    case LE:
    case '=':
    case '<':
    case '>':
      {
      ASS( PosTestSt >= 2 )
        char C1 = TestStack[PosTestSt-1];
      char C2 = TestStack[PosTestSt-2];
      //??      ASS( C1 == 'V' && C2 == 'V' )
      PosTestSt--;
      TestStack[PosTestSt-1] = 'B';
      }
      break;
    case '+':
    case '-':
    case '*':
    case '%':
    case '/':
    case '^':
      {
      ASS( PosTestSt >= 2 )
      char C1 = TestStack[PosTestSt-1];
      char C2 = TestStack[PosTestSt-2];
      //      ASS( C1 == 'V' && C2 == 'V' )
      PosTestSt--;
      TestStack[PosTestSt-1] = 'V';
      }
      break;
    case '|':
    case '&': // Bit AND
    case '$': // AND
      {
      ASS( PosTestSt >= 2 )
        char C1 = TestStack[PosTestSt-1];
      char C2 = TestStack[PosTestSt-2];
      ASS( ( C1 == 'B' || C1 == 'V' ) && ( C2 == 'B' || C2 == 'V' ))
        PosTestSt--;
      TestStack[PosTestSt-1] = 'B';
      }
      break;
    case 'F':
      {
      switch ( nFunction[kFunction-1] )
        {
        case 2: //"SHR"
          {
          // 2arg -> 1res
          ASS( PosTestSt >= 2 )
            PosTestSt--;
          TestStack[PosTestSt-1] = 'B';
          }
          break;
        case 3: //"TRUNC"
        case 4: //"LOGEVENT"
          {
          // 1arg -> 1res            
          ASS( PosTestSt >= 1 )
            TestStack[PosTestSt-1] = 'V';
          }
          break;
        case 5: //EXP
          // 1arg -> 1res            
          ASS( PosTestSt >= 1 )
            TestStack[PosTestSt-1] = 'V';
          break;
        case 6: //ASIN
          // 1arg -> 1res            
          ASS( PosTestSt >= 1 )
            TestStack[PosTestSt-1] = 'V';
          break;
        case 7: //MIN
          // 2arg -> 1res
          ASS( PosTestSt >= 2 )
            PosTestSt--;
          TestStack[PosTestSt-1] = 'V';
          break;
        case 8: //MAX
          // 2arg -> 1res
          ASS( PosTestSt >= 2 )
            PosTestSt--;
          TestStack[PosTestSt-1] = 'V';
          break;
        case 9: //TIME
          // 1arg -> 1res            
          ASS( PosTestSt >= 1 )
            TestStack[PosTestSt-1] = 'V';
          break;
        case 10: //TIME_TO_STR
          // 2arg -> 1res
          ASS( PosTestSt >= 2 )
            PosTestSt--;
          TestStack[PosTestSt-1] = 'V';
          break;
        default:
          break;
        }
      }
      break;
    default:
      ASS(FALSE)
        break;
    }
  }

void CTrans::Logical( )
  {
// Логическое выражение
	int Level = 1;
  char S;
	while ( Pos < Len )
    {
    DelBlank( );
    CanNumb = false;
    if ( Str[Pos] == '-' && CanMin )
      {
      if ( Numb( ))
        continue;
      }
    if ( Symb( S ))
      {
      if ( S == '?' )
        {
        //...............................
        while ( PosSt > 1 )
          {
          char C = Stack[PosSt-1];
          if ( C == '(' )
            {
            Level--;
            PosSt--;
            break;
            }
          PosSt--;
          Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
          SetOP( C );
          TestOP( C );
          }
        return;
        }
      LogicOP( S );
			if ( S == '(' )
        {
        CanMin = true;
				Level++;
        }
			if ( S == ')' )
        {
        CanMin = false;
				Level--;
        }
			if ( Level <= 0 )
				return;
      continue;
      }
		if ( Level <= 0 )
			throw "Неправильная вложенность скобок";
    char Name[64];
//    if ( Reference( Name ))
//      {
//      UniMin = false;
//bool EnumConst( int & iVar, char * Name );
//      if ( EnumConst( iVar, Name ))
//        {
//        TestStack[PosTestSt++] = 'V'; 
//        int N = AddVar( "ConstD", CVar::Const, eint, 0., iVar );
//        Var[N].iVal = iVar;
//        Var[N].piVal = &Var[N].iVal;
//        Fprintf ( CPP, "%d/%d)OP_LoadVar [enum %d(%s)]\n", kOP, PosSt, iVar, Name );
//        OP[kOP++] = OP_LoadVar;
//        OP[kOP++] = N;
//        UniMin = false;
//        continue;
//        }
//      CVar::eType Type = CVar::Reference;
//      char * pREF = strstr ( Name, ".$REF" );
//      if ( pREF )
//        Type = CVar::$REF;
//      int N = AddVar( Name, Type, eRef, 0., 0 );
//      Fprintf ( CPP, "%d/%d)OP_LoadVar [ссылка'%s']\n", kOP, PosSt, Name );
//      OP[kOP++] = OP_LoadVar;
//      OP[kOP++] = N;
//      TestStack[PosTestSt++] = 'V'; 
//      UniMin = false;
//      continue;
//      }
    if ( Identificator( Name ))
      {
      CanMin = false;
      int nF = IsFunc( Name );
      if ( nF >= 0 )
        {
        nFunction[kFunction++] = nF;
        LogicOP( 'F' );
        continue;
        }
// только X			int N = FindVar( Name, CVar::Var );
      ASS( Name[0] == 'X' || Name[0] == 'x' )
      Fprintf ( CPP, "%d/%d)OP_LoadVar ['%s']\n", kOP, PosSt, Name );
      OP[kOP++].Type = COP::Load_X;
      TestStack[PosTestSt++] = 'V'; 
      continue;
      }
    if ( Numb( ))
      continue;
    throw "Неправильный операнд";
    }
//...............................
  while ( PosSt > 0 )
    {
    char C = Stack[PosSt-1];
    PosSt--;
    Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
    SetOP( C );
    TestOP( C );
    }
//  ASS( PosTestSt == 1 && ( TestStack[0] == 'B' || TestStack[0] == 'V' ));
  }

int PRIOR( char AOP )
  {
  switch ( AOP )
    {
    case ':':
    return 0;
    case '+':
    case '-':
    return 1;
    case '*':
    case '/':
    case '%':
    return 2;
    case '^':
    case '#':
    return 3;
    case '|':
    case '&': // Bit AND
    case '$': // AND
    return 4;
    case '@': //NOT
    return 5;
    case EQ:
    case NE:
    case GE:
    case LE:
    case '=':
    case '<':
    case '>':
    case '~':
    return 6;
    case 'F': //Функция
    return 7;
    }
  ASS(FALSE)
  return 0;
  }

bool IsLogic( char AOP )
  {
  if ( AOP == '(' )
    return false;
  return true;
  }

// ( ) + - * / 

void CTrans::LogicOP( char AOP )
  {
  switch ( AOP )
    {
    case 'F': //Ф-ия
    case '@': //NOT
    case '(':
    case '~':
      {
// Если '(' - поместить в стек трансляции  
      if ( PosSt >= MAX_ST )
        throw "Стек мал";
      Stack[PosSt++] = AOP;
      }
    break;
    case ')':
      {
// Если ')' - Все из стека трансляции до '('
      while ( PosSt > 0 )
        {
        char C = Stack[PosSt-1];
        PosSt--;
        if ( C == '(' )
          break;
        Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
        SetOP( C );
        TestOP( C );
        }
      if ( PosSt > 0 )
        {
        char C = Stack[PosSt-1];
        if ( C == 'F' )
          {
          TestOP( C );
          Fprintf ( CPP, "%d/%d)OP_Function['%d']\n", kOP, PosSt, nFunction[kFunction-1] );
          OP[kOP++].Type = COP::Func;
          ASS( nFunction[kFunction-1] >= 0 )
          ASS(kFunction>0)
          OP[kOP].I = nFunction[--kFunction];
          PosSt--;
          }
        }
      }
    break;
    case EQ:
    case NE:
    case GE:
    case LE:
    case '+':
    case '-':
    case '*':
    case '%':
    case '^':
    case '/':
    case '=':
    case '<':
    case '>':
    case '|':
    case '&': // Bit AND
    case '$': // AND
      {
      CanMin = true;
      int Prty = PRIOR( AOP );
      while ( PosSt > 0 )
        {
        char C = Stack[PosSt-1];
        if ( !IsLogic( C ) )
          break;
        int P = PRIOR( C );
        if ( Prty > P )
          break;
        Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
        SetOP( C );
        TestOP( C );
        PosSt--;
        }
      if ( PosSt > MAX_ST )
        throw "Много переменных";
      Stack[PosSt++] = AOP;
      }
    break;
    case ',':
      {
// Разделитель аргументов ф-ии
      while ( PosSt > 0 )
        {
        char C = Stack[PosSt-1];
        if ( C == '(' )
          break;
        PosSt--;
        Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
        SetOP( C );
        TestOP( C );
        }
      }
    }
  }

static char * Func[] = {
  "ABS",    // 0
  "SQRT",   // 1
  "EXP",    // 2
  "LEV",    // 3
  "MIN",    // 4
  "MAX",    // 5
  "RND",    // 6
  NULL,
  };


int CTrans::IsFunc( char * Name )
  {
  int n = 0;
  while ( Func[n] )
    {
    if ( lstrcmp ( Func[n], Name ) == 0 )
      return n;
    n++;
    }
  return -1;
  }

bool IsArith( char AOP )
  {
  if ( AOP == '(' )
    return false;
  return true;
  }

void CTrans::ArithOP( char AOP )
  {
  switch ( AOP )
    {
    case 'F': //Ф-ия
    case ':':
    case '(':
    case '~':
      {
      // Если '(' - поместить в стек трансляции  
      if ( PosSt >= MAX_ST )
        throw "Стек мал";
      Stack[PosSt++] = AOP;
      if ( AOP == '(' )
        CanMin = true;
      }
      break;
    case ')':
      {
      // Если ')' - Все из стека трансляции до '('
      while ( PosSt > 0 )
        {
        char C = Stack[PosSt-1];
        PosSt--;
        if ( C == '(' )
          break;
        Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
        SetOP( C );
        TestOP( C );
        }
      if ( PosSt > 0 )
        {
        char C = Stack[PosSt-1];
        if ( C == 'F' )
          {
          TestOP( C );
          Fprintf ( CPP, "%d/%d)OP_Func['%d']\n", kOP, PosSt, nFunction[kFunction-1] );
          OP[kOP].Type = COP::Func;
          ASS( kFunction > 0 )
          OP[kOP].I = nFunction[--kFunction];
          kOP++;
          PosSt--;
          }
        }
      }
      break;
    case '+':
    case '-':
    case '*':
    case '%':
    case '^':
    case '&':
    case '/':
      //
    case '<':
    case '>':
    case '=':
    case GE:
    case NE:
    case LE:
      {
      if ( AOP == '-' && CanMin )
        {
        AOP = '#';
        }
      CanMin = false;
      int Prty = PRIOR( AOP );
      while ( PosSt > 0 )
        {
        char C = Stack[PosSt-1];
        if ( !IsArith( C ) )
          break;
        int P = PRIOR( C );
        if ( Prty > P )
          break;
        Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
        SetOP( C );
        TestOP( C );
        PosSt--;
        }
      if ( PosSt > MAX_ST )
        throw "Много переменных";
      Stack[PosSt++] = AOP;
      }
      break;
    default:
      ASS(FALSE);
      break;
    case ',':
      {
      // Разделитель аргументов ф-ии
      while ( PosSt > 0 )
        {
        char C = Stack[PosSt-1];
        if ( C == '(' )
          break;
        PosSt--;
        Fprintf ( CPP, "%d/%d)OP_Operation ['%c']\n", kOP, PosSt, C );
        SetOP( C );
        TestOP( C );
        }
      }
      break;
    }
  }

void CTrans::SetOP( char C )
  {
  ASS( kOP < MAX_OP )
  if ( kOP >= MAX_OP )
    return;
  OP[kOP].Type = COP::Operation;
  OP[kOP].OP = C;
  kOP++;
  }
