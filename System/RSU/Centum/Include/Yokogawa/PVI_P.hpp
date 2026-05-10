//
YPARM_S(Comment,64,"ETCM")
//
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
YPARM_I( nAfterDotPV, -1, "кол-во знаков после . для PV" )
//
YPARM_Dref(HH,W->SH,"----")
YPARM_Dref(PH,W->SH,"----")
YPARM_Dref(PL,W->SL,"----")
YPARM_Dref(LL,W->SL,"----")
YPARM_Dref(VL,W->SH-W->SL,"----")
//
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
YPARM_L(Aux,0,"SUOP","%{PV;DPV}")
//
YPARM_B(PV_Overshoot,false,"OVPV")
YPARM_B(PV_Limit,false,"PVRL")
YPARM_I(AlarmLevel,12,"EALA")
YPARM_L(Input_Open_alarm,0,"INOP","%{NO;H;L;HL}")
YPARM_L(HHLL,0,"HHLL","%{NO;HH;LL;HHLL}")
YPARM_L(HL,0,"INHL","%{NO;H;L;HL}")
YPARM_H(HYS_HL,2.,"HHLH")
YPARM_L(Sides_VL,0,"INVC","%{NO;SINGLE;BOTH}")
YPARM_IID(N_Samples,Sample_Int,HYS_VL,0,0,2.,"IVHS","%I:%I:%H")
YPARM_4D(Data_GAINin,Data_BIASin,H_Limit,L_Limit,1.,0.,100.,0.,"SSI!","%D:%D:%D:%D")
YPARM_DD(Conv_GAINout,Conv_BIASout,1.,0.,"SSO!","%D:%D")

YPARM_L(TotalizerTimeUnit,0,"ESUM","%{NO;SEC;MIN;HOUR;DAY}")//Totalizer time unit, Totalizer low input cut value
SKIP("ILCN")//Bad joint status alarm"
SKIP("SOAC")//Auxiliary output action(POSITION,VELOCITY)"
//
YPARM_L(InputSignalConversion,0,"EINP", "%{LINEAR;SQRT;BTHPUL;SUBSYS}")
