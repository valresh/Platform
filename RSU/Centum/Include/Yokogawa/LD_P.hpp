//
YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
//
YPARM_D(GAIN,1.,"RTGN");
YPARM_D(D,1.,"D");
//
YPARM_L( InputSignalConvertion, 0, "EINP", "%{LINEAR,SQRT,PULSE,QTPUL,SUBSYS}" )
YPARM_L( OutputSignalConvertion, 0, "OUTL", "%{LINEAR,SUBSYS}" )
YPARM_L( AuxOutputType, 0, "SOAC", "%{POSITION;VELOCITY}" )
YPARM_L( AuxDataType,0,"SUOC","%{CPV;DCPV}")
YPARM_L( InputOpenAlarm,0,"INOP","%{HL,H,L,NO}")
