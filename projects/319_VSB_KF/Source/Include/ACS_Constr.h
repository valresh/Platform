#undef ACS_AI 
#define ACS_AI(Var,Point,Descr) , Var( pMain ? pMain : this, Point, "AI", Descr )

#undef ACS_AO
#define ACS_AO(Var,Point,Descr) , Var( pMain ? pMain : this, Point, "AO", Descr )

#undef ACS_AIO
#define ACS_AIO(Var,Point,Descr) , Var( pMain ? pMain : this, Point, "AIO", Descr )

#undef ACS_DI
#define ACS_DI(Var,Point,Descr) , Var( pMain ? pMain : this, Point, "DI", Descr )

#undef ACS_DO
#define ACS_DO(Var,Point,Descr) , Var( pMain ? pMain : this, Point, "DO", Descr )

#undef F_AI
#define F_AI(Var,Point) , Var( this, Point, "AIF" )

#undef F_AO
#define F_AO(Var,Point) , Var( this, Point, "AOF" )

#undef F_DI
#define F_DI(Var,Point) , Var( this, Point, "DIF" )

#undef F_DO
#define F_DO(Var,Point) , Var( this, Point, "DOF" )

#undef VAR_D
#define VAR_D(Var,Descr) , Var( this, #Var, "AM" )
#undef VAR_I
#define VAR_I(Var,Descr) , Var( this, #Var, "DM" )

#undef INT_REG
#define INT_REG(Var,Descr) , Var( pMain ? pMain : this, Descr )

#undef SET_COMP
#define SET_COMP(Var,Descr) , Var( this, Descr )

#undef ACS_AI2
#define ACS_AI2(Var,Point) , Var( this, Point, "AI", "" )

#undef ACS_AO2
#define ACS_AO2(Var,Point) , Var( this, Point, "AO", "" )

#undef ACS_DI2
#define ACS_DI2(Var,Point) , Var( this, Point, "DI", "" )

#undef ACS_DO2
#define ACS_DO2(Var,Point) , Var( this, Point, "DO", "" )

