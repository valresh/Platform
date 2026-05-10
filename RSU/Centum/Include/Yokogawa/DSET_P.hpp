YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
//
YPARM_Dref(SVH,W->SH,"----")
YPARM_Dref(SVL,W->SL,"----")
//
YPARM_I(AlarmLevel,3,"EALA")
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
YPARM_DD(Conv_GAIN,Conv_BIAS,1.,0.,"SSO!","%D:%D")
YPARM_B(BAD_connection_alarm,false,"ILCN")// BAD connection alarm
YPARM_H(Out_Vel_Lim,1.,"OVLO")//
//

