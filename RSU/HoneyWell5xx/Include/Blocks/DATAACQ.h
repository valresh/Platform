Boolean(INSFAILFL,"Insertion Fail Flag",false)// - Indicates if an alarm condition has been detected for an insertion failure
Real(LASTGOODPV,"Last good Process Variable (PV) value",NaN)
Real(P1,"Process Input Value #1",NaN)// - The current value of the process input to a Data Acquisition function block
Real(P1EU,"",NaN)
Boolean(P1FILTINIT,"P1 Filter Initialization Request Flag",false)// - Used to reset the filtered P1 value
Enumeration(P1STS,"P1 Status",Bad,Uncertain,Normal,Manual,Def=Bad)
Real(PV,"Contains the current value of the PV",NaN)//after it has been selected from one of the following sources: a field device, another function block, an operator, or a program
Real(PVAUTO,"Filtered and clamped value of the Process Input 1 (P1)",NaN)
Enumeration(PVAUTOSTS,"PVAUTO Status",Bad,Uncertain,Normal,Manual,Def=Bad)
Boolean(PVEXHIFL,"(PV) High Limit Flag",false)// - Indicates if PV has exceeded its high limit.
Boolean(PVEXLOFL,"(PV) Low Limit Flag",false)//Indicates if PV has exceeded its low limit.
Real(PVP,"Process Variable (PV) Value (in Percent)",NaN)
Enumeration(PVSTS,"Process Variable (PV) Status",Bad,Uncertain,Normal,Manual,Def=Bad)
Struct4B(PVSTSFL,"",BAD,MAN,NORM,UNCER)
StructRE(PVVALSTS,"",PV,PVSTS)
Struct_BEIR(ROCNEGALM,FL,ALPRIOR,PR,SV,TP)
Struct_BEIR(ROCPOSALM,FL,ALPRIOR,PR,SV,TP)