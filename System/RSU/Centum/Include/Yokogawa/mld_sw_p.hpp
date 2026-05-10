YPARM_S(Comment,64,"ETCM")
YPARM_DD(SSH,SSL,DBL_MAX,DBL_MAX, "SVRG","%D:%D")
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
YPARM_D(MSH,100.,"----")
YPARM_D(MSL,0.,"----")
YPARM_D(MH,100.,"----")
YPARM_D(ML,0.,"----")
YPARM_B(BIAS_Trk,false,"BSTK")
YPARM_B(Out_Trk,false,"OTRK")
YPARM_B(Limit_On_CLP,false,"CLPO")
YPARM_L(Aux,0,"SUOM","%{MV;DMV}")
//
YPARM_B(Use_MINMAX,false,"AOTF")
YPARM_DD(Out_MAX,Out_MIN,100.,0.,"AOF!","%D:%D")//106.25:-17.19
YPARM_DDS(MVH,MVL,cMesMV,W->MSH,W->MSL,8,"AORA","%D:%D:%S") 
YPARM_L(Display_style_MV,0,"MV99","%{AUTO;SELF}")//Display style for MV data"
//
YPARM_I(AlarmLevel,3,"EALA")
YPARM_B(Output_Open_alarm,false,"ALOP" );
YPARM_B(BAD_connection_alarm,false,"ILCN")
YPARM_L(HL_OUT,0,"HLLT","%{NO;H;L;HL}")
YPARM_H(HYS_OUT,1.,"HLHT")
YPARM_DB(Out_VL,MAN_bypass,100.,false,"OVLM", "%D:%B")
//
YPARM_DD(GAIN,BIAS,1.,0.,"SSO!","%D:%D")

YPARM_L(OutputSignalConvertion,0,"OUTP","%{LINEAR;PW;SUBSYS;SUBWFT}")
YPARM_DDD(FullStrokeValue,BacklashCompensatedValue,MinOutputWidth,0.,0.,0.,"PWF!","%D:%D:%D")

YPARM_D(PMV,0,"")
