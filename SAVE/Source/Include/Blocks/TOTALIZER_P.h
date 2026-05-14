Real(ACCTV,"Accumulator Target Value",0)
Real(C1,"Engineering Units scaling factor",1.)// for the calculation of DELAYTIME or Totalizer Process Variable
Enumeration(CMDATTR,"Totalizer Command Attribute",Operator,OtherFB,Def=Operator)
Struct_R(CUTOFF,LM,0)
String_K(DESC,"Block Descriptor",132*4)
String_K(EUDESC,"Block Engineering Units Descriptor",24*4)
Enumeration(HIALM,"",NoAlarm,Config,OPHi,OPLo,AdvDev,DevHi,DevLo,SafetyIlock,BadCtl,BadCompTerm,Def=NoAlarm)
String_K(NAME,"",32*4)
//Integer(ORDERINCM,"Execution order",10)
Enumeration(PVEQN,"PV Equation Type",EqA,EqB,EqC,EqD,EqE,EqF,Def=EqA)
Enumeration(PVFORMAT,"Process Variable (PV) Display Format",D0,D1,D2,D3,Def=D1)//D0 (-XXXXXX.) D1 (-XXXXX.X) D2 (-XXXX.XX) D3 (XXX.XXX)
Enumeration(TIMEBASE,"",Seconds,Minutes,Hours,Def=Minutes)//Defines whether the Process Input Value (P1) input should be sampled every second, minute, or hour
Real(RESETVAL,"Accumulator Reset Value",0)//- Value that the Totalizer function block uses to reset the accumulator. When RESET is applied, the Process Variable (PV) is set to RESETVAL

