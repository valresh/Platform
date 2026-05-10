
#undef Boolean
#define Boolean(Var,Def,Desc) params.Add( Var, "#"#Var );


#undef DBLE
#define DBLE(Var,Def,Desc) params.Add( Var, "#"#Var );

#undef STRING
#define STRING(Var,Def,Desc) params.Add( Var, "#"#Var );

#undef SDEnum
#define SDEnum(Var,Def,Desc) params.Add( Var, "#"#Var );

#undef Int32
#define Int32(Var,Def,Desc) params.Add( Var, "#"#Var );
