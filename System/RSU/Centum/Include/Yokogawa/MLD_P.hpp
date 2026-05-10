YPARM_S(Comment,64,"ETCM")
YPARM_B( Out_Trk, false, "OTRK" );
YPARM_DB(Out_VL,MAN_bypass,100.,false,"OVLM", "%D:%B")
YPARM_B(Limit_On_CLP,false,"CLPO")
YPARM_L(Positional,1,"CCOU","%{VELOCITY;POSITION}")
YPARM_D(MSH,DBL_MAX,"----")
YPARM_D(MSL,DBL_MAX,"----")
YPARM_Dref(MH,W->MSH,"----")
YPARM_Dref(ML,W->MSL,"----")
YPARM_B(Use_MINMAX,false,"AOTF")
YPARM_DD(Out_MAX,Out_MIN,100.,0.,"AOF!","%D:%D")//106.25:-17.19
YPARM_L(Aux,0,"SUOM","%{MV;DMV}")

YPARM_I(AlarmLevel,3,"EALA")
YPARM_B(Output_Open_alarm,false,"ALOP" );
YPARM_B(BAD_connection_alarm,false,"ILCN")
YPARM_L(Display_style_MV,0,"MV99","%{AUTO;SELF}")//Display style for MV data"
YPARM_DDS(MVH,MVL,cMesMV,W->MSH,W->MSL,8,"AORA","%D:%D:%S") 
YPARM_DD(Conv_GAIN,Conv_BIAS,1.,0.,"SSO!","%D:%D")
YPARM_L(OutputSignalConvertion,0,"OUTP","%{LINEAR;PW;SUBSYS;SUBWFT}")
