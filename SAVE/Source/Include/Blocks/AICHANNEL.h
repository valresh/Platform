Boolean(ACCEPTDEV,"Accept Device ID",false)
Boolean(ACCEPTRNG,"Accept Device Ranges",false)
Boolean(BADPVFL,"BAD PV",false)
Enumeration(COMMAND,"Command. Lets the user initiate ...",None,Def=None)
Real(DAMPING,"",0)
Boolean(HPVMISM,"Device PV Range Mismatch",false)
Boolean(HVARALERT,"Device Variable Alert",false)
Real(PV,"Process Variable",NaN)
Real(PVRAW,"Process Variable Raw Value",NaN)
Enumeration(PVSTS,"Process Variable (PV) Status",BAD,UNCERTAIN,NORMAL,MANUAL,Def=BAD)

Real_A(HDYNVAL,"Value",4,NaN)
Boolean(HENABLE,"Enable HART",false)
/*
Boolean(BADCAL,"Bad Calibration Indicator ",false)// - On a per channel basis, this parameter indicates if calibration is bad or missing.
Integer(BADCODE,"Reason for Bad States",0)
Boolean(OVERRANGE,"Over Range Alarm Indicator",false)//- Indicates the input value is above the valid hardware input range of the channel.
Enumeration(PVSTS,"Process Variable (PV) Status",BAD,UNCERTAIN,NORMAL,MANUAL,Def=BAD)
StructRE(PVVALSTS,"",PV,PVSTS)
Boolean(UNDERRANGE,"Under Range Alarm Indicator",true)//- Indicates that the input value is under the valid hardware input range of the channel. 
                                                      //Input is not in an under-range state (0)
                                                      //Input is in an under-range state (1)
*/
