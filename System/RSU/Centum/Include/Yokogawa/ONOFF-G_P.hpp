YPARM_S(Comment,64,"ETCM")

YPARM_I(AlarmLevel,12,"EALA")
YPARM_L(Input_Open_alarm,0,"INOP","%{NO;H;L;HL}")
YPARM_L(HHLL,0,"HHLL","%{NO;HH;LL;HHLL}")
YPARM_L(HL,0,"INHL","%{NO;H;L;HL}")
YPARM_H(HYS_HL,2.,"HHLH")

YPARM_D(MSH,DBL_MAX,"----")
YPARM_D(MSL,DBL_MAX,"----")

YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
YPARM_L(Aux,0,"SUOP","%{PV;DPV}")
YPARM_Dref(HH,W->SH,"----")
YPARM_Dref(PH,W->SH,"----")
YPARM_Dref(PL,W->SL,"----")
YPARM_Dref(LL,W->SL,"----")
YPARM_Dref(VL,W->SH-W->SL,"----")
YPARM_Dref(DL,W->SH-W->SL,"----")
YPARM_Dref(SVH,W->SH,"----")
YPARM_Dref(SVL,W->SL,"----")
YPARM_D(CK,1.,"----")
YPARM_D(CB,0.,"----")
YPARM_D(PMV,0.,"----")
YPARM_I(BSW,0,"----")

YPARM_L(PIDD,1,"PIDD","%{REVERSE;DIRECT}")
YPARM_IID(N_Samples,Sample_Int,HYS_VL,0,0,2.,"IVHS","%I:%I:%H")
YPARM_DDD(GAIN_DL,Time_DL,HYS_DL,1.,1.,2.,"DVFH", "%D:%D:%H")

YPARM_D(DB,2,"----")//neutral status range
YPARM_D(onfHYS,2.,"ONFH")

YPARM_B(Trk_MAN,false,"MJTO")
