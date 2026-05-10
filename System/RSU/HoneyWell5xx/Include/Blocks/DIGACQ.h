Boolean(IN,"Input Value usually from Channel Block",false)
String_K(PV,"Actual State",12*4)
Boolean(PVAUTOFL,"Field PV Value",false)
Boolean(PVFL,"Actual State Flag",false)
Enumeration(PVSTS,"Process Variable (PV) Status",BAD,UNCERTAIN,NORMAL,MANUAL,INITACK,INITREQ,INITBAD,Def=BAD)
Struct4B(PVSTSFL,"",BAD,MAN,NORM,UNCERTN)//BAD,"Bad PV Flag"|MAN,"PVSouce Manual"|NORM,"PVSouce Auto"| ,"PVSouce Sub"
Boolean(STATE0,"",false)
Boolean(STATE1,"",false)
