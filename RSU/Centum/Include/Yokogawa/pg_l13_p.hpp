YPARM_S(Comment,64,"ETCM")
YPARM_I(PH,0,"----")//*
YPARM_I(PL,0,"----")//*
YPARM_I(ZONE,1,"----")
YPARM_I(ZSTR,1,"----")
YPARM_I(ZEND,1,"----")
YPARM_Dref(OPHI,W->MSH,"----")
YPARM_Dref(OPLO,W->MSL,"----")
YPARM_Dref(MH,W->MSH,"----")
YPARM_Dref(ML,W->MSL,"----")
YPARM_D(MSH,DBL_MAX,"----")
YPARM_D(MSL,DBL_MAX,"----")
//
YPARM_I_A(X,14,"")// ??
//
YPARM_D_A(Y,14,"")// ??
//
YPARM_L(When_program_exits, 0, "PGSA", "%{HD;QR}" )//Hold PV, Quick Return
YPARM_I( Total_Elapsed_Time, 10000, "RUNT" ); //??
YPARM_I(AlarmLevel, 11, "EALA" )
YPARM_B(Output_Open_alarm,false,"ALOP" )
YPARM_B(BAD_connection_alarm,false,"ILCN")
YPARM_DB(Out_VL,MAN_bypass,100.,false,"OVLM", "%D:%B")
YPARM_B(Bumpless_Start,false,"BPSS")
YPARM_L(Time_Unit,0,"TMUT","%{SEC;MIN}")

YPARM_DDS(MVH,MVL,cMesMV,W->MSH,W->MSL,8,"AORA","%D:%D:%S")
YPARM_DD(SH,SL,W->Total_Elapsed_Time,0,"ESCL","%D:%D") 
