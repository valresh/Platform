YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
//
YPARM_Dref(HH,W->SH,"----")
YPARM_Dref(PH,W->SH,"----")
YPARM_Dref(PL,W->SL,"----")
YPARM_Dref(LL,W->SL,"----")
YPARM_Dref(VL,W->SH-W->SL,"----")
YPARM_L(HHLL,0,"HHLL","%{NO;HH;LL;HHLL}")
YPARM_L(HL,0,"INHL","%{NO;H;L;HL}")
YPARM_L(Input_Open_alarm,0,"INOP","%{NO;H;L;HL}")
YPARM_IID(N_Samples,Sample_Int,HYS_VL,0,0,2.,"IVHS","%I:%I:%H")
YPARM_L(Sides_VL,0,"INVC","%{NO;SINGLE;BOTH}")
YPARM_H(HYS_HL,2.,"HHLH")
//
YPARM_D(MSH,DBL_MAX,"----")
YPARM_D(MSL,DBL_MAX,"----")
YPARM_Dref(MH,W->MSH,"----")
YPARM_Dref(ML,W->MSL,"----")
//
YPARM_DB(Out_VL,MAN_bypass,100.,false,"OVLM", "%D:%B")
YPARM_B(Limit_On_CLP,false,"CLPO")
YPARM_L(Positional,1,"CCOU","%{VELOCITY;POSITION}")
YPARM_B(PV_Overshoot,false,"OVPV")
YPARM_B(PV_Limit,false,"PVRL")
YPARM_L(Aux,0,"SUOU","%{PV;DPV;MV;DMV}")
YPARM_B( Out_Trk, false, "OTRK" );
//
YPARM_B(Use_MINMAX,false,"AOTF")
YPARM_DD(Out_MAX,Out_MIN,100.,0.,"AOF!","%D:%D")//106.25:-17.19
//
YPARM_I(AlarmLevel,3,"EALA")
YPARM_B(Output_Open_alarm,false,"ALOP" );
YPARM_B(BAD_connection_alarm,false,"ILCN")
YPARM_L(Display_style_MV,0,"MV99","%{AUTO;SELF}")//Display style for MV data"
YPARM_DDS(MVH,MVL,cMesMV,W->MSH,W->MSL,8,"AORA","%D:%D:%S") 
YPARM_DD(Conv_GAINout,Conv_BIASout,1.,0.,"SSO!","%D:%D")
//
YPARM_L(TotalizerTimeUnit,0,"ESUM","%{NO;SEC;MIN;HOUR;DAY}")//Totalizer time unit, Totalizer low input cut value
YPARM_L(OutputSignalConvertion,0,"OUTP","%{LINEAR;PW;SUBSYS;SUBWFT}")

YPARM_4D(Data_GAINin,Data_BIASin,H_Limit,L_Limit,1.,0.,100.,0.,"SSI!","%D:%D:%D:%D")
YPARM_L(InputSignalConversion,0,"EINP", "%{LINEAR;SQRT;BTHPUL;SUBSYS}")
