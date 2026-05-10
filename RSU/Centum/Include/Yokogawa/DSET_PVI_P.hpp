YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
//
YPARM_Dref(HH,W->SH,"----")
YPARM_Dref(PH,W->SH,"----")
YPARM_Dref(PL,W->SL,"----")
YPARM_Dref(LL,W->SL,"----")
YPARM_Dref(VL,W->SH-W->SL,"----")
//
YPARM_Dref(SVH,W->SH,"----")
YPARM_Dref(SVL,W->SL,"----")
//
YPARM_B(PV_Overshoot,false,"OVPV")
YPARM_B(PV_Limit,false,"PVRL")
YPARM_I(AlarmLevel,12,"EALA")
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
YPARM_DD(Conv_GAIN,Conv_BIAS,1.,0.,"SSO!","%D:%D")
YPARM_B(BAD_connection_alarm,false,"ILCN")// BAD connection alarm
YPARM_H(Out_Vel_Lim,1.,"OVLO")//
//
YPARM_L(Input_Open_alarm,0,"INOP","%{NO;H;L;HL}")
YPARM_L(HHLL,0,"HHLL","%{NO;HH;LL;HHLL}")
YPARM_L(HL,0,"INHL","%{NO;H;L;HL}")
YPARM_H(HYS_HL,2.,"HHLH")
YPARM_L(Sides_VL,0,"INVC","%{NO;SINGLE;BOTH}")
YPARM_IID(N_Samples,Sample_Int,HYS_VL,0,0,2.,"IVHS","%I:%I:%H")

