#undef SKIP
#define SKIP(Name)
#undef YPARM_I
#define YPARM_I(Var,Def,Name) int Var;
#undef YPARM_I_A
#define YPARM_I_A(Var,Dim,Desc) int Var[Dim]; char Var##Name[Dim][8];
#undef YPARM_L
#define YPARM_L(Var,Def,Name,Fmt) int Var;
#undef YPARM_L_savable
#define YPARM_L_savable(Var,Def,Name,Fmt) int Var;
#undef YPARM_N
#define YPARM_N(Var,Def,Name,Fmt) int Var;
#undef YPARM_D
#define YPARM_D(Var,Def,Name) double Var;
#undef YPARM_Dref
#define YPARM_Dref(Var,Def,Name) double Var;
#undef YPARM_D_A
#define YPARM_D_A(Var,Dim,Desc) double Var[Dim]; char Var##Name[Dim][8];
#undef YPARM_D_savable
#define YPARM_D_savable(Var,Def,Name) double Var;
#undef YPARM_D_ALT
#define YPARM_D_ALT(Var,Def,Name,VARaltNAME) double Var;
#undef YPARM_H
#define YPARM_H(Var,Def,Name) double Var;
#undef YPARM_D1
#define YPARM_D1(Var,Def,Name,Fmt,Func) double Var;
#undef YPARM_B
#define YPARM_B(Var,Def,Name) bool Var;
#undef YPARM_FLAG
#define YPARM_FLAG(Var,Def,Name) BYTE Var;
#undef YPARM_S_A
#define YPARM_S_A(Var,Dim,Max,Desc) char Var[Dim][Max+1]; char Var##Name[Dim][8];
#undef YPARM_S
#define YPARM_S(Var,Dim,Name) char Var[Dim];
#undef YPARM_IID
#define YPARM_IID(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) int Var_1; int Var_2; double Var_3;
#undef YPARM_DD
#define YPARM_DD(Var_1,Var_2,Def_1,Def_2,Name,Fmt) double Var_1, Var_2;
#undef YPARM_DD_savable
#define YPARM_DD_savable(Var_1,Var_2,Def_1,Def_2,Name,Fmt) double Var_1, Var_2;
#undef YPARM_DDD
#define YPARM_DDD(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) double Var_1, Var_2, Var_3;
#undef YPARM_4D
#define YPARM_4D(Var_1,Var_2,Var_3,Var_4,Def_1,Def_2,Def_3,Def_4,Name,Fmt) double Var_1, Var_2, Var_3, Var_4;
#undef YPARM_DDS
#define YPARM_DDS(Var_1,Var_2,Var_3,Def_1,Def_2,Dim_3,Name,Fmt) double Var_1, Var_2; char Var_3[Dim_3];
#undef YPARM_BBB
#define YPARM_BBB(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) bool Var_1, Var_2, Var_3;
#undef YPARM_DB
#define YPARM_DB(Var_D,Var_B,Def_D,Def_B,Name,Fmt) double Var_D; bool Var_B;
#undef YPARM_VARIANT
#define YPARM_VARIANT(Name)
//
