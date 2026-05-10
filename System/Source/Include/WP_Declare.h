
#undef  Enumeration
#define Enumeration(Var,Desc,...) \
struct _##Var\
    {\
    BYTE V;\
    enum { __VA_ARGS__ };\
    void operator = ( int Z ) { V = Z; };\
    void operator = ( BYTE Z ) { V = Z; };\
    bool operator == ( int Z ) { return V == Z; };\
    void operator = ( _##Var Z ) { V = Z.V; };\
    operator int () { return V; };\
    operator BYTE () { return V; };\
    static LPCSTR GetVals() { return #__VA_ARGS__;}\
    };\
    _##Var Var;

#undef  EnumerationDecl
#define EnumerationDecl(Type,...) \
struct __##Type\
    {\
    BYTE V;\
    enum { __VA_ARGS__ };\
    void operator = ( int Z ) { V = Z; };\
    void operator = ( BYTE Z ) { V = Z; };\
    bool operator == ( int Z ) { return V == Z; };\
    void operator = ( __##Type Z ) { V = Z.V; };\
    operator int () { return V; };\
    operator BYTE () { return V; };\
    static LPCSTR GetVals() { return #__VA_ARGS__;}\
    };\

#undef  Enumeration_A
#define Enumeration_A(Var,Desc,Len,...) \
struct _##Var\
    {\
    BYTE V;\
    enum { __VA_ARGS__ };\
    void operator = ( int Z ) { V = Z; };\
    void operator = ( BYTE Z ) { V = Z; };\
    bool operator == ( int Z ) { return V == Z; };\
    void operator = ( _##Var Z ) { V = Z.V; };\
    operator int () { return V; };\
    operator BYTE () { return V; };\
    };\
    _##Var Var[Len+1];

#undef  Enumeration16
#define Enumeration16(Var,Desc,...) \
struct _##Var\
    {\
    USHORT V;\
    enum { __VA_ARGS__ };\
    void operator = ( int Z ) { V = Z; };\
    void operator = ( USHORT Z ) { V = Z; };\
    bool operator == ( int Z ) { return V == Z; };\
    void operator = ( _##Var Z ) { V = Z.V; };\
    operator int () { return V; };\
    operator USHORT () { return V; };\
    };\
    _##Var Var;

#undef Boolean
#define Boolean(Var,Desc,Def) BYTE Var;

#undef BooleanEx2
#define BooleanEx2(Var,Desc,Def,Name2) BYTE Var;

#undef Byte_A
#define Byte_A(Var,Desc,Len,Def) BYTE Var[Len+1];

#undef Boolean_A
#define Boolean_A(Var,Desc,Len,Def) BYTE Var[Len+1];

#undef Boolean_AA
#define Boolean_AA(Var,Desc,Len1,Len2,Def) BYTE Var[Len1+1][Len2+1];

#undef String_K
#define String_K(Var,Comm,Len) char Var[Len+1];

#undef String_A
#define String_A(Var,Comm,Len,Count) char Var[Count+1][Len+1];

#undef Integer
#define Integer(Var,Desc,Def) int Var;

#undef UInt64
#define UInt64(Var,Desc,Def) UINT64 Var;

#undef Integer_A
#define Integer_A(Var,Desc,Len,Def) int Var[Len+1];//т.к. в honeywell нумерация с 1

#undef Time
#define Time(Var,Desc,Def) time_t Var;

#undef Time_A
#define Time_A(Var,Desc,Len,Def) time_t Var[Len+1];

#undef Real
#define Real(Var,Desc,Def) double Var;

#undef Real_A
#define Real_A(Var,Desc,Len,Def) double Var[Len+1];

#undef StructRE
#define StructRE(Var,Desc,realVal, enumVal)\
struct _##Var\
    {\
    double realVal;\
    _##enumVal enumVal;\
    };\
    _##Var Var;

#undef StructRB
#define StructRB(Var,Desc,realVal, bVal)\
struct _##Var\
    {\
    double realVal;\
    BYTE bVal;\
    };\
    _##Var Var;

#undef StructBE
#define StructBE(Var,Desc,bVal, enumVal)\
struct _##Var\
    {\
    BYTE bVal;\
    _##enumVal enumVal;\
    };\
    _##Var Var;

#undef Struct3B
#define Struct3B(Var,Desc,Val1,Val2,Val3)\
struct _##Var\
    {\
    BYTE Val1;\
    BYTE Val2;\
    BYTE Val3;\
    };\
    _##Var Var;

#undef Struct4B
#define Struct4B(Var,Desc,Val1,Val2,Val3,Val4)\
struct _##Var\
    {\
    BYTE Val1;\
    BYTE Val2;\
    BYTE Val3;\
    BYTE Val4;\
    };\
    _##Var Var;

#undef Struct5B
#define Struct5B(Var,Desc,Val1,Val2,Val3,Val4,Val5)\
struct _##Var\
    {\
    BYTE Val1;\
    BYTE Val2;\
    BYTE Val3;\
    BYTE Val4;\
    BYTE Val5;\
    };\
    _##Var Var;

#undef Struct2B
#define Struct2B(Var,Desc,Val1,Val2)\
struct _##Var\
    {\
    BYTE Val1;\
    BYTE Val2;\
    };\
    _##Var Var;

#undef Struct_BEI
#define Struct_BEI(Var,ValB,tValE,ValE,ValI)\
struct _##Var\
    {\
    BYTE ValB;\
    __##tValE ValE;\
    int ValI;\
    };\
    _##Var Var;

#undef Struct_BEIII
#define Struct_BEIII(Var,ValB,tValE,ValE,ValI,ValI2,ValI3)\
struct _##Var\
    {\
    BYTE ValB;\
    __##tValE ValE;\
    int ValI;\
    int ValI2;\
    int ValI3;\
    };\
    _##Var Var;

#undef Struct_EIE
#define Struct_EIE(Var,tValB,ValB,ValI,tValE,ValE)\
struct _##Var\
    {\
    __##tValB ValB;\
    int ValI;\
    __##tValE ValE;\
    };\
    _##Var Var;

#undef Struct_EEEE
#define Struct_EEEE(Var,tValE1,ValE1,tValE2,ValE2,tValE3,ValE3,tValE4,ValE4)\
struct _##Var\
    {\
    __##tValE1 ValE1;\
    __##tValE2 ValE2;\
    __##tValE3 ValE3;\
    __##tValE4 ValE4;\
    };\
    _##Var Var;

#undef Struct_EI
#define Struct_EI(Var,tValB,ValB,ValI)\
struct _##Var\
    {\
    __##tValB ValB;\
    int ValI;\
    };\
    _##Var Var;

#undef Struct_EB
#define Struct_EB(Var,tValE,ValE,ValB)\
struct _##Var\
    {\
    __##tValE ValE;\
    BYTE ValB;\
    };\
    _##Var Var;

#undef Struct_REBEIIR
#define Struct_REBEIIR(Var,R1,tE2,E2,B3,tE4,E4,I5,I6,R7)\
struct _##Var\
    {\
    double R1;\
    __##tE2 E2;\
    BYTE B3;\
    __##tE4 E4;\
    int I5;\
    int I6;\
    double R7;\
    };\
    _##Var Var;

#undef Struct_REBEIIIR
#define Struct_REBEIIIR(Var,R1,tE2,E2,B3,tE4,E4,I5,I6,I7,R8)\
struct _##Var\
    {\
    double R1;\
    __##tE2 E2;\
    BYTE B3;\
    __##tE4 E4;\
    int I5;\
    int I6;\
    int I7;\
    double R8;\
    };\
    _##Var Var;

#undef Struct_EBEIII
#define Struct_EBEIII(Var,tE1,E1,B2,tE3,E3,I4,I5,I6)\
struct _##Var\
    {\
    __##tE1 E1;\
    BYTE B2;\
    __##tE3 E3;\
    int I4;\
    int I5;\
    int I6;\
    };\
    _##Var Var;

#undef Struct_IR
#define Struct_IR(Var,I1,R2,Def2)\
struct _##Var\
    {\
    int I1;\
    double R2;\
    };\
    _##Var Var;

#undef Struct_BEIR
#define Struct_BEIR(Var,B1,tE2,E2,I3,R4)\
struct _##Var\
    {\
    BYTE B1;\
    __##tE2 E2;\
    int I3;\
    double R4;\
    };\
    _##Var Var;

#undef Struct_BEIIa
#define Struct_BEIIa(Var,B1,tE2,E2,I3,I4arr,I4count)\
struct _##Var\
    {\
    BYTE B1;\
    __##tE2 E2;\
    int I3;\
    int I4arr[I4count+1];\
    };\
    _##Var Var;

#undef StructA_BBEI
#define StructA_BBEI(Var,Count,B1,B2,tE3,E3,I4)\
struct _##Var\
    {\
    BYTE B1[Count+1];\
    BYTE B2[Count+1];\
    __##tE3 E3[Count+1];\
    int I4[Count+1];\
    };\
    _##Var Var;

#undef Struct_BBEI
#define Struct_BBEI(Var,B1,B2,tE3,E3,I4)\
struct _##Var\
    {\
    BYTE B1;\
    BYTE B2;\
    __##tE3 E3;\
    int I4;\
    };\
    _##Var Var;

#undef Struct_BxIx
#define Struct_BxIx(Var,B1,B1c,I2,I2c)\
struct _##Var\
    {\
    BYTE B1[B1c+1];\
    int I2[I2c+1];\
    };\
    _##Var Var;

#undef Struct_BxRx
#define Struct_BxRx(Var,B1,B1c,D2,D2c)\
struct _##Var\
    {\
    BYTE B1[B1c+1];\
    double D2[D2c+1];\
    };\
    _##Var Var;

#undef Struct_R
#define Struct_R(Var,R2,Def2)\
struct _##Var\
    {\
    double R2;\
    };\
    _##Var Var;


#undef Struct_3R
#define Struct_3R(Var,Desc,Val1,Val2,Val3)\
struct _##Var\
    {\
    double Val1;\
    double Val2;\
    double Val3;\
    };\
    _##Var Var;

#undef Struct_3R_A
#define Struct_3R_A(Var,Desc,Len,Val1,Val2,Val3)\
struct _##Var\
    {\
    double Val1;\
    double Val2;\
    double Val3;\
    };\
    _##Var Var[Len+1];


#undef Struct_BERB
#define Struct_BERB(Var,B1,tE2,E2,R3,B4)\
struct _##Var\
    {\
    BYTE B1;\
    _##tE2 E2;\
    double R3;\
    int I4;\
    };\
    _##Var Var;

#undef Struct_BIRBSIR
#define Struct_BIRBSIR(Var,Desc,Val1,Val2,Val3,Val4,szVal5,lenV5,Val6,Val7)\
    struct _##Var\
    {\
    BYTE Val1;\
    int Val2;\
    double Val3;\
    BYTE Val4;\
    char szVal5[lenV5];\
    int Val6;\
    double Val7;\
    };\
    _##Var Var;

#undef Struct_ER
#define Struct_ER(Var,tE2,E2,R3)\
struct _##Var\
    {\
    __##tE2 E2;\
    double R3;\
    };\
    _##Var Var;

#undef Struct_RRE
#define Struct_RRE(Var,R1,R2,tE3,E3)\
struct _##Var\
    {\
    double R1;\
    double R2;\
    __##tE3 E3;\
    };\
    _##Var Var;

#undef Struct_BIRBR
#define Struct_BIRBR(Var,Desc,Val1,Val2,Val3,Val4,Val5)\
struct _##Var\
    {\
    BYTE Val1;\
    int Val2;\
    double Val3;\
    BYTE Val4;\
    double Val5;\
    };\
    _##Var Var;


#undef Struct_F32F64x32I32UI64
#define Struct_F32F64x32I32UI64(Var,Desc,Val1,Val2,Val3,Val4)\
struct _##Var\
    {\
    float Val1;\
    double Val2;\
    int Val3;\
    uint64_t Val4;\
    };\
    _##Var Var;


#undef StructA_SCM_TRANS_G
#define StructA_SCM_TRANS_G(Var,Count) \
struct _##Var\
  {\
  __ALGID ALGID;\
  int FIRSTCOND;\
  int NUMINPTS;\
  BYTE SO;\
  };\
  _##Var Var[Count+1];

#undef StructA_SCM_TRANS_C
#define StructA_SCM_TRANS_C(Var,Count) \
struct _##Var\
  {\
  BYTE BYPPERM;\
  __BYPREQ BYPREQ;\
  char szCONFIGDESC[39*4];\
  char szDESC[65*4];\
  char szEXPR[513*4];\
  BYTE FL;\
  __GATEASGN GATEASGN;\
  int INCONNSTS1;\
  int INCONNSTS2;\
  int INCONNSTS3;\
  int INCONNSTS4;\
  int INCONNSTS5;\
  int INCONNSTS6;\
  BYTE INVOKFL;\
  };\
  _##Var Var[Count+1];


#undef Struct_SCM_STEP_SC
#define Struct_SCM_STEP_SC(Var) \
struct _##Var\
  {\
  BYTE BYPPERM;\
  __BYPREQ BYPREQ;\
  char szCONFIGDESC[39*4];\
  char szDESC[65*4];\
  char szEXPR[513*4];\
  BYTE FL;\
  int INCONNSTS1;\
  int INCONNSTS2;\
  int INCONNSTS3;\
  int INCONNSTS4;\
  int INCONNSTS5;\
  int INCONNSTS6;\
  BYTE INVOKFL;\
  BYTE OPT;\
  };\
  _##Var Var;

#undef StructA_SCM_STEP_OP
#define StructA_SCM_STEP_OP(Var,Count) \
struct _##Var\
  {\
  __ACQSTATE ACQSTATE;\
  __BYPREQ BYPREQ;\
  int CMDREQSTS;\
  BYTE CONFIGASSOCPARAM;\
  char CONFIGDESC[39*4];\
  BYTE CURRVALTREND;\
  double DELAYTIME;\
  double DELAYTIMEREM;\
  char DESC[65*4];\
  int EQUIPNAME;\
  char EXECDESC[39*4];\
  __EXECSTS EXECSTS;\
  char EXPRPCODE[218*4];\
  BYTE FILTERABLE;\
  BYTE HIDETBLVIEW;\
  int INCONNSTS1;\
  int INCONNSTS2;\
  int INCONNSTS3;\
  int INCONNSTS4;\
  int INCONNSTS5;\
  int INCONNSTS6;\
  BYTE INSTRUCTCNFM;\
  BYTE INSTRUCTCOMP;\
  BYTE INSTRUCTPEND;\
  __INSTRUCTTYPE INSTRUCTTYPE;\
  int OWNERSSTS;\
  BYTE OWNFL;\
  BYTE RELFL;\
  BYTE REQFL;\
  char ROLE[17*4];\
  char SRCEXPR[513*4];\
  __STATE STATE;\
  int STORESTS;\
  BYTE TARGETCHANGE;\
  __TYPE TYPE;\
  };\
  _##Var Var[Count+1];

#undef Struct_EII
#define Struct_EII(Var,tE1,E1,I2,I3)\
struct _##Var\
    {\
    __##tE1 E1;\
    int I2;\
    int I3;\
    };\
    _##Var Var;

#undef Struct2BR
#define Struct2BR(Var,Desc,Val1,Val2,R3)\
struct _##Var\
    {\
    BYTE Val1;\
    BYTE Val2;\
    double R3;\
    };\
    _##Var Var;

#undef StructBR
#define StructBR(Var,Desc,Val1,R2)\
struct _##Var\
    {\
    BYTE Val1;\
    double R2;\
    };\
    _##Var Var;
