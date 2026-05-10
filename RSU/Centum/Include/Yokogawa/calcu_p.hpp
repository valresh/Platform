YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
//
YPARM_D(P01,0.,"----")
YPARM_D(P02,0.,"----")
YPARM_D(P03,0.,"----")
YPARM_D(P04,0.,"----")
YPARM_D(P05,0.,"----")
YPARM_D(P06,0.,"----")
YPARM_D(P07,0.,"----")
YPARM_D(P08,0.,"----")
//
YPARM_I(AlarmLevel,3,"EALA")
YPARM_B(BAD_connection_alarm,false,"ILCN" )// BAD connection alarm
YPARM_L(Input_Open_alarm,0,"INOP","%{NO;H;L;HL}")
YPARM_L(Calc_input_err_detect,0,"CABC","%{NO;CALC;ALL;}")
YPARM_L( Numb_of_IP, 0, "EINP", "%{2ST;3ST}" )
YPARM_B( Out_Trk, false, "OTRK" ); //Output value tracking
YPARM_B(PV_Overshoot,false,"OVPV")
YPARM_B(PV_Limit,false,"PVRL")
YPARM_L(Aux,0,"SUOC","%{CPV;DCPV}")
YPARM_L(Aux_Out_Type,0,"SOAC", "%{POSITION;VELOCITY}")
YPARM_DD(Conv_GAINout,Conv_BIASout,1.,0.,"SSO!","%D:%D")
//
YPARM_B(CPV_RangeLimit,false,"CPVR")
YPARM_L(TotalizerTimeUnit,0,"ESUM","%{NO;SEC;MIN;HOUR;DAY}")//Totalizer time unit, Totalizer low input cut value

YPARM_L( ProcessingTiming, 1, "TMNC", "%{O;T}" )//O - one short, T - periodic execution

YPARM_L(InputSignalConversion,0,"EINP", "%{LINEAR;SQRT;PULSE;QTPUL;SUBSYS}")
YPARM_4D(Data_GAINin,Data_BIASin,H_Limit,L_Limit,1.,0.,100.,0.,"SSI!","%D:%D:%D:%D")











