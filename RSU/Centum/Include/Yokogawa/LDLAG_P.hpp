YPARM_D(GAIN,1.,"RTGN");
YPARM_D(D,0,"----")
YPARM_D(I,1.,"----")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
YPARM_L(Aux,0,"SUOC","%{CPV;DCPV}")
YPARM_L(Aux_Out_Type,0,"SOAC", "%{POSITION;VELOCITY}")
YPARM_L( InputSignalConvertion, 0, "EINP", "%{LINEAR,SQRT,PULSE,QTPUL,SUBSYS}" )
YPARM_L( OutputSignalConvertion, 0, "OUTL", "%{LINEAR,SUBSYS}" )
