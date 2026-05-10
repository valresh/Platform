//
YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,100.,0.,"ESCL","%D:%D") 
//
YPARM_4D(Data_GAINin,Data_BIASin,H_Limit,L_Limit,1.,0.,100.,0.,"SSI!","%D:%D:%D:%D")
//
YPARM_L(Label,0,"LBNO", "%{DIR;REV}")
YPARM_I(Calc_Val_Err,1,"CABN")
YPARM_L(PositionalOut,0,"CCOU","%{POSITION;VELOCITY}");
//
YPARM_I(AlarmLevel,3,"EALA")
YPARM_B(BAD_connection_alarm,false,"ILCN" )// BAD connection alarm
YPARM_L(Input_Open_alarm,0,"INOP","%{NO;H;L;HL}")
YPARM_L(Aux,0,"SUOC","%{CPV;DCPV}")

YPARM_D(GN1,1.,"----");
YPARM_D(BS1,0.,"----");
YPARM_D(GAIN,1.,"----");
YPARM_D(BIAS,0.,"----");