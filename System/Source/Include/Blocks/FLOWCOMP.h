Real(COMPTERM,"Compensation Term",1.)//This term is calculated differently for each of five flow compensation equations, A through E.
Real(F,"Flow Input",NaN)//The value of the uncompensated flow input. This is a square-rooted, differential pressure input.
Enumeration(FSTS,"Flow Input Status",BAD,UNCERTN,NORMAL,MANUAL,INITACK,INITREQ,INITBAD,Def=BAD)
Real(G,"Specific Gravity Input",1.)//Shows value of the specific gravity.
Enumeration(GSTS,"Specific Gravity Input Status",BAD,UNCERTN,NORMAL,MANUAL,INITACK,INITREQ,INITBAD,Def=BAD)
Real(P,"Pressure Input",1.)//Indicates the value of the gage pressure.
Enumeration(PSTS,"Pressure Input Status",BAD,UNCERTN,NORMAL,MANUAL,INITACK,INITREQ,INITBAD,Def=BAD)
Real(PV,"PV Value",NaN)//Compensated flow
Enumeration(PVSTS,"PV Status",BAD,UNCERTN,NORMAL,MANUAL,INITACK,INITREQ,INITBAD,Def=BAD)
Real(Q,"Steam Quality Factor Input",0)//Shows value of the steam quality factor
Enumeration(QSTS,"Steam Quality Input Status",BAD,UNCERTN,NORMAL,MANUAL,INITACK,INITREQ,INITBAD,Def=BAD)
//STRING(QUALSTATE,100,"Qualification State")
Real(T,"Temperature Input",1.)
Enumeration(TSTS,"Steam Quality Input Status",BAD,UNCERTN,NORMAL,MANUAL,INITACK,INITREQ,INITBAD,Def=BAD)
Real(X,"Steam Compressibility Input",1.)
Enumeration(XSTS,"Steam Quality Input Status",BAD,UNCERTN,NORMAL,MANUAL,INITACK,INITREQ,INITBAD,Def=BAD)