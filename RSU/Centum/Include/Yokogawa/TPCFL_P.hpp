YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
//
YPARM_D(TB,0.,"----")
YPARM_D(PB,0.,"----")
YPARM_D(GAIN,1.,"RTGN");
//
YPARM_I(AlarmLevel,3,"EALA")
YPARM_B(BAD_connection_alarm,false,"ILCN" )// BAD connection alarm
YPARM_L(Input_Open_alarm,0,"INOP","%{NO;H;L;HL}")
//
YPARM_B(PV_Overshoot,false,"OVPV")
YPARM_L( PositionalOut, 0, "CCOU", "%{POSITION;VELOCITY}" );
YPARM_B( Out_Trk, false, "OTRK" ); //Output value tracking
YPARM_DD(Conv_GAIN,Conv_BIAS,1.,0.,"SSO!","%D:%D")
YPARM_L(Aux,0,"SUOC","%{CPV;DCPV}")
//
YPARM_L(CLTP,0,"CLTP","%{TP;P;T}")// Corrective Computation PT, ...
YPARM_L(PRUN,4,"PRUN","%{HZ0;KGF/CM2;PA;KPA;MPA}")// Размерность P
YPARM_L(TPUN,0,"TPUN","%{C;F}")// Размерность T
