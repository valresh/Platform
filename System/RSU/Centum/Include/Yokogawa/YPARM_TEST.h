
#undef SKIP
#define SKIP(Name)

#undef YPARM_I
#define YPARM_I(Var,Def,Name)

#undef YPARM_I_A
#define YPARM_I_A(Var,Dim,Desc)

#undef YPARM_L
#define YPARM_L(Var,Def,Name,Fmt)

#undef YPARM_L_savable
#define YPARM_L_savable(Var,Def,Name,Fmt)

#undef YPARM_N
#define YPARM_N(Var,Def,Name,Fmt)

#undef YPARM_D
#define YPARM_D(Var,Def,Name) \
  TestParm(#Var,W->Var,Def);

#undef YPARM_Dref
#define YPARM_Dref(Var,Def,Name) \
  TestParm(#Var,W->Var,Def);

#undef  YPARM_D_A
#define YPARM_D_A(Var,Dim,Desc)

#undef YPARM_D_savable
#define YPARM_D_savable(Var,Def,Name) \
  TestParm(#Var,W->Var,Def);

#undef YPARM_D_ALT
#define YPARM_D_ALT(Var,Def,Name,VARaltNAME) \
  TestParm(#Var,W->Var,Def);

#undef YPARM_D1
#define YPARM_D1(Var,Def,Name,Fmt,Func) \
  TestParm(#Var,W->Var,Def);

#undef YPARM_H
#define YPARM_H(Var,Def,Name) \
  TestParm(#Var,W->Var,Def);

#undef YPARM_B
#define YPARM_B(Var,Def,Name)

#undef YPARM_FLAG
#define YPARM_FLAG(Var,Def,Name)

#undef YPARM_S
#define YPARM_S(Var,Dim,Name)

#undef YPARM_S_A
#define YPARM_S_A(Var,Dim,Max,Name)

#undef YPARM_IID
#define YPARM_IID(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) \
  TestParm(#Var_3,W->Var_3,Def_3);

#undef YPARM_DDD
#define YPARM_DDD(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) \
  TestParm(#Var_1,W->Var_1,Def_1);\
  TestParm(#Var_2,W->Var_2,Def_2);\
  TestParm(#Var_3,W->Var_3,Def_3);

#undef YPARM_4D
#define YPARM_4D(Var_1,Var_2,Var_3,Var_4,Def_1,Def_2,Def_3,Def_4,Name,Fmt) \
  TestParm(#Var_1,W->Var_1,Def_1);\
  TestParm(#Var_2,W->Var_2,Def_2);\
  TestParm(#Var_3,W->Var_3,Def_3);\
  TestParm(#Var_4,W->Var_4,Def_4);

#undef YPARM_DDS
#define YPARM_DDS(Var_1,Var_2,Var_3,Def_1,Def_2,Dim_3,Name,Fmt) \
  TestParm(#Var_1,W->Var_1,Def_1);\
  TestParm(#Var_2,W->Var_2,Def_2);

#undef YPARM_BBB
#define YPARM_BBB(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt)

#undef YPARM_DD
#define YPARM_DD(Var_1,Var_2,Def_1,Def_2,Name,Fmt) \
  TestParm(#Var_1,W->Var_1,Def_1);\
  TestParm(#Var_2,W->Var_2,Def_2);

#undef YPARM_DD_savable
#define YPARM_DD_savable(Var_1,Var_2,Def_1,Def_2,Name,Fmt) \
  TestParm(#Var_1,W->Var_1,Def_1);\
  TestParm(#Var_2,W->Var_2,Def_2);

#undef YPARM_DB
#define YPARM_DB(Var_1,Var_2,Def_1,Def_2,Name,Fmt) \
  TestParm(#Var_1,W->Var_1,Def_1);

#undef YPARM_VARIANT
#define YPARM_VARIANT(Name)

//////////////////////////////////////////////////////
#undef  UMODE
#define UMODE

#undef ACK_ALARMS
#define ACK_ALARMS
//
#undef FLAG
#define FLAG(Var,Comm)
//
#undef DOUBLE
#define DOUBLE(Var,Comm)

#undef DOUBLE2
#define DOUBLE2(Var,Var_Tune,Comm)

#undef INTEGER
#define INTEGER(Var,Comm)

#undef DOUBLE_A
#define DOUBLE_A(Var,Dim,Comm)

#undef INTEGER_A
#define INTEGER_A(Var,Dim,Desc)

#undef ENUM
#define ENUM(Var,Desc,...)

#undef  STRING_A
#define STRING_A(Var,Dim,Max,Desc)

#undef  STRING_skipSave
#define STRING_skipSave(Var,Dim,Max,Desc)
//
#undef YPARM_VARIANT
#define YPARM_VARIANT(Name)

#undef  ACK_ALARMS
#define ACK_ALARMS
