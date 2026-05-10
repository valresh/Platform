YPARM_S(Comment,64,"ETCM")
YPARM_DD(SSH,SSL,DBL_MAX,DBL_MAX, "SVRG","%D:%D")
YPARM_VARIANT("MVRG")
YPARM_DD(MV1H,MV1L,DBL_MAX,DBL_MAX, "MVRG:1","%D:%D")
YPARM_DD(MV2H,MV2L,DBL_MAX,DBL_MAX, "MVRG:2","%D:%D")
YPARM_VARIANT("MVRS")
YPARM_DD(SRH1,SRL1,DBL_MAX,DBL_MAX, "MVRS:1","%D:%D")
YPARM_DD(SRH2,SRL2,DBL_MAX,DBL_MAX, "MVRS:2","%D:%D")
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
//
YPARM_Dref(SVH,W->SSH,"----")
YPARM_Dref(SVL,W->SSL,"----")
//
YPARM_I(AlarmLevel,12,"EALA")
YPARM_B(Output_Open_alarm,true,"ALOP") 
YPARM_B(BAD_connection_alarm,false,"ILCN" )// BAD connection alarm
YPARM_B(Limit_On_CLP,false,"CLPO")
YPARM_VARIANT("MVDR")
YPARM_L(MV1_direct,1,"MVDR:1","%{REVERSE;DIRECT}")
YPARM_L(MV2_direct,1,"MVDR:2","%{REVERSE;DIRECT}")
YPARM_D(MSL1,0.,"----")
YPARM_D(MSH1,100.,"----")
YPARM_D(MSL2,0.,"----")
YPARM_D(MSH2,100.,"----")
YPARM_DD(Conv_GAIN,Conv_BIAS,1.,0.,"SSO!","%D:%D")
//
YPARM_L(Display_style_MV,0,"MV99","%{AUTO;SELF}")//Display style for MV data"
