#undef YPARM_I
#define YPARM_I(Var,Def,Name,Fmt,Func) Func( &Var, Name, Fmt );

#undef YPARM_D
#define YPARM_D(Var)

#undef YPARM_B
#define YPARM_B(Var)

#undef YPARM_FLAG
#define YPARM_FLAG(Var)

#undef YPARM_S
#define YPARM_S(Var,Dim) memset ( Var, 0, sizeof ( Var ));

#undef YPARM_S_A
#define YPARM_S_A(Var,Dim,Max,Name) memset ( Var, 0, sizeof ( Var ));

