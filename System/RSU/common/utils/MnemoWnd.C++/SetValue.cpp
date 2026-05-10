#include "Queue.h"
//#include "QueueAPM.h"
#include "Function.h"
#include <assert.h>
#include "Lang.h"
#include "FormatScn.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef void (*LPFunc)(Define);
struct STegs { char*  name;LPFunc func; };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SetSensor(Define) { def.AddKind( id_Sensor, value ); }
void SetZadv  (Define) { def.AddKind( id_Zadv  , value ); }
void SetKlapan(Define) { def.AddKind( id_Klapan, value ); }
void SetKVO   (Define) { def.AddKind( id_KVO   , value ); }
void SetPump  (Define) { def.AddKind( id_Pump  , value ); }
void SetQooler(Define) { def.AddKind( id_KVO   , value ); }
void SetHS    (Define) { def.AddKind( id_HS    , value ); }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#undef  DST
#define DST(VAL) \
  char* sa = value-1;\
  if ( *sa == '"' )\
    {\
    *def.szParam[VAL]='"';\
    StrCPY(def.szParam[VAL]+1,value,sizeof(def.szParam[VAL])-2);\
    lstrcat(def.szParam[VAL],"4");\
    def.szParam[VAL][lstrlen(def.szParam[VAL])-1] = '"';\
    }\
  else\
    {\
    StrCPY(def.szParam[VAL],value,sizeof(def.szParam[VAL]));\
    }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SetP0(Define){def.dwPar|=PAR0;DST(0);}
void SetP1(Define){def.dwPar|=PAR1;DST(1);}
void SetP2(Define){def.dwPar|=PAR2;DST(2);}
void SetP3(Define){def.dwPar|=PAR3;DST(3);}
void SetP4(Define){def.dwPar|=PAR4;DST(4);}
void SetP5(Define){def.dwPar|=PAR5;DST(5);}
void SetP6(Define){def.dwPar|=PAR6;DST(6);}
void SetP7(Define){def.dwPar|=PAR7;DST(7);}
void SetP8(Define){def.dwPar|=PAR8;DST(8);}
void SetP9(Define){def.dwPar|=PAR9;DST(9);}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#undef  DST
#define DST(VAL) StrCPY(def.szParam[VAL],value,sizeof(def.szParam[VAL]));
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SetPa0(Define){def.dwPar|=PAR0;DST(0);}
void SetPa1(Define){def.dwPar|=PAR1;DST(1);}
void SetPa2(Define){def.dwPar|=PAR2;DST(2);}
void SetPa3(Define){def.dwPar|=PAR3;DST(3);}
void SetPa4(Define){def.dwPar|=PAR4;DST(4);}
void SetPa5(Define){def.dwPar|=PAR5;DST(5);}
void SetPa6(Define){def.dwPar|=PAR6;DST(6);}
void SetPa7(Define){def.dwPar|=PAR7;DST(7);}
void SetPa8(Define){def.dwPar|=PAR8;DST(8);}
void SetPa9(Define){def.dwPar|=PAR9;DST(9);}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#undef  DST
#define DST(VAL) StrCPY(def.szParam[VAL],value,sizeof(def.szParam[VAL]));
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline void Tag( Define, int n )
  {
  char* szParam = strrchr( value, '.' );
  int nNumber = -1;
  if ( szParam )
    {
    *szParam++ = 0;
    nNumber = ::FindTag( value );
    }
  else
    {
    szParam = value;
    int nActive = def.GetActiveObj();
    nNumber = def.GetMnemoTags( nActive );
    }
  char* szValue = (char*)::TegValueS( nNumber, szParam, "0" );
  StrCPY(def.szParam[n],szValue,sizeof(def.szParam[n]));
  }
