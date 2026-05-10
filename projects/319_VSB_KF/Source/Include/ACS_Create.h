// 
#undef ACS_AI
#define ACS_AI(Var,Point,Descr) CAnalog Var;

#undef ACS_AO
#define ACS_AO(Var,Point,Descr) CAnalogD Var;

#undef ACS_AIO
#define ACS_AIO(Var,Point,Descr) CAnalog Var;

#undef ACS_DI
#define ACS_DI(Var,Point,Descr) CDiscrete Var;

#undef ACS_DO
#define ACS_DO(Var,Point,Descr) CDiscrete Var;

#undef ACS_DIO
#define ACS_DIO(Var,Point,Descr) CDiscrete Var;

#undef F_AI
#define F_AI(Var,Point) CAnalog Var;

#undef F_AO
#define F_AO(Var,Point) CAnalogD Var;

#undef F_DI
#define F_DI(Var,Point) CDiscrete Var;

#undef F_DO
#define F_DO(Var,Point) CDiscrete Var;

#undef VAR_D
#define VAR_D(Var,Descr) CAnalogD Var;
#undef VAR_I
#define VAR_I(Var,Descr) CDiscrete Var;

#undef INT_REG
#define INT_REG(Var,Descr) CIntReg Var;

#undef SET_COMP
#define SET_COMP(Var,Descr) CSetComp Var;

#undef ACS_AI2
#define ACS_AI2(Var,Point) CAnalog Var;

#undef ACS_AO2
#define ACS_AO2(Var,Point) CAnalogD Var;

#undef ACS_DI2
#define ACS_DI2(Var,Point) CDiscrete Var;

#undef ACS_DO2
#define ACS_DO2(Var,Point) CDiscrete Var;

