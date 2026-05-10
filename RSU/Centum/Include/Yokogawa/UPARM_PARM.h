#undef SKIP
#define SKIP(Name)


#undef YPARM_I
#define YPARM_I(Var,Def,Name) \
  PARM ( Var, "#"#Var );

#undef YPARM_I_A
#define YPARM_I_A(Var,Dim,Max,Desc)

#undef YPARM_L
#define YPARM_L(Var,Def,Name,Fmt) \
  PARM ( Var, "#"#Var );

#undef YPARM_N
#define YPARM_N(Var,Def,Name,Fmt) \
  PARM ( Var, Name );

#undef YPARM_D
#define YPARM_D(Var,Def,Name) \
  PARM ( Var, "#"#Var );

#undef YPARM_Dref
#define YPARM_Dref(Var,Def,Name) \
  PARM ( Var, "#"#Var );

#undef YPARM_D_A
#define YPARM_D_A(Var,Dim,Max,Desc)

#undef YPARM_D_ALT
#define YPARM_D_ALT(Var,Def,Name,VARaltName) \
  PARM ( Var, "#"#Var );

#undef YPARM_D1
#define YPARM_D1(Var,Def,Name,Fmt,Func) \
  PARM ( Var, "#"#Var );

#undef YPARM_H
#define YPARM_H(Var,Def,Name) \
  PARM ( Var, "#"#Var );

#undef YPARM_B
#define YPARM_B(Var,Def,Name) \
  PARM ( Var, "#"#Var );

#undef YPARM_FLAG
#define YPARM_FLAG(Var,Def,Name) \
  PARM ( Var, "#"#Var );

#undef YPARM_S
#define YPARM_S(Var,Dim,Name) \
  PARM ( Var, "#"#Var );

#undef YPARM_S_A
#define YPARM_S_A(Var,Dim,Max,Name)

#undef YPARM_IID
#define YPARM_IID(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) \
  PARM ( Var_1, "#"#Var_1 );\
  PARM ( Var_2, "#"#Var_2 );\
  PARM ( Var_3, "#"#Var_3 );

#undef YPARM_DDD
#define YPARM_DDD(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) \
  PARM ( Var_1, "#"#Var_1 );\
  PARM ( Var_2, "#"#Var_2 );\
  PARM ( Var_3, "#"#Var_3 );

#undef YPARM_4D
#define YPARM_4D(Var_1,Var_2,Var_3,Var_4,Def_1,Def_2,Def_3,Def_4,Name,Fmt) \
  PARM ( Var_1, "#"#Var_1 );\
  PARM ( Var_2, "#"#Var_2 );\
  PARM ( Var_3, "#"#Var_3 );\
  PARM ( Var_4, "#"#Var_4 );

#undef YPARM_DDS
#define YPARM_DDS(Var_1,Var_2,Var_3,Def_1,Def_2,Dim_3,Name,Fmt) \
  PARM ( Var_1, "#"#Var_1 );\
  PARM ( Var_2, "#"#Var_2 );\
  PARM ( Var_3, "#"#Var_3 );
 
#undef YPARM_BBB
#define YPARM_BBB(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) \
  PARM ( Var_1, "#"#Var_1 );\
  PARM ( Var_2, "#"#Var_2 );\
  PARM ( Var_3, "#"#Var_3 );


#undef YPARM_DD
#define YPARM_DD(Var_1,Var_2,Def_1,Def_2,Name,Fmt) \
  PARM ( Var_1, "#"#Var_1 );\
  PARM ( Var_2, "#"#Var_2 );

#undef YPARM_DB
#define YPARM_DB(Var_1,Var_2,Def_1,Def_2,Name,Fmt) \
  PARM ( Var_1, "#"#Var_1 );\
  PARM ( Var_2, "#"#Var_2 );

#undef YPARM_VARIANT
#define YPARM_VARIANT(Name)

//////////////////////////////////////////////////////
#undef  UMODE
#define UMODE

#undef ACK_ALARMS
#define ACK_ALARMS
//
#undef FLAG
#define FLAG(Var,Comm) \
  PARM ( Var, "#"#Var );
//
#undef DOUBLE
#define DOUBLE(Var,Comm) \
  PARM ( Var, "#"#Var );

#undef DOUBLE2
#define DOUBLE2(Var,Var_Tune,Comm) \
  PARM ( Var, "#"#Var );\
  PARM ( Var_Tune, "#"#Var_Tune );

#undef INTEGER
#define INTEGER(Var,Comm) \
  PARM ( Var, "#"#Var );

#undef INTEGER_A
#define INTEGER_A(Var,Dim,Desc) \
  {\
  char Name[128];\
  for ( int n = 0; n < Dim; n++ )\
    {\
    Sprintf( Name, "#%s", Var##Name[n] );\
    PARM ( Var[n], Name );\
    }\
  }

#undef DOUBLE_A
#define DOUBLE_A(Var,Dim,Desc) \
  {\
  char Name[128];\
  for ( int n = 0; n < Dim; n++ )\
    {\
    Sprintf( Name, "#%s", Var##Name[n] );\
    PARM ( Var[n], Name );\
    }\
  }

#undef  ACK_ALARMS
#define ACK_ALARMS
