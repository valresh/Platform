
#undef INT2
#define INT2(Var,Def,Desc) params.Add( Var, "#"#Var );

#undef INT4
#define INT4(Var,Def,Desc) params.Add( Var, "#"#Var );

#undef Boolean
#define Boolean(Var,Def,Desc) params.Add( Var, "#"#Var );

#undef REAL
#define REAL(Var,Def,Desc) params.Add( Var, "#"#Var );

#undef DBLE
#define DBLE(Var,Def,Desc) params.Add( Var, "#"#Var );

#undef DBLE2
#define DBLE2(Var,VarName,Def,Desc) params.Add( Var, "#"#Var );

#undef STRING
#define STRING(Var,Len,Desc) params.Add( Var, "#"#Var );

#undef STRING_2
#define STRING_2(Var,VarNameDef,Len,Desc) params.Add( Var, "#"#Var );

#undef  ENUMERATION
#define ENUMERATION(Var,Desc,...) params.Add( Var.V, "#"#Var );
