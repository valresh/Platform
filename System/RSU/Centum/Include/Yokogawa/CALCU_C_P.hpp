YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
//
YPARM_D(P01,0.,"----")
YPARM_D(P02,0.,"----")
YPARM_D(P03,0.,"----")
YPARM_D(P04,0.,"----")
YPARM_S(P05,20,"----")
YPARM_S(P06,20,"----")
YPARM_S(P07,20,"----")
YPARM_S(P08,20,"----")
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
YPARM_DD(Conv_GAIN,Conv_BIAS,1.,0.,"SSO!","%D:%D")
//
SKIP("CPVR")//??
YPARM_L(TotalizerTimeUnit,0,"ESUM","%{NO;SEC;MIN;HOUR;DAY}")//Totalizer time unit, Totalizer low input cut value

YPARM_L( ProcessingTiming, 1, "TMNC", "%{O;T}" )//O - one short, T - periodic execution