//
void SetTg0(Define){def.dwPar|=PAR0;Tag(def,value,0);}
void SetTg1(Define){def.dwPar|=PAR1;Tag(def,value,1);}
void SetTg2(Define){def.dwPar|=PAR2;Tag(def,value,2);}
void SetTg3(Define){def.dwPar|=PAR3;Tag(def,value,3);}
void SetTg4(Define){def.dwPar|=PAR4;Tag(def,value,4);}
void SetTg5(Define){def.dwPar|=PAR5;Tag(def,value,5);}
void SetTg6(Define){def.dwPar|=PAR6;Tag(def,value,6);}
void SetTg7(Define){def.dwPar|=PAR7;Tag(def,value,7);}
void SetTg8(Define){def.dwPar|=PAR8;Tag(def,value,8);}
void SetTg9(Define){def.dwPar|=PAR9;Tag(def,value,9);}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#undef  DST
#define DST(VAL)  gnRegistr[VAL] = Atoi(value);
#undef  DSF
#define DSF(VAL)  gnRegistf[VAL] = Atof(value);
extern int    gnRegistr[];
extern double gnRegistf[];
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SetR0(Define){DST(0);}void SetF0(Define){DSF(0);}
void SetR1(Define){DST(1);}void SetF1(Define){DSF(1);}
void SetR2(Define){DST(2);}void SetF2(Define){DSF(2);}
void SetR3(Define){DST(3);}void SetF3(Define){DSF(3);}
void SetR4(Define){DST(4);}void SetF4(Define){DSF(4);}
void SetR5(Define){DST(5);}void SetF5(Define){DSF(5);}
void SetR6(Define){DST(6);}void SetF6(Define){DSF(6);}
void SetR7(Define){DST(7);}void SetF7(Define){DSF(7);}
void SetR8(Define){DST(8);}void SetF8(Define){DSF(8);}
void SetR9(Define){DST(9);}void SetF9(Define){DSF(9);}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static STegs gs_funcTegs[] =
  {
  //
  {"hs"    , SetHS    },
  {"kvo"   , SetKVO   },
  {"klapan", SetKlapan},
  {"pump"  , SetPump  },
  {"qooler", SetQooler},
  {"sensor", SetSensor},
  {"zadv"  , SetZadv  },
  //
  {"p0",SetP0},{"param0",SetP0},{"par0",SetPa0},{"tag0",SetTg0},{"registr0",SetR0},{"registf0",SetF0},{"regI0",SetR0},{"regF0",SetF0},
  {"p1",SetP1},{"param1",SetP1},{"par1",SetPa1},{"tag1",SetTg1},{"registr1",SetR1},{"registf1",SetF1},{"regI1",SetR1},{"regF1",SetF1},
  {"p2",SetP2},{"param2",SetP2},{"par2",SetPa2},{"tag2",SetTg2},{"registr2",SetR2},{"registf2",SetF2},{"regI2",SetR2},{"regF2",SetF2},
  {"p3",SetP3},{"param3",SetP3},{"par3",SetPa3},{"tag3",SetTg3},{"registr3",SetR3},{"registf3",SetF3},{"regI3",SetR3},{"regF3",SetF3},
  {"p4",SetP4},{"param4",SetP4},{"par4",SetPa4},{"tag4",SetTg4},{"registr4",SetR4},{"registf4",SetF4},{"regI4",SetR4},{"regF4",SetF4},
  {"p5",SetP5},{"param5",SetP5},{"par5",SetPa5},{"tag5",SetTg5},{"registr5",SetR5},{"registf5",SetF5},{"regI5",SetR5},{"regF5",SetF5},
  {"p6",SetP6},{"param6",SetP6},{"par6",SetPa6},{"tag6",SetTg6},{"registr6",SetR6},{"registf6",SetF6},{"regI6",SetR6},{"regF6",SetF6},
  {"p7",SetP7},{"param7",SetP7},{"par7",SetPa7},{"tag7",SetTg7},{"registr7",SetR7},{"registf7",SetF7},{"regI7",SetR7},{"regF7",SetF7},
  {"p8",SetP8},{"param8",SetP8},{"par8",SetPa8},{"tag8",SetTg8},{"registr8",SetR8},{"registf8",SetF8},{"regI8",SetR8},{"regF8",SetF8},
  {"p9",SetP9},{"param9",SetP9},{"par9",SetPa9},{"tag9",SetTg9},{"registr9",SetR9},{"registf9",SetF9},{"regI9",SetR9},{"regF9",SetF9},
  //
  };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool FindTegs(CLang& def,char* name,char* value)
  {
  static int  count = sizeof(gs_funcTegs)/sizeof(gs_funcTegs[0]);
  static bool bInit = true;
  Qsort( gs_funcTegs, count, sizeof(STegs), bInit );
  STegs* find = (STegs*)SearchName( name, gs_funcTegs, count, sizeof(STegs) );
  if ( find ) (*find->func)(def,value);
  return find != NULL;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
